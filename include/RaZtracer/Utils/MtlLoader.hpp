#pragma once

#ifndef RAZTRACER_MTLLOADER_HPP
#define RAZTRACER_MTLLOADER_HPP

#include <string>
#include <unordered_map>
#include <vector>

#include "../Render/Material.hpp"

namespace MtlLoader {

void importMtl(const std::string& file,
               std::vector<MaterialPtr>& materials,
               std::unordered_map<std::string, std::size_t>& materialCorrespIndices);

} // namespace MtlLoader

#endif // RAZTRACER_MTLLOADER_HPP
