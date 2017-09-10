#include <iostream>

#include "Scene.hpp"
#include "Shape.hpp"
#include "Vector.hpp"

int main() {
  Scene scene;
  scene.add(Sphere(Vec3f({ 1.5, 2.5, 3.5 }), 1.f, Vec3f({ 1.0, 1.0, 1.0 }), 1.f));
  scene.add(Cube(Vec3f({ 3.5, 1.5, 5.5 }), 1.f, Vec3f({ 1.0, 0.5, 0.2 }), 0.5f));

  scene.render();

  return 0;
}
