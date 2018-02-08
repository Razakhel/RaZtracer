#include <chrono>
#include <iostream>

#include "RaZtracer/RaZtracer.hpp"

int main() {
  Window window(800, 600, "RaZtracer");

  Scene scene;
  scene.addShape(std::make_unique<Sphere>(Vec3f(0.f), 1.f));
  scene.addShape(std::make_unique<Sphere>(Vec3f({ 0.f, 0.f, 3.f }), 1.f, Vec3f({ 1.f, 0.f, 0.f })));
  scene.addShape(std::make_unique<Sphere>(Vec3f({ 2.f, 1.f, 3.f }), 1.f, Vec3f({ 1.f, 0.f, 0.f })));
  scene.addShape(std::make_unique<Sphere>(Vec3f({ 1.f, -2.f, 2.f }), 1.f, Vec3f({ 0.f, 1.f, 1.f })));
  scene.addShape(std::make_unique<Sphere>(Vec3f({ -2.f, 3.f, 0.f }), 1.f, Vec3f({ 1.f, 1.f, 0.f })));
  scene.addShape(std::make_unique<Sphere>(Vec3f({ -1.f, 1.5f, 0.f }), 1.f, Vec3f({ 0.f, 1.f, 0.f })));
  scene.addShape(std::make_unique<Sphere>(Vec3f({ 2.f, -0.5f, 1.f }), 1.f, Vec3f({ 1.f, 1.f, 0.f })));
  scene.addShape(std::make_unique<Sphere>(Vec3f({ -3.f, -2.f, 3.f }), 1.f, Vec3f({ 1.f, 0.f, 1.f })));

  scene.addLight(std::make_unique<PointLight>(Vec3f(5.f), Vec3f(1.f)));

  scene.setCamera(std::make_unique<Camera>(800, 600, 45.f, 0.1f, 100.f, Vec3f({ 0.f, 0.f, -5.f })));

  const auto startTime = std::chrono::system_clock::now();
  ImagePtr img = scene.render();
  const auto endTime = std::chrono::system_clock::now();

  std::cout << "Scene rendering duration: "
            << std::chrono::duration_cast<std::chrono::duration<float>>(endTime - startTime).count()
            << " seconds." << std::endl;

  window.mapImage(std::move(img));

  while (window.show());

  return 0;
}
