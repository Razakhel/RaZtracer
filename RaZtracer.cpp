
#include <iostream>
#include <vector>

template<typename T>
class Vec3 {
public:
  Vec3() : x{T{0.f}}, y{T{0.f}}, z{T{0.f}} {}
  Vec3(const Vec3& vec) {}
  Vec3(const T& x, const T& y, const T& z) : x{x}, y{y}, z{z} {}

  T dot(const Vec3& vec) { return x * vec.x + y * vec.y + z * vec.z; }

  Vec3<T>& operator+(const Vec3& vec) { return Vec3<T>(x + vec.x, y + vec.y, z + vec.z); }
  Vec3<T>& operator-(const Vec3& vec) { return Vec3<T>(x - vec.x, y - vec.y, z - vec.z); }
  Vec3<T>& operator*(const float& coeff) { return Vec3<T>(x * coeff, y * coeff, z * coeff); }
  Vec3<T>& operator*(const Vec3& vec) { return Vec3<T>(x * vec.x, y * vec.y, z * vec.z); }

  friend std::ostream& operator<<(std::ostream& os, Vec3& vec) {
    return (os << "[ " << vec.x << ", " << vec.y << ", " << vec.z << " ]");
  }

  ~Vec3() {}
private:
  T x, y, z;
};
typedef Vec3<float> Vec3f;

/*template<typename T>
class Mat {
public:

private:
  std::vector<Vec3<T>> content;
};
typedef Mat<float> Mat3f;*/

class Shape {};

class Sphere : public Shape {
public:
  Sphere() : origin{Vec3f{0.f, 0.f, 0.f}}, radius{1.f}, color{Vec3f{0.5f, 0.5f, 0.5f}}, transparency{1.f} {}
  Sphere(const Vec3f& origin, const float& radius, const Vec3f& color, const float& transparency)
    : origin{origin}, radius{radius}, color{color}, transparency{transparency} {}

  ~Sphere() {}
private:
  Vec3f origin;
  float radius;
  Vec3f color;
  float transparency;
};

class Cube : public Shape {
public:
  Cube() : origin{Vec3f{0.f, 0.f, 0.f}}, size{1.f}, color{Vec3f{0.5f, 0.5f, 0.5f}}, transparency{1.f} {}
  Cube(const Vec3f& origin, const float& size, const Vec3f& color, const float& transparency)
    : origin{origin}, size{size}, color{color}, transparency{transparency} {}

  ~Cube() {}
private:
  Vec3f origin;
  float size;
  Vec3f color;
  float transparency;
};

void render(const std::vector<Shape>& scene) {
  unsigned int width = 1366, height = 768;  // Width & height of the rendered picture

  std::cout << sizeof(scene) << std::endl;
}

int main() {
  Vec3f vec(1.5, 2.5, 3.5);
  Vec3f vec2(3.5, 1.5, 5.5);

  std::vector<Shape> scene;
  scene.push_back(Sphere(vec, 1.f, Vec3f(1.0, 1.0, 1.0), 1.f));
  scene.push_back(Cube(vec2, 1.f, Vec3f(1.0, 0.5, 0.2), 0.5f));

  render(scene);

  return 0;
}
