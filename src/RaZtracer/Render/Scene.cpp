#include <limits>

#include "RaZtracer/Math/Matrix.hpp"
#include "RaZtracer/Math/Quaternion.hpp"
#include "RaZtracer/Render/Scene.hpp"
#include "RaZtracer/Utils/RayHit.hpp"

namespace {
  void buildOrthoBase(const Vec3f& yAxis, Vec3f& xAxis, Vec3f& zAxis) {
    const float sign = std::copysign(1.f, yAxis[2]);
    const float a = -1.f / (sign + yAxis[2]);
    const float b = yAxis[0] * yAxis[1] * a;

    xAxis = Vec3f({ 1.f + sign * yAxis[0] * yAxis[0] * a, sign * b, -sign * yAxis[0] });
    zAxis = Vec3f({ b, sign + yAxis[1] * yAxis[1] * a, -yAxis[1] });
  }
}

const float PHI = (std::sqrt(5.f) + 1) / 2;

void Scene::enableAmbientOcclusion(bool enabled, uint16_t raySamples) {
  m_params.ambientOcclusion = enabled;
  m_params.ambOccRaySamples = raySamples;
}

ImagePtr Scene::render() {
  const unsigned int imgWidth = m_camera->getFrameWidth();
  const unsigned int imgHeight = m_camera->getFrameHeight();
  const float scaleFactor = std::tan(m_camera->getFieldOfView() / 2);

  ImagePtr img = std::make_unique<Image>(imgWidth, imgHeight);

  #pragma omp parallel for schedule(dynamic, 1)
  for (std::size_t heightIndex = 0; heightIndex < imgHeight; ++heightIndex) {
    const std::size_t finalHeightIndex = heightIndex * imgWidth;

    for (std::size_t widthIndex = 0; widthIndex < imgWidth; ++widthIndex) {
      const Vec3f screenSpaceDirection({ (2 * (widthIndex + 0.5f) / imgWidth - 1) * m_camera->getAspectRatio() * scaleFactor,
                                         (1 - 2 * (heightIndex + 0.5f) / imgHeight) * scaleFactor,
                                         -1.f });
      const Vec3f rayDirection = screenSpaceDirection.normalize();

      RayHit hit {};
      RayHit closestHit {};
      Vec3b finalColor({ 50, 75, 75 });

      closestHit.distance = std::numeric_limits<float>::infinity();

      for (const auto& shape : m_shapes) {
        if (shape->intersect(m_camera->getPosition(), rayDirection, &hit) && hit.distance < closestHit.distance)
          closestHit = hit;
      }

      if (closestHit.distance < std::numeric_limits<float>::infinity()) {
        Vec3f hitColor = closestHit.material->getDiffuseColor();

        if (m_params.ambientOcclusion)
          hitColor *= computeAmbientOcclusion(closestHit);
        else if (!m_lights.empty())
          hitColor *= computeLighting(closestHit);
        else
          hitColor *= 1 - (rayDirection.dot(closestHit.normal) / 2 + 0.5f); // Simulate lighting coming from camera

        finalColor = Vec3b(hitColor * 255);
      }

      const std::size_t finalIndex = (finalHeightIndex + widthIndex) * 3;
      img->getData()[finalIndex]     = finalColor[0];
      img->getData()[finalIndex + 1] = finalColor[1];
      img->getData()[finalIndex + 2] = finalColor[2];
    }
  }

  return img;
}

float Scene::computeLighting(const RayHit& hit) {
  uint16_t hitCount = 0;

  for (const auto& light : m_lights) {
    const Vec3f lightDir = (light->getPosition() - hit.position).normalize();

    for (const auto& shapeObstacle : m_shapes) {
      if (shapeObstacle->intersect(hit.position + hit.normal * 0.0001f, lightDir)) {
        ++hitCount;
        break;
      }
    }
  }

  return (1 - (static_cast<float>(hitCount) / m_lights.size()));
}

float Scene::computeAmbientOcclusion(const RayHit& hit) {
  float hitCount = 0;

  std::mt19937 randomGenerator(m_randomDevice());
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

    for (const auto& shape : m_shapes) {
      if (shape->intersect(hit.position + hit.normal * 0.0001f, worldDirection)) {
        hitCount += weight;
        break;
      }
    }
  }

  return (1 - (hitCount / m_params.ambOccRaySamples));
}
