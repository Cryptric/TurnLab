//
// Created by gawain on 9/19/25.
//

#include "GeometryUtils.h"


std::tuple<Point, Point> GeometryUtils::getInnerAndOuterPoint(const Line &l, const MachineConfig& config) {
    return getInnerAndOuterPoint(l.p1, l.p2, config);
}

std::tuple<Point, Point> GeometryUtils::getInnerAndOuterPoint(const Point &p1, const Point &p2, const MachineConfig& config) {
    const Point& smallerX = p1.y < p2.y ? p1 : p2;
    const Point& largerX = p1.y > p2.y ? p1 : p2;
    if (config.xAxisDirection == AxisDirection::Positive) {
        return {smallerX, largerX};
    } else {
        return {largerX, smallerX};
    }
}

std::tuple<Point, Point> GeometryUtils::getChuckAndTailstockPoint(const Line &l, const MachineConfig& config) {
    return getChuckAndTailstockPoint(l.p1, l.p2, config);
}

std::tuple<Point, Point> GeometryUtils::getChuckAndTailstockPoint(const Point &p1, const Point &p2, const MachineConfig& config) {
    const Point& smallerZ = p1.x < p2.x ? p1 : p2;
    const Point& largerZ = p1.x > p2.x ? p1 : p2;
    if (config.zAxisDirection == AxisDirection::Positive) {
        return {smallerZ, largerZ};
    } else {
        return {largerZ, smallerZ};
    }
}
