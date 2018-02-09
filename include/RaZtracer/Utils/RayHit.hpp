#pragma once

#ifndef RAZTRACER_RAYHIT_HPP
#define RAZTRACER_RAYHIT_HPP

#include "../Render/Material.hpp"

struct RayHit {
  Vec3f position;
  Vec3f normal;
  Vec2f texcoords;
  Material material;
  float distance;
};

#endif // RAZTRACER_RAYHIT_HPP
