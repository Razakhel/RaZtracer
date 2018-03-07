#pragma once

#ifndef RAZTRACER_MESH_HPP
#define RAZTRACER_MESH_HPP

#include <memory>
#include <string>

#include "../Math/Vector.hpp"
#include "Submesh.hpp"
#include "Material.hpp"

class Mesh {
public:
  Mesh() { m_submeshes.emplace_back(std::make_unique<Submesh>()); }

  const std::vector<SubmeshPtr>& getSubmeshes() const { return m_submeshes; }
  std::vector<SubmeshPtr>& getSubmeshes() { return m_submeshes; }
  const std::vector<MaterialPtr>& getMaterials() const { return m_materials; }
  std::vector<MaterialPtr>& getMaterials() { return m_materials; }

  void addSubmesh(SubmeshPtr submesh) { m_submeshes.emplace_back(std::move(submesh)); }
  void addMaterial(MaterialPtr material) { m_materials.emplace_back(std::move(material)); }

private:
  std::vector<SubmeshPtr> m_submeshes {};
  std::vector<MaterialPtr> m_materials {};
};

using MeshPtr = std::unique_ptr<Mesh>;

#endif // RAZTRACER_MESH_HPP
