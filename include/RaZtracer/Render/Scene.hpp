#pragma once

#ifndef RAZTRACER_SCENE_HPP
#define RAZTRACER_SCENE_HPP

#include <string>
#include <vector>

#include "../Utils/Shape.hpp"

class Scene {
public:
  Scene() {}

  void add(const Shape& elt) { elements.push_back(elt); }
  void render(const std::string& fileName);

private:
  std::vector<Shape> elements;
};

#endif // RAZTRACER_SCENE_HPP
