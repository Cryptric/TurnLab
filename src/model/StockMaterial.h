//
// Created by gawain on 9/14/25.
//

#ifndef TURNLAB_STOCKMATERIAL_H
#define TURNLAB_STOCKMATERIAL_H

#include <nlohmann/json.hpp>

struct StockMaterial {
    double startPosition;
    double endPosition;
    double radius;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(StockMaterial, startPosition, endPosition, radius)
};


#endif //TURNLAB_STOCKMATERIAL_H