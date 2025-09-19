//
// Created by gawain on 9/14/25.
//

#ifndef TURNLAB_PROJECT_H
#define TURNLAB_PROJECT_H
#include <nlohmann/json.hpp>
#include "Geometry.h"
#include "StockMaterial.h"
#include "operation/OperationConfiguration.h"

struct Project {
    std::string savePath;
    Geometry geometry;
    StockMaterial stockMaterial;

    std::vector<OperationConfiguration> operations;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Project, savePath, geometry, stockMaterial, operations)
};

#endif //TURNLAB_PROJECT_H