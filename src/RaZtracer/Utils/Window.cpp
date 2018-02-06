#include <array>
#include <iostream>
#include <memory>

#include "GL/glew.h"
#include "RaZtracer/Utils/Window.hpp"

namespace {

void GLAPIENTRY callbackDebugLog(GLenum source,
                                 GLenum type,
                                 GLuint id,
                                 GLenum severity,
                                 GLsizei length,
                                 const GLchar* message,
                                 const void* userParam) {
  std::cerr << "OpenGL Debug - ";

  switch (source) {
    case GL_DEBUG_SOURCE_API: std::cerr << "Source: OpenGL\t"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM: std::cerr << "Source: Windows\t"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cerr << "Source: Shader compiler\t"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY: std::cerr << "Source: Third party\t"; break;
    case GL_DEBUG_SOURCE_APPLICATION: std::cerr << "Source: Application\t"; break;
    case GL_DEBUG_SOURCE_OTHER: std::cerr << "Source: Other\t"; break;
    default: break;
  }

  switch (type) {
    case GL_DEBUG_TYPE_ERROR: std::cerr << "Type: Error\t"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cerr << "Type: Deprecated behavior\t"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: std::cerr << "Type: Undefined behavior\t"; break;
    case GL_DEBUG_TYPE_PORTABILITY: std::cerr << "Type: Portability\t"; break;
    case GL_DEBUG_TYPE_PERFORMANCE: std::cerr << "Type: Performance\t"; break;
    case GL_DEBUG_TYPE_OTHER: std::cerr << "Type: Other\t"; break;
    default: break;
  }

  std::cout << "ID: " << id << "\t";

  switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH: std::cerr << "Severity: High\t"; break;
    case GL_DEBUG_SEVERITY_MEDIUM: std::cerr << "Severity: Medium\t"; break;
    case GL_DEBUG_SEVERITY_LOW: std::cerr << "Severity: Low\t"; break;
    default: break;
  }

  std::cerr << "Message: " << message << std::endl;
}

GLuint initShaders() {
  const std::string vertexShaderStr = R"(
    #version 330 core

    layout (location = 0) in vec3 vertPosition;
    layout (location = 1) in vec2 vertTexcoords;

    out vec2 fragTexcoords;

    void main() {
        gl_Position = vec4(vertPosition, 1.0);
        fragTexcoords = vertTexcoords;
    }
  )";

  const std::string fragmentShaderStr = R"(
    #version 330 core

    uniform sampler2D uniTexture;

    in vec2 fragTexcoords;

    void main() {
        gl_FragColor = texture(uniTexture, fragTexcoords);
    }
  )";

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, reinterpret_cast<const GLchar* const*>(&vertexShaderStr), nullptr);
  glCompileShader(vertexShader);

  GLint success;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

  if (!success) {
    std::array<GLchar, 512> infoLog;

    glGetShaderInfoLog(vertexShader, infoLog.size(), nullptr, infoLog.data());
    std::cerr << "Error: Vertex shader compilation failed.\n" << infoLog.data() << std::endl;
  }

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, reinterpret_cast<const GLchar* const*>(&fragmentShaderStr), nullptr);
  glCompileShader(fragmentShader);

  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

  if (!success) {
    std::array<GLchar, 512> infoLog;

    glGetShaderInfoLog(vertexShader, infoLog.size(), nullptr, infoLog.data());
    std::cerr << "Error: Fragment shader compilation failed.\n" << infoLog.data() << std::endl;
  }

  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

  if (!success) {
    std::array<GLchar, 512> infoLog;

    glGetProgramInfoLog(shaderProgram, infoLog.size(), nullptr, infoLog.data());
    std::cerr << "Error: Shader program link failed.\n" << infoLog.data() << std::endl;
  }

  return shaderProgram;
}

} // namespace

Window::Window(unsigned int width, unsigned int height, const std::string& name) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  m_window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
  if (!m_window) {
    std::cerr << "Error: Failed to create GLFW Window." << std::endl;
    glfwTerminate();
  }

  glfwMakeContextCurrent(m_window);
  glfwSetKeyCallback(m_window, [] (GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GL_TRUE);
  });

  glfwMakeContextCurrent(m_window);
  glViewport(0, 0, width, height);

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)
    std::cerr << "Error: Failed to initialize GLEW." << std::endl;

  glDebugMessageCallback(&callbackDebugLog, nullptr);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

  m_shaderProgram = initShaders();

  glGenVertexArrays(1, &m_vaoIndex);
  glGenBuffers(1, &m_vboIndex);
  glGenBuffers(1, &m_eboIndex);
  glGenTextures(1, &m_textureIndex);

  glBindVertexArray(m_vaoIndex);

  const std::array<unsigned int, 6> indices = { 0, 1, 3,
                                                1, 2, 3 };

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboIndex);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  const std::array<float, 20> vertices = { -1.f, -1.f, 0.f,   0.f, 1.f,
                                            1.f, -1.f, 0.f,   1.f, 1.f,
                                            1.f,  1.f, 0.f,   1.f, 0.f,
                                           -1.f,  1.f, 0.f,   0.f, 0.f };

  glBindBuffer(GL_ARRAY_BUFFER, m_vboIndex);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  glBindTexture(GL_TEXTURE_2D, m_textureIndex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glUseProgram(m_shaderProgram);
  glBindVertexArray(m_vaoIndex);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboIndex);
}

void Window::mapImage(std::unique_ptr<Image> img) const {
  if (img->getColorspace() == RAZTRACER_COLORSPACE_GRAY || img->getColorspace() == RAZTRACER_COLORSPACE_GRAY_ALPHA) {
    const std::array<GLint, 4> swizzle = { GL_RED,
                                           GL_RED,
                                           GL_RED,
                                           (img->getColorspace() == RAZTRACER_COLORSPACE_GRAY ? GL_ONE : GL_GREEN) };
    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle.data());
  }

  GLenum imgFormat;
  switch (img->getColorspace()) {
    case RAZTRACER_COLORSPACE_GRAY:
      imgFormat = GL_RED;
      break;

    case RAZTRACER_COLORSPACE_GRAY_ALPHA:
      imgFormat = GL_RG;
      break;

    case RAZTRACER_COLORSPACE_RGB:
    default:
      imgFormat = GL_RGB;
      break;

    case RAZTRACER_COLORSPACE_RGBA:
      imgFormat = GL_RGBA;
      break;
  }

  glTexImage2D(GL_TEXTURE_2D, 0, imgFormat, img->getWidth(), img->getHeight(), 0, imgFormat, GL_UNSIGNED_BYTE, img->getDataPtr());
}

bool Window::show() const {
  if (glfwWindowShouldClose(m_window))
    return false;

  glClear(GL_COLOR_BUFFER_BIT);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

  glfwSwapBuffers(m_window);
  glfwPollEvents();

  return true;
}

void Window::close() const {
  glDeleteVertexArrays(1, &m_vaoIndex);
  glDeleteBuffers(1, &m_vboIndex);
  glDeleteBuffers(1, &m_eboIndex);
  glDeleteTextures(1, &m_textureIndex);

  glfwTerminate();
}
