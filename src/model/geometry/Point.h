//
// Created by gawain on 9/9/25.
//

#ifndef TURNLAB_POINT_H
#define TURNLAB_POINT_H

#include <vector>

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

    bool operator==(const Point& other) const;
    bool operator!=(const Point& other) const;

    operator std::vector<double>() const;
};

#endif //TURNLAB_POINT_H