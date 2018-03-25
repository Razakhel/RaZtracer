#include <limits>
#include <iostream>

#include "RaZtracer/Math/Matrix.hpp"
#include "RaZtracer/Math/Quaternion.hpp"
#include "RaZtracer/Render/Scene.hpp"

namespace {

const float PHI = (std::sqrt(5.f) + 1) / 2;
std::mt19937 randomGenerator(0);

void buildOrthoBase(const Vec3f& yAxis, Vec3f& xAxis, Vec3f& zAxis) {
  const float sign = std::copysign(1.f, yAxis[2]);
  const float a = -1.f / (sign + yAxis[2]);
  const float b = yAxis[0] * yAxis[1] * a;

  xAxis = Vec3f({ 1.f + sign * yAxis[0] * yAxis[0] * a, sign * b, -sign * yAxis[0] });
  zAxis = Vec3f({ b, sign + yAxis[1] * yAxis[1] * a, -yAxis[1] });
}

} // namespace

void Scene::enableAmbientOcclusion(bool enabled, uint16_t raySamples) {
  m_params.ambientOcclusion = enabled;
  m_params.ambOccRaySamples = raySamples;
}

ImagePtr Scene::render() const {
  if (m_bvh)
    return renderBVH();
  else
    return renderStandard();
}

float Scene::computeLighting(const RayHit& hit) const {
  const Vec3f position = hit.position + hit.normal * 0.0001f;
  uint16_t hitCount = 0;

  for (const auto& light : m_lights) {
    const Vec3f lightDir = (light->getPosition() - hit.position).normalize();

    if (!m_bvh) {
      for (const auto& shapeObstacle : m_shapes) {
        if (shapeObstacle->intersect(position, lightDir)) {
          ++hitCount;
          break;
        }
      }
    } else {
      RayHit bvhHit {};
      if (m_bvh->recoverHitShape(m_shapes, position, lightDir, bvhHit))
        ++hitCount;
    }
  }

  return (1 - (static_cast<float>(hitCount) / m_lights.size()));
}

float Scene::computeAmbientOcclusion(const RayHit& hit) const {
  const Vec3f position = hit.position + hit.normal * 0.0001f;
  float hitCount = 0;

  std::uniform_int_distribution<uint16_t> randomCirc(0, 360);

  for (uint16_t raySample = 0; raySample < m_params.ambOccRaySamples; ++raySample) {
    const float cosTheta = 1 - (2.f * raySample + 1) / (2 * m_params.ambOccRaySamples);
    const float sinTheta = std::sqrt(1 - cosTheta * cosTheta);

    const float samplePhi = raySample / PHI;
    const float phi = 2 * pi<float> * (samplePhi - std::floor(samplePhi));
    const float cosPhi = std::cos(phi);
    const float sinPhi = std::sin(phi);

    Vec4f direction({ cosPhi * sinTheta, cosTheta, sinPhi * sinTheta, 1.f });

    const Quaternion<float> rotation(randomCirc(randomGenerator), 0.f, 1.f, 0.f);
    direction = rotation.computeMatrix() * direction;
    direction /= direction[3];

    Vec3f xAxis {}, zAxis {};
    buildOrthoBase(hit.normal, xAxis, zAxis);

    const Vec3f worldDirection = xAxis * direction[0] + hit.normal * direction[1] + zAxis * direction[2];

    const float weight = cosTheta * 2;

    if (!m_bvh) {
      for (const auto& shape : m_shapes) {
        if (shape->intersect(position, worldDirection)) {
          hitCount += weight;
          break;
        }
      }
    } else {
      RayHit bvhHit {};
      if (m_bvh->recoverHitShape(m_shapes, position, worldDirection, bvhHit))
        hitCount += weight;
    }
  }

  return (1 - (hitCount / m_params.ambOccRaySamples));
}

