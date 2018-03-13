#pragma once

#ifndef RAZTRACER_SCENE_HPP
#define RAZTRACER_SCENE_HPP

#include <memory>
#include <random>
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

  void enableAmbientOcclusion(bool enabled, uint16_t raySamples = 64);
  ImagePtr render();

private:
  struct SceneParams {
    bool ambientOcclusion;
    uint16_t ambOccRaySamples;
  };

  float computeLighting(const RayHit& hit);
  float computeAmbientOcclusion(const RayHit& hit);

  SceneParams m_params {};
  CameraPtr m_camera {};
  std::vector<ShapePtr> m_shapes {};
  std::vector<LightPtr> m_lights {};
  std::random_device m_randomDevice {};
};

#endif // RAZTRACER_SCENE_HPP
