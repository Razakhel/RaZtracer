#pragma once

#ifndef RAZTRACER_SCENE_HPP
#define RAZTRACER_SCENE_HPP

#include <memory>
#include <random>
#include <string>
#include <vector>

#include "../Render/Camera.hpp"
#include "../Render/Light.hpp"
#include "../Utils/BVH.hpp"
#include "../Utils/Image.hpp"
#include "../Utils/ModelLoader.hpp"
#include "../Utils/Shape.hpp"

class Scene {
public:
  const std::vector<DrawableShapePtr>& getShapes() const { return m_shapes; }

  void setCamera(CameraPtr camera) { m_camera = std::move(camera); }

  void addShape(DrawableShapePtr shape) { m_shapes.emplace_back(std::move(shape)); }
  void addLight(LightPtr light) { m_lights.emplace_back(std::move(light)); }
  void addModel(const std::string& fileName) { ModelLoader::importModel(fileName, m_shapes); }

  void enableAmbientOcclusion(bool enabled, uint16_t raySamples = 64);
  void enableMultiSampling(uint16_t samples = 4) { m_params.multiSamplingSamples = std::max(static_cast<uint16_t>(1), samples); }
  void buildBVH() { m_bvh = std::make_unique<BVH>(m_shapes); }
  ImagePtr render() const;

private:
  struct SceneParams {
    bool ambientOcclusion = false;
    uint16_t ambOccRaySamples = 0;

    uint16_t multiSamplingSamples = 1;
  };

  float computeLighting(const RayHit& hit) const ;
  float computeAmbientOcclusion(const RayHit& hit) const;
  ImagePtr renderStandard() const;
  ImagePtr renderBVH() const;

  SceneParams m_params {};
  CameraPtr m_camera {};
  std::vector<DrawableShapePtr> m_shapes {};
  std::vector<LightPtr> m_lights {};
  std::unique_ptr<BVH> m_bvh {};
};

#endif // RAZTRACER_SCENE_HPP
