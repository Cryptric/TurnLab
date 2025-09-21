//
// Created by gawain on 9/19/25.
//

#ifndef TURNLAB_GEOMETRYUTILS_H
#define TURNLAB_GEOMETRYUTILS_H
#include <tuple>

#include "Line.h"
#include "Point.h"
#include "../model/MachineConfig.h"

class GeometryUtils {
public:
    static std::tuple<Point, Point> getInnerAndOuterPoint(const Line& l, const MachineConfig& config);
    static std::tuple<Point, Point> getInnerAndOuterPoint(const Point& p1, const Point& p2, const MachineConfig& config);

    static std::tuple<Point, Point> getChuckAndTailstockPoint(const Line& l, const MachineConfig& config);
    static std::tuple<Point, Point> getChuckAndTailstockPoint(const Point& p1, const Point& p2, const MachineConfig& config);

};


#endif //TURNLAB_GEOMETRYUTILS_H