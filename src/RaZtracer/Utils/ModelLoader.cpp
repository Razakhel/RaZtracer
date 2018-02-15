#include <algorithm>
#include <fstream>

#include "RaZtracer/Utils/ModelLoader.hpp"

namespace {

const std::string extractFileExt(const std::string& fileName) {
  return (fileName.substr(fileName.find_last_of('.') + 1));
}

void importOff(std::ifstream& file, std::vector<ShapePtr>& shapes) {
  std::size_t vertexCount, faceCount;
  file.ignore(3);
  file >> vertexCount >> faceCount;
  file.ignore(100, '\n');

  std::vector<Vec3f> positions(vertexCount);

  for (std::size_t vertexIndex = 0; vertexIndex < vertexCount; ++vertexIndex)
    file >> positions[vertexIndex][0] >> positions[vertexIndex][1] >> positions[vertexIndex][2];

  for (std::size_t faceIndex = 0; faceIndex < faceCount; ++faceIndex) {
    std::size_t firstIndex {}, secondIndex {}, thirdIndex {};

    file.ignore(2);
    file >> firstIndex >> secondIndex >> thirdIndex;

    shapes.emplace_back(std::make_unique<Triangle>(positions[firstIndex], positions[secondIndex], positions[thirdIndex]));
    file.ignore(100, '\n');
  }
}

} // namespace

void ModelLoader::importModel(const std::string& fileName, std::vector<ShapePtr>& shapes) {
  std::ifstream file(fileName, std::ios_base::in | std::ios_base::binary);

  if (file) {
    const std::string format = extractFileExt(fileName);

    if (format == "off" || format == "OFF")
      importOff(file, shapes);
    else
      throw std::runtime_error("Error: '" + format + "' format is not supported");
  } else {
    throw std::runtime_error("Error: Couldn't open the file '" + fileName + "'");
  }
}
