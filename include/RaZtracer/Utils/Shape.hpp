#pragma once

#ifndef RAZTRACER_SHAPE_HPP
#define RAZTRACER_SHAPE_HPP

#include "../Math/Vector.hpp"

class Shape {
public:
  Shape(const Vec3f& origin, const Vec3f& color) : origin{ origin }, color{ color } {}

  bool intersect(const Vec3f& rayOrig, const Vec3f& rayDirection);

protected:
  Vec3f origin;
  Vec3f color;
};

class Sphere : public Shape {
public:
  Sphere() : Shape(Vec3f({ 0.f, 0.f, 0.f }), Vec3f({ 0.5f, 0.5f, 0.5f })), m_radius{ 1.f }, m_transparency{ 1.f } {}
  Sphere(const Vec3f& origin, float radius, const Vec3f& color, float transparency)
    : Shape(origin, color), m_radius{ radius }, m_transparency{ transparency } {}

private:
  float m_radius;
  float m_transparency;
};

class Cube : public Shape {
public:
  Cube() : Shape(Vec3f({ 0.f, 0.f, 0.f }), Vec3f({ 0.5f, 0.5f, 0.5f })), m_size{ 1.f }, m_transparency{ 1.f } {}
  Cube(const Vec3f& origin, float size, const Vec3f& color, float transparency)
    : Shape(origin, color), m_size{ size }, m_transparency{ transparency } {}

private:
  float m_size;
  float m_transparency;
};

#endif // RAZTRACER_SHAPE_HPP
