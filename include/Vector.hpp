#pragma once

#ifndef RAZTRACER_VECTOR_HPP
#define RAZTRACER_VECTOR_HPP

#include <array>
#include <initializer_list>

template <typename T, unsigned int Size>
class Vector {
public:
  Vector() {}
  Vector(const std::initializer_list<T>& list);

  const std::array<T, Size>& getData() const { return data; }
  std::array<T, Size>& getData() { return data; }

  T dot(const Vector& vec) const;
  Vector cross(const Vector& vec) const;

  Vector operator+(Vector vec);
  Vector operator+(float val);
  Vector operator-(Vector vec);
  Vector operator-(float val);
  Vector operator*(Vector vec);
  Vector operator*(float val);
  Vector operator/(Vector vec);
  Vector operator/(float val);
  Vector& operator+=(const Vector& vec);
  Vector& operator+=(float val);
  Vector& operator-=(const Vector& vec);
  Vector& operator-=(float val);
  Vector& operator*=(const Vector& vec);
  Vector& operator*=(float val);
  Vector& operator/=(const Vector& vec);
  Vector& operator/=(float val);
  const T& operator[](std::size_t index) const { return data[index]; }
  T& operator[](std::size_t index) { return data[index]; }

private:
  std::array<T, Size> data;
};

using Vec3f = Vector<float, 3>;

#include "Vector.inl"

#endif // RAZTRACER_VECTOR_HPP
