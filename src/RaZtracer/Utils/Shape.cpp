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

bool Sphere::intersect(const Vec3f& rayOrigin, const Vec3f& rayDirection, RayHit& hit) const {
  const Vec3f sphereDistance = rayOrigin - m_center;

  const float rayLength = rayDirection.dot(rayDirection);
  const float rayDiff = 2 * rayDirection.dot(sphereDistance);
  const float sphereDiff = sphereDistance.dot(sphereDistance) - m_radius * m_radius;

  float firstHitDist {}, secondHitDist {};

  if (!solveQuadratic(rayLength, rayDiff, sphereDiff, firstHitDist, secondHitDist))
    return false;

  if (firstHitDist > secondHitDist)
    std::swap(firstHitDist, secondHitDist);

  if (firstHitDist < 0) {
    firstHitDist = secondHitDist;

    if (firstHitDist < 0)
      return false;
  }

  hit.position = rayOrigin + rayDirection * firstHitDist;
  hit.color = m_color;
  hit.distance = firstHitDist;

  return true;
}

bool Cube::intersect(const Vec3f& rayOrigin, const Vec3f& rayDirection, RayHit& hit) const {


  return false;
}

bool Triangle::intersect(const Vec3f& rayOrigin, const Vec3f& rayDirection, RayHit& hit) const {


  return false;
}
