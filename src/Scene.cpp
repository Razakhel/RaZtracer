#include <iostream>

#include "Scene.hpp"
#include "Matrix.hpp"

void Scene::render() {
  Matrix<float, 1920, 1080> img;

  std::cout << img.getData().size() << std::endl;
}
