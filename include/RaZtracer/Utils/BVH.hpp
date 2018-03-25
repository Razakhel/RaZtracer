#pragma once

#ifndef RAZTRACER_BVH_HPP
#define RAZTRACER_BVH_HPP

#include <memory>
#include <vector>

#include "../Math/Vector.hpp"
#include "../Utils/Shape.hpp"

struct BVHNode {
  void build(std::vector<DrawableShapePtr>& shapes, std::size_t beginIndex, std::size_t endIndex);
  bool recoverHitShape(const std::vector<DrawableShapePtr>& shapes,
                       const Vec3f& rayOrigin,
                       const Vec3f& rayDirection,
                       RayHit& closestHit) const;

  std::unique_ptr<BVHNode> leftNode {};
  std::unique_ptr<BVHNode> rightNode {};
  std::unique_ptr<AABB> boundingBox {};
  std::size_t shapeIndex {};
};

class BVH {
public:
  explicit BVH(std::vector<DrawableShapePtr>& shapes) { build(shapes); }

  void build(std::vector<DrawableShapePtr>& shapes);
  bool recoverHitShape(const std::vector<DrawableShapePtr>& shapes,
                       const Vec3f& rayOrigin,
                       const Vec3f& rayDirection,
                       RayHit& closestHit) const;

private:
  std::unique_ptr<BVHNode> m_root {};
};

#endif // RAZTRACER_BVH_HPP
