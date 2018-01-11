#include <iostream>

#include "RaZtracer/Math/Matrix.hpp"
#include "RaZtracer/Render/Scene.hpp"

const unsigned int RENDER_WIDTH = 1920, RENDER_HEIGHT = 1080;

void Scene::render(const std::string& fileName) {
  Matrix<float, RENDER_WIDTH, RENDER_HEIGHT> img;

  for (std::size_t heightIndex = 0; heightIndex < RENDER_HEIGHT; ++heightIndex) {
    for (std::size_t widthIndex = 0; widthIndex < RENDER_HEIGHT; ++widthIndex) {
      for (auto elt = elements.cbegin(); elt != elements.cend(); ++elt) {
        //if (elt->intersect())
          // Draw pixel
      }
    }
  }
}
