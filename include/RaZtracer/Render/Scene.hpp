#pragma once

#ifndef RAZTRACER_SCENE_HPP
#define RAZTRACER_SCENE_HPP

#include <memory>
#include <string>
#include <vector>

#include "../Render/Camera.hpp"
#include "../Utils/Image.hpp"
#include "../Utils/Shape.hpp"

class Scene {
public:
  Scene() = default;

  const std::vector<ShapePtr>& getShapes() const { return m_shapes; }

  void setCamera(CameraPtr camera) { m_camera = std::move(camera); }

  void addShape(ShapePtr shape) { m_shapes.emplace_back(std::move(shape)); }
  ImagePtr render(const std::string& fileName);

private:
  CameraPtr m_camera;
  std::vector<ShapePtr> m_shapes;
};

#endif // RAZTRACER_SCENE_HPP
