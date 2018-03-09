#pragma once

#ifndef RAZTRACER_MATERIAL_HPP
#define RAZTRACER_MATERIAL_HPP

#include "../Math/Vector.hpp"
#include "../Utils/Image.hpp"

class Material {
public:
  Material() = default;
  explicit Material(ImagePtr diffuseMap) : m_diffuseMap{ std::move(diffuseMap) } {}
  explicit Material(const std::string& fileName) { m_diffuseMap = std::make_unique<Image>(fileName); }

  const Vec3f& getDiffuseColor() const { return m_diffuseColor; }
  Vec3f& getDiffuseColor() { return m_diffuseColor; }
  const Image* getAmbientMap() const { return m_ambientMap.get(); }
  const Image* getDiffuseMap() const { return m_diffuseMap.get(); }
  const Image* getSpecularMap() const { return m_specularMap.get(); }
  const Image* getTransparencyMap() const { return m_transparencyMap.get(); }
  const Image* getBumpMap() const { return m_bumpMap.get(); }

  void setDiffuseColor(float red, float green, float blue) { setDiffuseColor(Vec3f({ red, green, blue })); }
  void setDiffuseColor(const Vec3f& color) { m_diffuseColor = color; }
  void setAmbientColor(float red, float green, float blue) { setAmbientColor(Vec3f({ red, green, blue })); }
  void setAmbientColor(const Vec3f& color) { m_ambientColor = color; }
  void setSpecularColor(float red, float green, float blue) { setSpecularColor(Vec3f({ red, green, blue })); }
  void setSpecularColor(const Vec3f& color) { m_specularColor = color; }
  void setEmissiveColor(float red, float green, float blue) { setEmissiveColor(Vec3f({ red, green, blue })); }
  void setEmissiveColor(const Vec3f& color) { m_emissiveColor = color; }
  void setTransparency(float transparency) { m_transparency = transparency; }

  void loadDiffuseMap(const std::string& fileName) { m_diffuseMap = std::make_unique<Image>(fileName); }
  void loadAmbientMap(const std::string& fileName) { m_ambientMap = std::make_unique<Image>(fileName); }
  void loadSpecularMap(const std::string& fileName) { m_specularMap = std::make_unique<Image>(fileName); }
  void loadTransparencyMap(const std::string& fileName) { m_transparencyMap = std::make_unique<Image>(fileName); }
  void loadBumpMap(const std::string& fileName) { m_bumpMap = std::make_unique<Image>(fileName); }

private:
  Vec3f m_diffuseColor {};
  Vec3f m_ambientColor {};
  Vec3f m_specularColor {};
  Vec3f m_emissiveColor {};
  float m_transparency {};

  ImagePtr m_diffuseMap {};
  ImagePtr m_ambientMap {};
  ImagePtr m_specularMap {};
  ImagePtr m_transparencyMap {};
  ImagePtr m_bumpMap {};
};

using MaterialPtr = std::shared_ptr<Material>;

#endif // RAZTRACER_MATERIAL_HPP
