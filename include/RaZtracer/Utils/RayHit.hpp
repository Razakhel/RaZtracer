#pragma once

#ifndef RAZTRACER_RAYHIT_HPP
#define RAZTRACER_RAYHIT_HPP

#include "RaZtracer/Math/Vector.hpp"

struct RayHit {
  Vec3f position;
  Vec3f color;
  float distance;
};

#endif // RAZTRACER_RAYHIT_HPP
