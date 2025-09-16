//
// Created by gawain on 9/14/25.
//

#ifndef TURNLAB_PROJECT_H
#define TURNLAB_PROJECT_H
#include "Geometry.h"
#include "StockMaterial.h"

struct Project {
    std::string savePath;
    Geometry geometry;
    StockMaterial stockMaterial;
};

#endif //TURNLAB_PROJECT_H