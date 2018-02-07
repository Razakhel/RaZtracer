#include <iostream>

#include "RaZtracer/Math/Matrix.hpp"
#include "RaZtracer/Render/Scene.hpp"
#include "RaZtracer/Utils/Image.hpp"
#include "RaZtracer/Utils/RayHit.hpp"

ImagePtr Scene::render() {
  const Vec4f cameraPos(m_camera->getPosition(), 1.f);
  const Mat4f invProjectionMat = m_camera->computePerspectiveMatrix().inverse();
  const Mat4f invViewMat = m_camera->lookAt().inverse();
  const Vec3f rayOrigin = invProjectionMat * invViewMat * cameraPos;

  const unsigned int imgWidth = m_camera->getFrameWidth();
  const unsigned int imgHeight = m_camera->getFrameHeight();
  const float scaleFactor = std::tan(m_camera->getFieldOfView() / 2);

  ImagePtr img = std::make_unique<Image>(imgWidth, imgHeight);

  for (std::size_t heightIndex = 0; heightIndex < imgHeight; ++heightIndex) {
    const std::size_t finalHeightIndex = heightIndex * imgWidth;

    for (std::size_t widthIndex = 0; widthIndex < imgWidth; ++widthIndex) {
      const std::size_t finalIndex = (finalHeightIndex + widthIndex) * 3;
      const Vec4f screenSpaceDirection({ (2 * ((widthIndex + 0.5f) / imgWidth) - 1) * m_camera->getAspectRatio() * scaleFactor,
                                         (1 - 2 * ((heightIndex + 0.5f) / imgHeight)) * scaleFactor,
                                         -1.f,
                                          1.f });
      const Vec4f worldSpaceDirection = invProjectionMat * invViewMat * screenSpaceDirection;
      const Vec3f rayDirection = (worldSpaceDirection / worldSpaceDirection[3]).normalize();
      RayHit hit {};

      for (const auto& shape : m_shapes) {
        if (shape->intersect(rayOrigin, rayDirection, hit)) {
          img->getData()[finalIndex]     = static_cast<uint8_t>(hit.color[0] * 255);
          img->getData()[finalIndex + 1] = static_cast<uint8_t>(hit.color[1] * 255);
          img->getData()[finalIndex + 2] = static_cast<uint8_t>(hit.color[2] * 255);
        }
      }
    }
  }

  return img;
}
