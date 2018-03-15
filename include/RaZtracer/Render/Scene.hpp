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
  Scene() : m_randomGenerator{ m_randomDevice() } {}

  const std::vector<ShapePtr>& getShapes() const { return m_shapes; }

  void setCamera(CameraPtr camera) { m_camera = std::move(camera); }

  void addShape(ShapePtr shape) { m_shapes.emplace_back(std::move(shape)); }
  void addLight(LightPtr light) { m_lights.emplace_back(std::move(light)); }
  void addModel(const std::string& fileName) { ModelLoader::importModel(fileName, m_shapes); }

  void enableAmbientOcclusion(bool enabled, uint16_t raySamples = 64);
  void enableMultiSampling(uint16_t samples = 8) { m_params.multiSamplingSamples = std::max(static_cast<uint16_t>(1), samples); }
  ImagePtr render();

private:
  struct SceneParams {
    bool ambientOcclusion = false;
    uint16_t ambOccRaySamples = 0;

    uint16_t multiSamplingSamples = 1;
  };

  float computeLighting(const RayHit& hit);
  float computeAmbientOcclusion(const RayHit& hit);

  SceneParams m_params {};
  CameraPtr m_camera {};
  std::vector<ShapePtr> m_shapes {};
  std::vector<LightPtr> m_lights {};
  std::random_device m_randomDevice {};
  std::mt19937 m_randomGenerator {};
};

#endif // RAZTRACER_SCENE_HPP
