#pragma once

#ifndef RAZTRACER_SHAPE_HPP
#define RAZTRACER_SHAPE_HPP

#include "../Math/Vector.hpp"

class Shape {
public:
  Shape(const Vec3f& color, float transparency) : m_color{ color }, m_transparency{ transparency } {}

  virtual bool intersect(const Vec3f& rayOrigin, const Vec3f& rayDirection) const = 0;

protected:
  Vec3f m_color;
  float m_transparency;
};

class Sphere : public Shape {
public:
  Sphere() : Shape(Vec3f({ 0.5f, 0.f, 0.5f }), 1.f), m_center{ Vec3f(0.f) }, m_radius{ 1.f } {}
  Sphere(const Vec3f& center, float radius = 1.f, const Vec3f& color = Vec3f({ 0.5f, 0.f, 0.5f }), float transparency = 1.f)
    : Shape(color, transparency), m_center{ center }, m_radius{ radius } {}

  bool intersect(const Vec3f& rayOrigin, const Vec3f& rayDirection) const;

private:
  Vec3f m_center;
  float m_radius;
};

class Cube : public Shape {
public:
  Cube() : Shape(Vec3f({ 0.5f, 0.5f, 0.f }), 1.f), m_center{ Vec3f(0.f) }, m_size{ 1.f } {}
  Cube(const Vec3f& center, float size = 1.f, const Vec3f& color = Vec3f({ 0.5f, 0.5f, 0.f }), float transparency = 1.f)
    : Shape(color, transparency), m_center{ center }, m_size{ size } {}

  bool intersect(const Vec3f& rayOrigin, const Vec3f& rayDirection) const;

private:
  Vec3f m_center;
  float m_size;
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

  bool intersect(const Vec3f& rayOrigin, const Vec3f& rayDirection) const;

private:
  Vec3f m_firstPosition;
  Vec3f m_secondPosition;
  Vec3f m_thirdPosition;
};

#endif // RAZTRACER_SHAPE_HPP
