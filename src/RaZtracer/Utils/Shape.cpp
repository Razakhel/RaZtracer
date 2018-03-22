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

Drawable::Drawable(const Vec3f& color, float transparency) {
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

bool AABB::intersect(const Vec3f& rayOrigin, const Vec3f& rayDirection, RayHit* hit) const {
  const Vec3f invDir({ 1.f / rayDirection[0], 1.f / rayDirection[1], 1.f / rayDirection[2] });
  Vec3f closestBound = m_bottomLeftPosition;
  Vec3f farthestBound = m_topRightPosition;

  if(rayDirection[0] < 0)
    std::swap(closestBound[0], farthestBound[0]);

  if(rayDirection[1] < 0)
    std::swap(closestBound[1], farthestBound[1]);

  if(rayDirection[2] < 0)
    std::swap(closestBound[2], farthestBound[2]);

  const Vec3f closestPlanePos = (closestBound - rayOrigin) * invDir;
  const Vec3f farthestPlanePos = (farthestBound - rayOrigin) * invDir;
  const float minHitDist = std::max(closestPlanePos[0], std::max(closestPlanePos[1], std::max(closestPlanePos[2], 0.f)));
  const float maxHitDist = std::min(farthestPlanePos[0], std::min(farthestPlanePos[1], farthestPlanePos[2]));

  if (hit) {
    hit->position = rayOrigin + rayDirection * minHitDist;
    hit->distance = minHitDist;
  }

  return (minHitDist <= maxHitDist);
}

bool Box::intersect(const Vec3f& rayOrigin, const Vec3f& rayDirection, RayHit* hit) const {
  const bool res = AABB::intersect(rayOrigin, rayDirection, hit);

  if (hit)
    hit->material = m_material;

  return res;
}

std::unique_ptr<AABB> Triangle::computeBoundingBox() const {
  const float xMax = std::max(m_firstVert.position[0], std::max(m_secondVert.position[0], m_thirdVert.position[0]));
  const float yMax = std::max(m_firstVert.position[1], std::max(m_secondVert.position[1], m_thirdVert.position[1]));
  const float zMax = std::max(m_firstVert.position[2], std::max(m_secondVert.position[2], m_thirdVert.position[2]));

  const float xMin = std::min(m_firstVert.position[0], std::min(m_secondVert.position[0], m_thirdVert.position[0]));
  const float yMin = std::min(m_firstVert.position[1], std::min(m_secondVert.position[1], m_thirdVert.position[1]));
  const float zMin = std::min(m_firstVert.position[2], std::min(m_secondVert.position[2], m_thirdVert.position[2]));

  return std::make_unique<AABB>(Vec3f({ xMax, yMax, zMax }), Vec3f({ xMin, yMin, zMin }));
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
    hit->normal = (m_firstVert.normal * thirdBaryCoord
                + m_secondVert.normal * firstBaryCoord
                + m_thirdVert.normal * secondBaryCoord).normalize();
    hit->texcoords = m_firstVert.texcoords * thirdBaryCoord
                   + m_secondVert.texcoords * firstBaryCoord
                   + m_thirdVert.texcoords * secondBaryCoord;
    hit->material = m_material;
    hit->distance = hitDist;
  }

  return (hitDist > 0);
}
