#pragma once

#ifndef RAZTRACER_SCENE_HPP
#define RAZTRACER_SCENE_HPP

#include <memory>
#include <string>
#include <vector>

#include "../Render/Camera.hpp"
#include "../Utils/Shape.hpp"

class Scene {
public:
  Scene() = default;

  void setCamera(std::unique_ptr<Camera> camera) { m_camera = std::move(camera); }
  void addShape(std::unique_ptr<Shape> shape) { m_shapes.emplace_back(std::move(shape)); }
  void render(const std::string& fileName);

private:
  std::unique_ptr<Camera> m_camera;
  std::vector<std::unique_ptr<Shape>> m_shapes;
};

#endif // RAZTRACER_SCENE_HPP
