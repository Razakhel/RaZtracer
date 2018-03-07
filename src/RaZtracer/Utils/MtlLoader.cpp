#include <fstream>
#include <memory>

#include "RaZtracer/Utils/MtlLoader.hpp"

namespace MtlLoader {

void importMtl(const std::string& fileName,
               std::vector<MaterialPtr>& materials,
               std::unordered_map<std::string, std::size_t>& materialCorrespIndices) {
  std::ifstream file(fileName, std::ios_base::in | std::ios_base::binary);

  if (file) {
    while (!file.eof()) {
      std::string line;
      std::string nextValue;
      file >> line >> nextValue;

      if (line[0] == 'K') { // Assign properties
        std::string secondValue, thirdValue;
        file >> secondValue >> thirdValue;

        const float red = std::stof(nextValue);
        const float green = std::stof(secondValue);
        const float blue = std::stof(thirdValue);

        if (line[1] == 'a') {
          materials.back()->setAmbientColor(red, green, blue);
        } else if (line[1] == 'd') {
          materials.back()->setDiffuseColor(red, green, blue);
        } else if (line[1] == 's') {
          materials.back()->setSpecularColor(red, green, blue);
        } else if (line[1] == 'e') {
          materials.back()->setEmissiveColor(red, green, blue);
        }
      } else if (line[0] == 'm') { // Import texture
        if (line[4] == 'K') {
          if (line[5] == 'a') {
            materials.back()->loadAmbientMap(nextValue);
          } else if (line[5] == 'd') {
            materials.back()->loadDiffuseMap(nextValue);
          } else if (line[5] == 's') {
            materials.back()->loadSpecularMap(nextValue);
          }
        } else if (line[4] == 'd') {
          materials.back()->loadTransparencyMap(nextValue);
        } else if (line[4] == 'b') {
          materials.back()->loadBumpMap(nextValue);
        }
      } else if (line[0] == 'T') {
        if (line[1] == 'r') { // Assign transparency
          materials.back()->setTransparency(std::stof(nextValue));
        }/* else if (line[1] == 'f') { // Assign transmission filter

        }*/
      } else if (line[0] == 'd') { // Assign transparency
        materials.back()->setTransparency(std::stof(nextValue));
      } else if (line[0] == 'b') { // Import bump map
        materials.back()->loadBumpMap(nextValue);
      } else if (line[0] == 'n') { // Create new material
        materials.emplace_back(std::make_unique<Material>());
        materialCorrespIndices.insert({ nextValue, materialCorrespIndices.size() });
      } else {
        std::getline(file, line); // Skip the rest of the line
      }
    }
  } else {
    throw std::runtime_error("Error: Couldn't open the file '" + fileName + "'");
  }
}

} // namespace MtlLoader
