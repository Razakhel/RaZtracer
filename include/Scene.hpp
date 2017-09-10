#pragma once

#ifndef RAZTRACER_SCENE_HPP
#define RAZTRACER_SCENE_HPP

#include <vector>

#include "Shape.hpp"

class Scene {
public:
  Scene() {}

  void add(const Shape& elt) { elements.push_back(elt); }
  void render();

private:
  std::vector<Shape> elements;
};

#endif // RAZTRACER_SCENE_HPP