ImagePtr Scene::renderStandard() const {
  const unsigned int imgWidth = m_camera->getFrameWidth();
  const unsigned int imgHeight = m_camera->getFrameHeight();

  const float scaleFactor = std::tan(m_camera->getFieldOfView() / 2);
  const float aspectRatio = m_camera->getAspectRatio() * scaleFactor;

  const float sampleStride = 1.f / m_params.multiSamplingSamples;
  std::normal_distribution<float> normalDistrib(0.5f * sampleStride, (m_params.multiSamplingSamples != 1 ? 0.1f * sampleStride : 0.f));

  ImagePtr img = std::make_unique<Image>(imgWidth, imgHeight);

  #pragma omp parallel for schedule(dynamic, 1)
  for (std::size_t heightIndex = 0; heightIndex < imgHeight; ++heightIndex) {
    const std::size_t finalHeightIndex = heightIndex * imgWidth;
    std::cout << "Computed: " << (static_cast<float>(heightIndex) / imgHeight) * 100 << '%' << std::endl;

    for (std::size_t widthIndex = 0; widthIndex < imgWidth; ++widthIndex) {
      Vec3f sumColor {};

      for (float heightSample = heightIndex; heightSample < heightIndex + 1.f; heightSample += sampleStride) {
        const float heightDirection = (1 - 2 * (heightSample + normalDistrib(randomGenerator)) / imgHeight) * scaleFactor;

        for (float widthSample = widthIndex; widthSample < widthIndex + 1.f; widthSample += sampleStride) {
          const float widthDirection = (2 * (widthSample + normalDistrib(randomGenerator)) / imgWidth - 1) * aspectRatio;

          const Vec3f screenSpaceDirection({ widthDirection, heightDirection, -1.f });
          const Vec3f rayDirection = screenSpaceDirection.normalize();

          RayHit hit {};
          RayHit closestHit {};

          for (const auto& shape : m_shapes) {
            if (shape->intersect(m_camera->getPosition(), rayDirection, &hit) && hit.distance < closestHit.distance)
              closestHit = hit;
          }

          if (closestHit.distance < std::numeric_limits<float>::infinity()) {
            Vec3f hitColor = closestHit.material->getDiffuseColor();

            if (m_params.ambientOcclusion)
              hitColor *= computeAmbientOcclusion(closestHit);

            if (!m_lights.empty())
              hitColor *= computeLighting(closestHit);
            else
              hitColor *= 1 - (rayDirection.dot(closestHit.normal) / 2 + 0.5f); // Simulate lighting coming from camera

            sumColor += hitColor;
          }
        }
      }

      const Vec3b finalColor((sumColor * 255) / (m_params.multiSamplingSamples * m_params.multiSamplingSamples));
      const std::size_t finalIndex = (finalHeightIndex + widthIndex) * 3;
      img->getData()[finalIndex]     = finalColor[0];
      img->getData()[finalIndex + 1] = finalColor[1];
      img->getData()[finalIndex + 2] = finalColor[2];
    }
  }

  return img;
}

ImagePtr Scene::renderBVH() const {
  const unsigned int imgWidth = m_camera->getFrameWidth();
  const unsigned int imgHeight = m_camera->getFrameHeight();

  const float scaleFactor = std::tan(m_camera->getFieldOfView() / 2);
  const float aspectRatio = m_camera->getAspectRatio() * scaleFactor;

  const float sampleStride = 1.f / m_params.multiSamplingSamples;
  std::normal_distribution<float> normalDistrib(0.5f * sampleStride, (m_params.multiSamplingSamples != 1 ? 0.1f * sampleStride : 0.f));

  ImagePtr img = std::make_unique<Image>(imgWidth, imgHeight);

  #pragma omp parallel for schedule(dynamic, 1)
  for (std::size_t heightIndex = 0; heightIndex < imgHeight; ++heightIndex) {
    const std::size_t finalHeightIndex = heightIndex * imgWidth;
    std::cout << "Computed: " << (static_cast<float>(heightIndex) / imgHeight) * 100 << '%' << std::endl;

    for (std::size_t widthIndex = 0; widthIndex < imgWidth; ++widthIndex) {
      Vec3f sumColor {};

      for (float heightSample = heightIndex; heightSample < heightIndex + 1.f; heightSample += sampleStride) {
        const float heightDirection = (1 - 2 * (heightSample + normalDistrib(randomGenerator)) / imgHeight) * scaleFactor;

        for (float widthSample = widthIndex; widthSample < widthIndex + 1.f; widthSample += sampleStride) {
          const float widthDirection = (2 * (widthSample + normalDistrib(randomGenerator)) / imgWidth - 1) * aspectRatio;

          const Vec3f screenSpaceDirection({ widthDirection, heightDirection, -1.f });
          const Vec3f rayDirection = screenSpaceDirection.normalize();

          RayHit hit {};

          if (m_bvh->recoverHitShape(m_shapes, m_camera->getPosition(), rayDirection, hit)) {
            Vec3f hitColor = hit.material->getDiffuseColor();

            if (m_params.ambientOcclusion)
              hitColor *= computeAmbientOcclusion(hit);

            if (!m_lights.empty())
              hitColor *= computeLighting(hit);
            else
              hitColor *= 1 - (rayDirection.dot(hit.normal) / 2 + 0.5f); // Simulate lighting coming from camera

            sumColor += hitColor;
          }
        }
      }

      const Vec3b finalColor((sumColor * 255) / (m_params.multiSamplingSamples * m_params.multiSamplingSamples));
      const std::size_t finalIndex = (finalHeightIndex + widthIndex) * 3;
      img->getData()[finalIndex]     = finalColor[0];
      img->getData()[finalIndex + 1] = finalColor[1];
      img->getData()[finalIndex + 2] = finalColor[2];
    }
  }

  return img;
}
