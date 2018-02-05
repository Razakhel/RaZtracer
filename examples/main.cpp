#include <chrono>
#include <iostream>

#include "RaZtracer/RaZtracer.hpp"

int main() {
  Scene scene;
  scene.addShape(std::make_unique<Sphere>(Vec3f(0.f), 1.f));

  scene.setCamera(std::make_unique<Camera>(800, 600, 45.f, 0.1f, 100.f, Vec3f({ 0.f, 0.f, 5.f })));

  const auto startTime = std::chrono::system_clock::now();
  scene.render("output.png");
  const auto endTime = std::chrono::system_clock::now();

  std::cout << "Scene rendering duration: "
            << std::chrono::duration_cast<std::chrono::duration<float>>(endTime - startTime).count()
            << " seconds." << std::endl;

  return 0;
}
