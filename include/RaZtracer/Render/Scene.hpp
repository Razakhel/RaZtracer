#pragma once

#ifndef RAZTRACER_SCENE_HPP
#define RAZTRACER_SCENE_HPP

#include <string>
#include <vector>

#include "../Utils/Shape.hpp"

class Scene {
public:
  Scene() {}

  void addShape(const Shape& shape) { m_shapes.emplace_back(shape); }
  void render(const std::string& fileName);

private:
  std::vector<Shape> m_shapes;
};

#endif // RAZTRACER_SCENE_HPP
