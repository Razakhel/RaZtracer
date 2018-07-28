#pragma once

#ifndef RAZTRACER_MODELLOADER_HPP
#define RAZTRACER_MODELLOADER_HPP

#include "../Utils/Shape.hpp"

namespace ModelLoader {

void importModel(const std::string& file, std::vector<DrawableShapePtr>& shapes);

} // namespace ModelLoader

#endif // RAZTRACER_MODELLOADER_HPP
