//
// Created by gawain on 9/9/25.
//

#ifndef TURNLAB_DXFUTILS_H
#define TURNLAB_DXFUTILS_H
#include <filesystem>

#include "../model/geometry/Geometry.h"

Geometry loadDXF(const std::filesystem::path &path);


#endif //TURNLAB_DXFUTILS_H
