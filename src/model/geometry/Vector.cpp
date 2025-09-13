//
// Created by gawain on 9/13/25.
//

#include "Vector.h"
#include <cmath>

Vector::Vector() : x(0.0), y(0.0) {}

Vector::Vector(double x, double y) : x(x), y(y) {}

Vector::Vector(const std::vector<double>& vec) {
    if (vec.size() != 2) {
        throw std::invalid_argument("Vector must have exactly 2 elements");
    }
    x = vec[0];
    y = vec[1];
}

Vector::Vector(const Point &p1, const Point &p2) {
    x = p2.x - p1.x;
    y = p2.y - p1.y;
}


Vector& Vector::operator=(const std::vector<double>& vec) {
    if (vec.size() != 2) {
        throw std::invalid_argument("Vector must have exactly 2 elements");
    }
    x = vec[0];
    y = vec[1];
    return *this;
}

Vector Vector::operator+(const Vector& other) const {
    return Vector(x + other.x, y + other.y);
}

Vector Vector::operator-(const Vector& other) const {
    return Vector(x - other.x, y - other.y);
}

double Vector::dot(const Vector& other) const {
    return x * other.x + y * other.y;
}

double Vector::norm() const {
    return std::sqrt(x * x + y * y);
}

bool Vector::operator==(const Vector& other) const {
    return x == other.x && y == other.y;
}

bool Vector::operator!=(const Vector& other) const {
    return !(*this == other);
}

double& Vector::operator[](int index) {
    if (index == 0) return x;
    if (index == 1) return y;
    throw std::out_of_range("Vector index out of range");
}

const double& Vector::operator[](int index) const {
    if (index == 0) return x;
    if (index == 1) return y;
    throw std::out_of_range("Vector index out of range");
}