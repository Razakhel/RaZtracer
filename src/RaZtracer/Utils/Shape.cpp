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
  m_material = std::make_shared<Material>();
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
  const Vec3f firstEdge = m_secondVert.position - m_firstVert.position;
  const Vec3f secondEdge = m_thirdVert.position - m_firstVert.position;
  const Vec3f normal = (firstEdge.cross(secondEdge)).normalize();

  m_firstVert.normal = normal;
  m_secondVert.normal = normal;
  m_thirdVert.normal = normal;
}

bool Triangle::intersect(const Vec3f& rayOrigin, const Vec3f& rayDirection, RayHit* hit) const {
  const Vec3f firstEdge = m_secondVert.position - m_firstVert.position;
  const Vec3f secondEdge = m_thirdVert.position - m_firstVert.position;
  const Vec3f pVec = rayDirection.cross(secondEdge);
  const float determinant = firstEdge.dot(pVec);

  if (std::abs(determinant) < std::numeric_limits<float>::epsilon())
    return false;

  const float invDeterm = 1 / determinant;

  const Vec3f invPlaneDir = rayOrigin - m_firstVert.position;
  const float firstBaryCoord = invPlaneDir.dot(pVec) * invDeterm;

  if (firstBaryCoord < 0 || firstBaryCoord > 1)
    return false;

  const Vec3f qVec = invPlaneDir.cross(firstEdge);
  const float secondBaryCoord = qVec.dot(rayDirection) * invDeterm;

  if (secondBaryCoord < 0 || firstBaryCoord + secondBaryCoord > 1)
    return false;

  const float thirdBaryCoord = 1 - (firstBaryCoord + secondBaryCoord);
  const float hitDist = secondEdge.dot(qVec) * invDeterm;

  if (hit) {
    hit->position = rayOrigin + rayDirection * hitDist;
    hit->normal = m_firstVert.normal * firstBaryCoord
                + m_secondVert.normal * secondBaryCoord
                + m_thirdVert.normal * thirdBaryCoord;
    hit->texcoords = m_firstVert.texcoords * firstBaryCoord
                   + m_secondVert.texcoords * secondBaryCoord
                   + m_thirdVert.texcoords * thirdBaryCoord;
    hit->material = m_material;
    hit->distance = hitDist;
  }

  return true;
}
