#include <iostream>

#include "RaZtracer/Math/Vector.hpp"
#include "RaZtracer/Render/Scene.hpp"
#include "RaZtracer/Utils/Shape.hpp"

int main() {
  Scene scene;
  scene.add(Sphere(Vec3f({ 1.5, 2.5, 3.5 }), 1.f, Vec3f({ 1.0, 1.0, 1.0 }), 1.f));
  scene.add(Cube(Vec3f({ 3.5, 1.5, 5.5 }), 1.f, Vec3f({ 1.0, 0.5, 0.2 }), 0.5f));

  Vec3f vec1({ 5, 3, 4 }), vec2({ -2, 5, -3 });
  vec1 = vec1.cross(vec2);
  std::cout << vec1[0] << ' ' << vec1[1] << ' ' << vec1[2] << std::endl;

  vec1 = vec1.normalize();
  std::cout << vec1[0] << ' ' << vec1[1] << ' ' << vec1[2] << std::endl;

  scene.render("output.png");

  return 0;
}
