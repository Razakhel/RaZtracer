#pragma once

#ifndef RAZTRACER_SHAPE_HPP
#define RAZTRACER_SHAPE_HPP

#include <memory>

#include "../Math/Vector.hpp"
#include "../Render/Material.hpp"
#include "../Utils/RayHit.hpp"

class Shape {
public:
  Shape(const Vec3f& color, float transparency) : m_material{ color, transparency } {}

  virtual bool intersect(const Vec3f& rayOrigin, const Vec3f& rayDirection, RayHit* hit = nullptr) const = 0;

protected:
  Material m_material;
};

using ShapePtr = std::unique_ptr<Shape>;

class Sphere : public Shape {
public:
  Sphere() : Shape(Vec3f({ 0.5f, 0.f, 0.5f }), 1.f), m_center{ Vec3f(0.f) }, m_radius{ 1.f } {}
  Sphere(const Vec3f& center, float radius = 1.f, const Vec3f& color = Vec3f({ 0.5f, 0.f, 0.5f }), float transparency = 1.f)
    : Shape(color, transparency), m_center{ center }, m_radius{ radius } {}

  Vec3f computeNormal(const Vec3f& hitPosition) const { return (m_center - hitPosition).normalize(); }
  bool intersect(const Vec3f& rayOrigin, const Vec3f& rayDirection, RayHit* hit) const;

private:
  Vec3f m_center;
  float m_radius;
};

class Box : public Shape {
public:
  Box() : Shape(Vec3f({ 0.5f, 0.5f, 0.f }), 1.f), m_topRightPosition{ Vec3f(1.f) }, m_bottomLeftPosition{ Vec3f(-1.f) } {}
  Box(const Vec3f& topRightPosition, const Vec3f& bottomLeftPosition,
      const Vec3f& color = Vec3f({ 0.5f, 0.5f, 0.f }), float transparency = 1.f)
    : Shape(color, transparency), m_topRightPosition{ topRightPosition }, m_bottomLeftPosition{ bottomLeftPosition } {}

  bool intersect(const Vec3f& rayOrigin, const Vec3f& rayDirection, RayHit* hit) const;

private:
  Vec3f m_topRightPosition;
  Vec3f m_bottomLeftPosition;
};

class Triangle : public Shape {
public:
  Triangle() : Shape(Vec3f({ 0.f, 0.5f, 0.5f }), 1.f), m_firstPosition{ Vec3f({ -0.5f, -0.5f, 0.f }) },
                                                       m_secondPosition{ Vec3f({ 0.f, 0.5f, 0.f }) },
                                                       m_thirdPosition{ Vec3f({ 0.5f, -0.5f, 0.f }) } {}
  Triangle(const Vec3f& firstPosition, const Vec3f& secondPosition, const Vec3f& thirdPosition,
           const Vec3f& color = Vec3f({ 0.f, 0.5f, 0.5f }), float transparency = 1.f)
    : Shape(color, transparency), m_firstPosition{ firstPosition },
                                  m_secondPosition{ secondPosition },
                                  m_thirdPosition{ thirdPosition } {}

  Vec3f computeNormal() const;
  bool intersect(const Vec3f& rayOrigin, const Vec3f& rayDirection, RayHit* hit) const;

private:
  Vec3f m_firstPosition;
  Vec3f m_secondPosition;
  Vec3f m_thirdPosition;
};

#endif // RAZTRACER_SHAPE_HPP
