//
// Created by gawain on 9/9/25.
//

#include "Point.h"
#include <stdexcept>

Point::Point(const std::vector<double>& vec) {
    if (vec.size() != 2) {
        throw std::invalid_argument("Vector must have exactly 2 elements for Point construction");
    }
    x = vec[0];
    y = vec[1];
}

double& Point::operator[](int index) {
    if (index == 0) return x;
    if (index == 1) return y;
    throw std::out_of_range("Point index out of range");
}

const double& Point::operator[](int index) const {
    if (index == 0) return x;
    if (index == 1) return y;
    throw std::out_of_range("Point index out of range");
}

Point Point::operator+(const Point& other) const {
    return Point(x + other.x, y + other.y);
}

Point Point::operator-(const Point& other) const {
    return Point(x - other.x, y - other.y);
}

Point& Point::operator=(const std::vector<double>& vec) {
    if (vec.size() != 2) {
        throw std::invalid_argument("Vector must have exactly 2 elements for Point assignment");
    }
    x = vec[0];
    y = vec[1];
    return *this;
}

Point::operator std::vector<double>() const {
    return {x, y};
}

bool Point::operator==(const Point& other) const {
    return x == other.x && y == other.y;
}

bool Point::operator!=(const Point& other) const {
    return !(*this == other);
}