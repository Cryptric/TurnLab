//
// Created by gawain on 9/9/25.
//

#ifndef TURNLAB_POINT_H
#define TURNLAB_POINT_H

#include <vector>
#include <nlohmann/json.hpp>

#include "Transform.h"

class Point {
public:
    double x;
    double y;

    Point() : x(0.0), y(0.0) {}
    Point(double x, double y) : x(x), y(y) {}
    Point(const std::vector<double>& vec);

    double& operator[](int index);
    const double& operator[](int index) const;

    Point operator+(const Point& other) const;
    Point operator-(const Point& other) const;
    Point& operator=(const std::vector<double>& vec);

    Point transform(const std::vector<Transform> &transformations);

    Point apply(const Transform &transformation);

    bool operator==(const Point& other) const;
    bool operator!=(const Point& other) const;

    operator std::vector<double>() const;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Point, x, y)
};

#endif //TURNLAB_POINT_H