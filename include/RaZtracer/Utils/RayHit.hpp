#pragma once

#ifndef RAZTRACER_RAYHIT_HPP
#define RAZTRACER_RAYHIT_HPP

#include "../Render/Material.hpp"

struct RayHit {
  Vec3f position;
  Vec3f normal;
  Vec2f texcoords;
  MaterialPtr material;
  float distance = std::numeric_limits<float>::infinity();
};

#endif // RAZTRACER_RAYHIT_HPP
