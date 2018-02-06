#pragma once

#ifndef RAZTRACER_WINDOW_HPP
#define RAZTRACER_WINDOW_HPP

#include "GLFW/glfw3.h"
#include "RaZtracer/Utils/Image.hpp"

class Window {
public:
  Window(unsigned int width, unsigned int height, const std::string& name = "");

  void mapImage(std::unique_ptr<Image> img) const;
  bool show() const;
  void close() const;

  ~Window() { close(); }

private:
  GLFWwindow* m_window;

  GLuint m_vaoIndex;
  GLuint m_vboIndex;
  GLuint m_eboIndex;
  GLuint m_shaderProgram;
  GLuint m_textureIndex;
};

#endif // RAZTRACER_WINDOW_HPP
