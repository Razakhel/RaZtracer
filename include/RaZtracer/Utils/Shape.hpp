#pragma once

#ifndef RAZTRACER_SHAPE_HPP
#define RAZTRACER_SHAPE_HPP

#include <memory>

#include "../Math/Vector.hpp"
#include "../Render/Material.hpp"
#include "../Utils/RayHit.hpp"
#include "../Utils/Vertex.hpp"

class Drawable {
public:
  Drawable(const Vec3f& color, float transparency);

  void setMaterial(MaterialPtr material) { m_material = material; }

protected:
  MaterialPtr m_material {};
};

class AABB;

class Shape {
public:
  virtual std::unique_ptr<AABB> computeBoundingBox() const = 0;
  virtual Vec3f computeCentroid() const = 0;
  virtual bool intersect(const Vec3f& rayOrigin, const Vec3f& rayDirection, RayHit* hit = nullptr) const = 0;
};

class DrawableShape : public Drawable, public Shape {
public:
  DrawableShape(const Vec3f& color, float transparency) : Drawable(color, transparency) {}
};

using ShapePtr = std::unique_ptr<Shape>;
using DrawableShapePtr = std::unique_ptr<DrawableShape>;

class Sphere : public DrawableShape {
public:
  Sphere() : DrawableShape(Vec3f({ 0.5f, 0.f, 0.5f }), 1.f), m_center{ Vec3f(0.f) }, m_radius{ 1.f } {}
  explicit Sphere(const Vec3f& center, float radius = 1.f, const Vec3f& color = Vec3f({ 0.5f, 0.f, 0.5f }), float transparency = 1.f)
    : DrawableShape(color, transparency), m_center{ center }, m_radius{ radius } {}

  std::unique_ptr<AABB> computeBoundingBox() const { return std::make_unique<AABB>(m_center + m_radius, m_center - m_radius); }
  Vec3f computeCentroid() const { return m_center; }
  Vec3f computeNormal(const Vec3f& hitPosition) const { return (hitPosition - m_center).normalize(); }
  bool intersect(const Vec3f& rayOrigin, const Vec3f& rayDirection, RayHit* hit) const;

private:
  Vec3f m_center {};
  float m_radius {};
};

class AABB : public Shape {
public:
  AABB(const Vec3f& topRightPosition, const Vec3f& bottomLeftPosition)
    : m_topRightPosition{ topRightPosition }, m_bottomLeftPosition{ bottomLeftPosition } {}

  const Vec3f& getTopRightPosition() const { return m_topRightPosition; }
  const Vec3f& getBottomLeftPosition() const { return m_bottomLeftPosition; }

  std::unique_ptr<AABB> computeBoundingBox() const { return std::make_unique<AABB>(m_topRightPosition, m_bottomLeftPosition); }
  Vec3f computeCentroid() const { return m_topRightPosition - m_bottomLeftPosition; }
  bool intersect(const Vec3f& rayOrigin, const Vec3f& rayDirection, RayHit* hit) const;

protected:
  Vec3f m_topRightPosition {};
  Vec3f m_bottomLeftPosition {};
};

class Box : public AABB, public Drawable {
public:
  Box() : AABB(Vec3f(1.f), Vec3f(-1.f)), Drawable(Vec3f({ 0.5f, 0.5f, 0.f }), 1.f) {}
  Box(const Vec3f& topRightPosition, const Vec3f& bottomLeftPosition,
      const Vec3f& color = Vec3f({ 0.5f, 0.5f, 0.f }), float transparency = 1.f)
    : AABB(topRightPosition, bottomLeftPosition), Drawable(color, transparency) {}

  bool intersect(const Vec3f& rayOrigin, const Vec3f& rayDirection, RayHit* hit) const;
};

class Triangle : public DrawableShape {
public:
  Triangle() : DrawableShape(Vec3f({ 0.f, 0.5f, 0.5f }), 1.f), m_firstVert(Vec3f({ -0.5f, -0.5f, 0.f })),
                                                               m_secondVert(Vec3f({ 0.f, 0.5f, 0.f })),
                                                               m_thirdVert(Vec3f({ 0.5f, -0.5f, 0.f })) {}
  Triangle(const Vec3f& firstPosition, const Vec3f& secondPosition, const Vec3f& thirdPosition,
           const Vec3f& color = Vec3f({ 0.f, 0.5f, 0.5f }), float transparency = 1.f)
    : DrawableShape(color, transparency), m_firstVert(firstPosition),
                                          m_secondVert(secondPosition),
                                          m_thirdVert(thirdPosition) { computeNormal(); }
  Triangle(const Vec3f& firstPosition, const Vec3f& secondPosition, const Vec3f& thirdPosition, const Vec3f& normal,
           const Vec3f& color = Vec3f({ 0.f, 0.5f, 0.5f }), float transparency = 1.f)
    : DrawableShape(color, transparency), m_firstVert(firstPosition, normal),
                                          m_secondVert(secondPosition, normal),
                                          m_thirdVert(thirdPosition, normal) {}

  std::unique_ptr<AABB> computeBoundingBox() const;
  Vec3f computeCentroid() const { return (m_firstVert.position + m_secondVert.position + m_thirdVert.position) / 3.f; }
  void computeNormal();
  bool intersect(const Vec3f& rayOrigin, const Vec3f& rayDirection, RayHit* hit) const;

private:
  Vertex m_firstVert {};
  Vertex m_secondVert {};
  Vertex m_thirdVert {};
};

#endif // RAZTRACER_SHAPE_HPP
