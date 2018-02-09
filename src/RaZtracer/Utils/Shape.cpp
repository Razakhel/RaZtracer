#include <limits>

#include "RaZtracer/Utils/Shape.hpp"

namespace {

bool solveQuadratic(float a, float b, float c, float& firstHitDist, float& secondHitDist) {
  const float discriminant = b * b - 4 * a * c;

  if (discriminant < 0) {
    return false;
  } else if (discriminant == 0) {
    firstHitDist = secondHitDist = -0.5f * b / a;
  } else {
    const float q = -0.5f * ((b > 0) ? b + std::sqrt(discriminant) : b - std::sqrt(discriminant));

    firstHitDist = q / a;
    secondHitDist = c / q;
  }

  if (firstHitDist > secondHitDist)
    std::swap(firstHitDist, secondHitDist);

  return true;
}

} // namespace

bool Sphere::intersect(const Vec3f& rayOrigin, const Vec3f& rayDirection, RayHit* hit) const {
  const Vec3f sphereDistance = rayOrigin - m_center;

  const float rayLength = rayDirection.dot(rayDirection);
  const float rayDiff = 2 * rayDirection.dot(sphereDistance);
  const float sphereDiff = sphereDistance.dot(sphereDistance) - m_radius * m_radius;

  float firstHitDist {}, secondHitDist {};

  if (!solveQuadratic(rayLength, rayDiff, sphereDiff, firstHitDist, secondHitDist))
    return false;

  if (firstHitDist < 0) {
    firstHitDist = secondHitDist;

    if (firstHitDist < 0)
      return false;
  }

  if (hit) {
    hit->position = rayOrigin + rayDirection * firstHitDist;
    hit->material = m_material;
    hit->distance = firstHitDist;
  }

  return true;
}

bool Box::intersect(const Vec3f& rayOrigin, const Vec3f& rayDirection, RayHit* hit) const {


  return false;
}

Vec3f Triangle::computeNormal() const {
  const Vec3f firstEdge = m_secondPosition - m_firstPosition;
  const Vec3f secondEdge = m_thirdPosition - m_firstPosition;
  const Vec3f normal = firstEdge.cross(secondEdge);

  return normal.normalize();
}

bool Triangle::intersect(const Vec3f& rayOrigin, const Vec3f& rayDirection, RayHit* hit) const {
  const Vec3f normal = computeNormal();
  const float incidentAngle = rayDirection.dot(normal);

  if (std::abs(incidentAngle) < std::numeric_limits<float>::epsilon())
    return false;

  const float planeDist = m_firstPosition.dot(rayOrigin);
  const float hitDistance = -(rayOrigin.dot(normal) + planeDist) / incidentAngle;

  if (hitDistance < 0)
    return false;

  const Vec3f hitPosition = rayOrigin + rayDirection * hitDistance;

  const Vec3f firstEdge = m_secondPosition - m_firstPosition;
  const Vec3f firstHitDir = hitPosition - m_firstPosition;

  if ((firstEdge.cross(firstHitDir)).dot(normal) < 0)
    return false;

  const Vec3f secondEdge = m_thirdPosition - m_secondPosition;
  const Vec3f secondHitDir = hitPosition - m_secondPosition;

  if ((secondEdge.cross(secondHitDir)).dot(normal) < 0)
    return false;

  const Vec3f thirdEdge = m_firstPosition - m_thirdPosition;
  const Vec3f thirdHitDir = hitPosition - m_thirdPosition;

  if ((thirdEdge.cross(thirdHitDir)).dot(normal) < 0)
    return false;

  if (hit) {
    hit->position = hitPosition;
    hit->normal = normal;
    hit->material = m_material;
    hit->distance = hitDistance;
  }

  return true;
}
