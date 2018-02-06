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

bool Sphere::intersect(const Vec3f& rayOrigin, const Vec3f& rayDirection) const {
  const Vec3f sphereDirection = rayOrigin - m_center;

  const float rayLength = rayDirection.dot(rayDirection);
  const float rayDiff = 2 * rayDirection.dot(sphereDirection);
  const float sphereDist = sphereDirection.dot(sphereDirection) - m_radius * m_radius;

  float firstHitDist, secondHitDist;

  return solveQuadratic(rayLength, rayDiff, sphereDist, firstHitDist, secondHitDist);
}

bool Cube::intersect(const Vec3f& rayOrigin, const Vec3f& rayDirection) const {


  return false;
}

bool Triangle::intersect(const Vec3f& rayOrigin, const Vec3f& rayDirection/*, float maxDepth*/) const {


  return false;
}
