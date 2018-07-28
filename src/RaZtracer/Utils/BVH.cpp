#include <algorithm>
#include <iostream>

#include "RaZtracer/Utils/BVH.hpp"

namespace {

enum CutAxis { AXIS_X = 0,
               AXIS_Y,
               AXIS_Z };

}

void BVHNode::build(std::vector<DrawableShapePtr>& shapes, std::size_t beginIndex, std::size_t endIndex) {
  if (endIndex - beginIndex <= 1) {
    shapeIndex = beginIndex;
    boundingBox = shapes[beginIndex]->computeBoundingBox();
    return;
  }

  auto currentBox = shapes[beginIndex]->computeBoundingBox();

  for (std::size_t i = beginIndex + 1; i < endIndex; ++i) {
    const auto shapeBox = shapes[i]->computeBoundingBox();

    const float xMax = std::max(shapeBox->getTopRightPosition()[0], currentBox->getTopRightPosition()[0]);
    const float yMax = std::max(shapeBox->getTopRightPosition()[1], currentBox->getTopRightPosition()[1]);
    const float zMax = std::max(shapeBox->getTopRightPosition()[2], currentBox->getTopRightPosition()[2]);

    const float xMin = std::min(shapeBox->getBottomLeftPosition()[0], currentBox->getBottomLeftPosition()[0]);
    const float yMin = std::min(shapeBox->getBottomLeftPosition()[1], currentBox->getBottomLeftPosition()[1]);
    const float zMin = std::min(shapeBox->getBottomLeftPosition()[2], currentBox->getBottomLeftPosition()[2]);

    currentBox = std::make_unique<AABB>(Vec3f({ xMax, yMax, zMax }), Vec3f({ xMin, yMin, zMin }));
  }

  boundingBox = std::move(currentBox);

  float maxLength = boundingBox->getTopRightPosition()[0] - boundingBox->getBottomLeftPosition()[0];
  CutAxis axis = AXIS_X;

  const float maxYLength = boundingBox->getTopRightPosition()[1] - boundingBox->getBottomLeftPosition()[1];
  if (maxYLength > maxLength) {
    axis = AXIS_Y;
    maxLength = maxYLength;
  }

  const float maxZLength = boundingBox->getTopRightPosition()[2] - boundingBox->getBottomLeftPosition()[2];
  if (maxZLength > maxLength) {
    axis = AXIS_Z;
    maxLength = maxZLength;
  }

  const float halfCutPos = boundingBox->getBottomLeftPosition()[axis] + (maxLength / 2.f);
  const auto midShape = std::partition(shapes.data() + beginIndex, shapes.data() + endIndex, [axis, halfCutPos] (const auto& shape) {
    return shape->computeCentroid()[axis] < halfCutPos;
  });

  auto midIndex = static_cast<std::size_t>(std::distance(shapes.data(), midShape));
  if (midIndex == beginIndex || midIndex == endIndex)
    midIndex = static_cast<std::size_t>((beginIndex + endIndex) / 2.f);

  leftNode = std::make_unique<BVHNode>();
  leftNode->build(shapes, beginIndex, midIndex);

  rightNode = std::make_unique<BVHNode>();
  rightNode->build(shapes, midIndex, endIndex);
}

void BVH::build(std::vector<DrawableShapePtr>& shapes) {
  m_root = std::make_unique<BVHNode>();

  Vec3f globalTopRightPos {};
  Vec3f globalBottomLeftPos {};

  for (const auto& shape : shapes) {
    const auto shapeBox = shape->computeBoundingBox();

    const Vec3f& topRightPos = shapeBox->getTopRightPosition();
    const Vec3f& bottomLeftPos = shapeBox->getBottomLeftPosition();

    globalTopRightPos = { std::max(globalTopRightPos[0], topRightPos[0]),
                          std::max(globalTopRightPos[1], topRightPos[1]),
                          std::max(globalTopRightPos[2], topRightPos[2]) };
    globalBottomLeftPos = { std::max(globalBottomLeftPos[0], bottomLeftPos[0]),
                            std::max(globalBottomLeftPos[1], bottomLeftPos[1]),
                            std::max(globalBottomLeftPos[2], bottomLeftPos[2]) };
  }

  m_root->boundingBox = std::make_unique<AABB>(globalTopRightPos, globalBottomLeftPos);
  m_root->build(shapes, 0, shapes.size());
}

bool BVHNode::recoverHitShape(const std::vector<DrawableShapePtr>& shapes,
                              const Vec3f& rayOrigin,
                              const Vec3f& rayDirection,
                              RayHit& closestHit) const {
  if (!leftNode) {
    RayHit hit {};
    const bool hasHitShape = shapes[shapeIndex]->intersect(rayOrigin, rayDirection, &hit);

    if (hasHitShape && hit.distance < closestHit.distance)
      closestHit = hit;

    return hasHitShape;
  }

  RayHit leftHit {}, rightHit {};
  const bool hasHitLeftBox = leftNode->boundingBox->intersect(rayOrigin, rayDirection, &leftHit);
  const bool hasHitRightBox = rightNode->boundingBox->intersect(rayOrigin, rayDirection, &rightHit);

  if (hasHitLeftBox && hasHitRightBox) {
    bool hasHitLeftShape = false;
    bool hasHitRightShape = false;

    if (leftHit.distance < rightHit.distance) {
      hasHitLeftShape = leftNode->recoverHitShape(shapes, rayOrigin, rayDirection, closestHit);

      if (closestHit.distance > rightHit.distance)
        hasHitRightShape = rightNode->recoverHitShape(shapes, rayOrigin, rayDirection, closestHit);

      return hasHitLeftShape || hasHitRightShape;
    } else {
      hasHitRightShape = rightNode->recoverHitShape(shapes, rayOrigin, rayDirection, closestHit);

      if (closestHit.distance > leftHit.distance)
        hasHitLeftShape = leftNode->recoverHitShape(shapes, rayOrigin, rayDirection, closestHit);

      return hasHitRightShape || hasHitLeftShape;
    }
  } else if (hasHitLeftBox) {
    return leftNode->recoverHitShape(shapes, rayOrigin, rayDirection, closestHit);
  } else if (hasHitRightBox) {
    return rightNode->recoverHitShape(shapes, rayOrigin, rayDirection, closestHit);
  }

  return false;
}

bool BVH::recoverHitShape(const std::vector<DrawableShapePtr>& shapes,
                          const Vec3f& rayOrigin,
                          const Vec3f& rayDirection,
                          RayHit& closestHit) const {
  RayHit boxHit {};

  if (m_root->boundingBox->intersect(rayOrigin, rayDirection, &boxHit)) {
    if (boxHit.distance >= 0.f)
      return m_root->recoverHitShape(shapes, rayOrigin, rayDirection, closestHit);
  }

  return false;
}
