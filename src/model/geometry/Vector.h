//
// Created by gawain on 9/13/25.
//

#ifndef TURNLAB_VECTOR_H
#define TURNLAB_VECTOR_H

#include <vector>
#include <stdexcept>

#include "Point.h"

class Vector {
public:
    double x;
    double y;

    Vector();
    Vector(double x, double y);
    Vector(const std::vector<double>& vec);
    Vector(const Point& p1, const Point& p2);

    Vector& operator=(const std::vector<double>& vec);

    Vector operator+(const Vector& other) const;
    Vector operator-(const Vector& other) const;

    double dot(const Vector& other) const;
    double norm() const;

    bool operator==(const Vector& other) const;
    bool operator!=(const Vector& other) const;

    double& operator[](int index);
    const double& operator[](int index) const;
};


#endif //TURNLAB_VECTOR_H