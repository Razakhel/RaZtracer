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
  Sphere() : Shape(Vec3f({ 0.f, 0.f, 0.f }), Vec3f({ 0.5f, 0.5f, 0.5f })), radius{ 1.f }, transparency{ 1.f } {}
  Sphere(const Vec3f& origin, float radius, const Vec3f& color, float transparency)
    : Shape(origin, color), radius{ radius }, transparency{ transparency } {}

private:
  float radius;
  float transparency;
};

class Cube : public Shape {
public:
  Cube() : Shape(Vec3f({ 0.f, 0.f, 0.f }), Vec3f({ 0.5f, 0.5f, 0.5f })), size{ 1.f }, transparency{ 1.f } {}
  Cube(const Vec3f& origin, float size, const Vec3f& color, float transparency)
    : Shape(origin, color), size{ size }, transparency{ transparency } {}

private:
  float size;
  float transparency;
};

#endif // RAZTRACER_SHAPE_HPP
