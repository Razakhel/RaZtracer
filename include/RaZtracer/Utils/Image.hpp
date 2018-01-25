#pragma once

#ifndef RAZTRACER_IMAGE_HPP
#define RAZTRACER_IMAGE_HPP

#include <string>
#include <vector>

enum Colorspace {
  RAZTRACER_COLORSPACE_GRAY = 0,
  RAZTRACER_COLORSPACE_GRAY_ALPHA,
  RAZTRACER_COLORSPACE_RGB,
  RAZTRACER_COLORSPACE_RGBA
};

class Image {
public:
  Image() : m_data(3, 255) {}
  Image(std::size_t width, std::size_t height) : m_width{ width }, m_height{ height }, m_data(width * height) {}
  explicit Image(const std::string& fileName) { read(fileName); }

  std::size_t getWidth() const { return m_width; }
  std::size_t getHeight() const { return m_height; }
  Colorspace getColorspace() const { return m_colorspace; }
  const std::vector<uint8_t>& getData() const { return m_data; }
  const uint8_t* getDataPtr() const { return m_data.data(); }

private:
  void readPng(std::ifstream& fileName);
  void read(const std::string& fileName);

  void writePng(std::ofstream& fileName);
  void write(const std::string& fileName);

  std::size_t m_width {};
  std::size_t m_height {};
  Colorspace m_colorspace {};
  std::vector<uint8_t> m_data {};
};

#endif // RAZTRACER_IMAGE_HPP
