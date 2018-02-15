#pragma once

#ifndef RAZTRACER_SCENE_HPP
#define RAZTRACER_SCENE_HPP

#include <memory>
#include <string>
#include <vector>

#include "../Render/Camera.hpp"
#include "../Render/Light.hpp"
#include "../Utils/Image.hpp"
#include "../Utils/ModelLoader.hpp"
#include "../Utils/Shape.hpp"

class Scene {
public:
  Scene() = default;

  const std::vector<ShapePtr>& getShapes() const { return m_shapes; }

  void setCamera(CameraPtr camera) { m_camera = std::move(camera); }

  void addShape(ShapePtr shape) { m_shapes.emplace_back(std::move(shape)); }
  void addLight(LightPtr light) { m_lights.emplace_back(std::move(light)); }
  void addModel(const std::string& fileName) { ModelLoader::importModel(fileName, m_shapes); }

  ImagePtr render();

private:
  CameraPtr m_camera;
  std::vector<ShapePtr> m_shapes;
  std::vector<LightPtr> m_lights;
};

#endif // RAZTRACER_SCENE_HPP
