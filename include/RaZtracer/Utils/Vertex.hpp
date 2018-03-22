#pragma once

#ifndef RAZTRACER_VERTEX_HPP
#define RAZTRACER_VERTEX_HPP

#include "../Math/Vector.hpp"

struct Vertex {
  Vertex() = default;
  explicit Vertex(const Vec3f& vertPosition) : position{ vertPosition } {}
  Vertex(const Vec3f& vertPosition, const Vec3f& vertNormal) : position{ vertPosition }, normal{ vertNormal } {}

  Vec3f position {};
  Vec3f normal {};
  Vec2f texcoords {};
};

#endif // RAZTRACER_VERTEX_HPP
