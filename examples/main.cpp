#include "RaZtracer/RaZtracer.hpp"

int main() {
  Scene scene;
  scene.addShape(std::make_unique<Sphere>(Vec3f({ 1.5f, 2.5f, 3.5f }), 1.f));
  scene.addShape(std::make_unique<Cube>(Vec3f({ 3.5f, 1.5f, 5.5f }), 1.f));
  //scene.addShape(std::make_unique<Triangle>(Vec3f({  })));

  scene.setCamera(std::make_unique<Camera>(800, 600, 45.f, 0.1f, 100.f, Vec3f({ 0.f, 0.f, -10.f })));

  scene.render("output.png", 1920, 1080);

  return 0;
}
