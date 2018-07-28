#pragma once

#ifndef RAZTRACER_CAMERA_HPP
#define RAZTRACER_CAMERA_HPP

#include <memory>

#include "RaZtracer/Math/Vector.hpp"
#include "RaZtracer/Math/Constants.hpp"
#include "RaZtracer/Math/Transform.hpp"

class Camera : public Transform {
public:
  Camera(unsigned int frameWidth,
         unsigned int frameHeight,
         float fieldOfViewDegrees,
         float nearPlane,
         float farPlane,
         const Vec3f& position = Vec3f(0.f))
    : m_frameWidth{ frameWidth },
      m_frameHeight{ frameHeight },
      m_frameRatio{ static_cast<float>(frameWidth) / frameHeight },
      m_fieldOfView{ fieldOfViewDegrees * pi<float> / 180 },
      m_nearPlane{ nearPlane },
      m_farPlane{ farPlane } { m_position = position; }

  unsigned int getFrameWidth() const { return m_frameWidth; }
  unsigned int getFrameHeight() const { return m_frameHeight; }
  float getAspectRatio() const { return m_frameRatio; }
  float getFieldOfView() const { return m_fieldOfView; }

  Vec3f computeRayDirection(float widthPos, float heightPos) const;
  Mat4f computePerspectiveMatrix() const;
  Mat4f lookAt(const Vec3f& target = Vec3f(0.f), const Vec3f& orientation = Vec3f({ 0.f, 1.f, 0.f })) const;

private:
  unsigned int m_frameWidth;
  unsigned int m_frameHeight;
  float m_frameRatio;
  float m_fieldOfView;
  float m_nearPlane;
  float m_farPlane;
};

using CameraPtr = std::unique_ptr<Camera>;

#endif // RAZTRACER_CAMERA_HPP
