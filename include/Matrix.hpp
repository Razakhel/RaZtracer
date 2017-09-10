#pragma once

#ifndef RAZTRACER_MATRIX_HPP
#define RAZTRACER_MATRIX_HPP

#include <array>

template <typename T, unsigned int W, unsigned int H>
class Matrix {
public:
  Matrix() {}
  Matrix(const std::initializer_list<const std::initializer_list<T>>& list);

  template <typename TI, unsigned int WI, unsigned int HI> Matrix(const Matrix<TI, WI, HI>& mat);

  std::size_t getWidth() const { return width; }
  std::size_t getHeight() const { return height; }
  const std::array<T, W * H>& getData() const { return data; }
  std::array<T, W * H>& getData() { return data; }

  Matrix operator+(Matrix mat);
  Matrix operator+(float val);
  Matrix operator-(Matrix mat);
  Matrix operator-(float val);
  Matrix operator*(Matrix mat);
  Matrix operator*(float val);
  Matrix operator/(Matrix mat);
  Matrix operator/(float val);
  Matrix& operator+=(const Matrix& mat);
  Matrix& operator+=(float val);
  Matrix& operator-=(const Matrix& mat);
  Matrix& operator-=(float val);
  Matrix& operator*=(const Matrix& mat);
  Matrix& operator*=(float val);
  Matrix& operator/=(const Matrix& mat);
  Matrix& operator/=(float val);
  template <typename TI, unsigned int WI, unsigned int HI> Matrix operator=(const Matrix<TI, WI, HI>& mat) { return Matrix<T, W, H>(mat); }
  T& operator()(unsigned int widthIndex, unsigned int heightIndex) { return data[heightIndex * width + widthIndex]; }
  const T& operator[](unsigned int index) const { return data[index]; }
  T& operator[](unsigned int index) { return data[index]; }

private:
  std::size_t width, height;
  std::array<T, W * H> data;
};

using Mat3f = Matrix<float, 3, 3>;

#include "Matrix.inl"

#endif // RAZTRACER_MATRIX_HPP
