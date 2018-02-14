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

Shape::Shape(const Vec3f& color, float transparency) {
  m_material = std::make_unique<Material>();
  m_material->setDiffuseColor(color);
  m_material->setTransparency(transparency);
}

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
    hit->normal = computeNormal(hit->position);
    hit->material = m_material;
    hit->distance = firstHitDist;
  }

  return true;
}

bool Box::intersect(const Vec3f& rayOrigin, const Vec3f& rayDirection, RayHit* hit) const {
  float minWidthDist = (m_bottomLeftPosition[0] - rayOrigin[0]) / rayDirection[0];
  float maxWidthDist = (m_topRightPosition[0] - rayOrigin[0]) / rayDirection[0];

  if (minWidthDist > maxWidthDist)
    std::swap(minWidthDist, maxWidthDist);

  float minHeightDist = (m_bottomLeftPosition[1] - rayOrigin[1]) / rayDirection[1];
  float maxHeightDist = (m_topRightPosition[1] - rayOrigin[1]) / rayDirection[1];

  if (minHeightDist > maxHeightDist)
    std::swap(minHeightDist, maxHeightDist);

  if ((minWidthDist > maxHeightDist) || (minHeightDist > maxWidthDist))
    return false;

  if (minHeightDist > minWidthDist)
    minWidthDist = minHeightDist;

  if (maxHeightDist < maxWidthDist)
    maxWidthDist = maxHeightDist;

  float minDepthDist = (m_bottomLeftPosition[2] - rayOrigin[2]) / rayDirection[2];
  float maxDepthDist = (m_topRightPosition[2] - rayOrigin[2]) / rayDirection[2];

  if (minDepthDist > maxDepthDist)
    std::swap(minDepthDist, maxDepthDist);

  if ((minWidthDist > maxDepthDist) || (minDepthDist > maxWidthDist))
    return false;

  if (minDepthDist > minWidthDist)
    minWidthDist = minDepthDist;

  if (maxDepthDist < maxWidthDist)
    maxWidthDist = maxDepthDist;

  if (hit) {
    hit->position = rayOrigin + rayDirection * maxWidthDist;
    hit->material = m_material;
    hit->distance = maxWidthDist;
  }

  return true;
}

void Triangle::computeNormal() {
  const Vec3f firstEdge = m_secondPosition - m_firstPosition;
  const Vec3f secondEdge = m_thirdPosition - m_firstPosition;
  m_normal = (firstEdge.cross(secondEdge)).normalize();
}

bool Triangle::intersect(const Vec3f& rayOrigin, const Vec3f& rayDirection, RayHit* hit) const {
  const float incidentAngle = rayDirection.dot(m_normal);

  if (std::abs(incidentAngle) < std::numeric_limits<float>::epsilon())
    return false;

  const float planeDist = m_firstPosition.dot(rayOrigin);
  const float hitDistance = -(rayOrigin.dot(m_normal) + planeDist) / incidentAngle;

  if (hitDistance < 0)
    return false;

  const Vec3f hitPosition = rayOrigin + rayDirection * hitDistance;

  const Vec3f firstEdge = m_secondPosition - m_firstPosition;
  const Vec3f firstHitDir = hitPosition - m_firstPosition;

  if ((firstEdge.cross(firstHitDir)).dot(m_normal) < 0)
    return false;

  const Vec3f secondEdge = m_thirdPosition - m_secondPosition;
  const Vec3f secondHitDir = hitPosition - m_secondPosition;

  if ((secondEdge.cross(secondHitDir)).dot(m_normal) < 0)
    return false;

  const Vec3f thirdEdge = m_firstPosition - m_thirdPosition;
  const Vec3f thirdHitDir = hitPosition - m_thirdPosition;

  if ((thirdEdge.cross(thirdHitDir)).dot(m_normal) < 0)
    return false;

  if (hit) {
    hit->position = hitPosition;
    hit->normal = m_normal;
    hit->material = m_material;
    hit->distance = hitDistance;
  }

  return true;
}
