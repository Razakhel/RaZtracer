#pragma once

#ifndef RAZTRACER_SUBMESH_HPP
#define RAZTRACER_SUBMESH_HPP

#include <memory>

#include "../Utils/Shape.hpp"

class Submesh {
public:
  Submesh() = default;

  const std::vector<Triangle>& getTriangles() const { return m_triangles; }
  std::vector<Triangle>& getTriangles() { return m_triangles; }
  std::size_t getMaterialIndex() const { return m_materialIndex; }

  void setMaterialIndex(std::size_t materialIndex) { m_materialIndex = materialIndex; }

private:
  std::vector<Triangle> m_triangles;

  std::size_t m_materialIndex {};
};

using SubmeshPtr = std::unique_ptr<Submesh>;

#endif // RAZTRACER_SUBMESH_HPP
