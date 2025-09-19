//
// Created by gawain on 9/19/25.
//

#ifndef TURNLAB_TPOINT_H
#define TURNLAB_TPOINT_H

#include <nlohmann/json.hpp>

struct TPoint {
    double x = 0.0;
    double z = 0.0;

    TPoint() = default;
    TPoint(double x, double z) : x(x), z(z) {}

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(TPoint, x, z)
};

#endif //TURNLAB_TPOINT_H