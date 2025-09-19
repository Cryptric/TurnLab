//
// Created by gawain on 9/9/25.
//

#include <memory>

#include "Line.h"
#include "Vector.h"

bool Line::operator==(const Segment &other) const {
    if (auto *otherLine = dynamic_cast<const Line*>(&other)) {
        return (p1 == otherLine->p1 && p2 == otherLine->p2) || (p1 == otherLine->p2 && p2 == otherLine->p1);
    }
    return false;
}

std::unique_ptr<Segment> Line::transform(const std::vector<Transform>& transformations) {
    Point newP1 = p1.transform(transformations);
    Point newP2 = p2.transform(transformations);
    return std::make_unique<Line>(newP1, newP2);
}


std::unique_ptr<Segment> Line::clone() const {
    return std::make_unique<Line>(*this);
}

bool Line::operator!=(const Segment &other) const {
    return !(*this == other);
}

double Line::distance(const Point &point) const {
    Vector b = Vector(p1, p2);
    if (b.norm() == 0) {
        return Vector(p1, point).norm();
    }
    Vector q = Vector(p1, point);
    double qp = q.dot(b) / b.dot(b);
    if (qp > 1) {
        return Vector(p2, point).norm();
    } else if (qp < 0) {
        return Vector(p1, point).norm();
    } else {
        return std::abs((q[1] * b[0] - q[0] * b[1]) / b.norm());
    }
}

bool Line::isVertical() const {
    return p1.x == p2.x;
}
