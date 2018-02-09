#pragma once

#ifndef RAZTRACER_MATERIAL_HPP
#define RAZTRACER_MATERIAL_HPP

#include "../Math/Vector.hpp"

struct Material {
  Vec3f color;
  float transparency;
};

#endif // RAZTRACER_MATERIAL_HPP
