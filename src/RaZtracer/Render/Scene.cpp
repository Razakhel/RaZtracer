#include <iostream>

#include "RaZtracer/Math/Matrix.hpp"
#include "RaZtracer/Render/Scene.hpp"
#include "RaZtracer/Utils/Image.hpp"

void Scene::render(const std::string& fileName, std::size_t imgWidth, std::size_t imgHeight) {
  Image img(imgWidth, imgHeight);

  for (std::size_t heightIndex = 0; heightIndex < imgHeight; ++heightIndex) {
    for (std::size_t widthIndex = 0; widthIndex < imgWidth; ++widthIndex) {
      for (const auto& shape : m_shapes) {
        //if (elt->intersect())
          // Draw pixel
      }
    }
  }

}
