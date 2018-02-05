#include <iostream>

#include "RaZtracer/Math/Matrix.hpp"
#include "RaZtracer/Render/Scene.hpp"
#include "RaZtracer/Utils/Image.hpp"

void Scene::render(const std::string& fileName) {
  const Vec4f cameraPos(m_camera->getPosition(), 1.f);
  const Vec3f rayOrigin = m_camera->computePerspectiveMatrix().inverse() * m_camera->lookAt().inverse() * cameraPos;

  const unsigned int imgWidth = m_camera->getFrameWidth();
  const unsigned int imgHeight = m_camera->getFrameHeight();
  const float scaleFactor = std::tan(m_camera->getFieldOfView() / 2);

  Image img(imgWidth, imgHeight);

  for (std::size_t heightIndex = 0; heightIndex < imgHeight; ++heightIndex) {
    for (std::size_t widthIndex = 0; widthIndex < imgWidth; ++widthIndex) {
      const Vec4f screenSpaceDirection({ (2 * ((widthIndex + 0.5f) / imgWidth) - 1) * m_camera->getAspectRatio() * scaleFactor,
                                         (1 - 2 * ((heightIndex + 0.5f) / imgHeight)) * scaleFactor,
                                         -1.f,
                                          1.f });
      const Vec4f worldSpaceDirection = m_camera->computePerspectiveMatrix().inverse()
                                        * m_camera->lookAt().inverse()
                                        * screenSpaceDirection;

      const Vec3f rayDirection = (worldSpaceDirection / worldSpaceDirection[3]).normalize();

      for (const auto& shape : m_shapes) {
        if (shape->intersect(rayOrigin, rayDirection))
          img.getData()[heightIndex * imgWidth + widthIndex] = 255;
      }
    }
  }

  img.write(fileName);
}
