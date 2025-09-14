//
// Created by gawain on 9/14/25.
//

#include "Transform.h"

#include <cmath>

Transform Transform::identity() {
    return {};
}

Transform Transform::translate(const double x, const double y) {
    Transform t;
    t.translation = {x, y};
    return t;
}

Transform Transform::scale(const double x, const double y) {
    Transform t;
    t.linear = {{{x, 0.0}, {0.0, y}}};
    return t;
}

Transform Transform::rotate(const double angle) {
    Transform t;
    t.linear = {{{std::cos(angle), -std::sin(angle)}, {std::sin(angle), std::cos(angle)}}};
    return t;
}

Transform Transform::mirrorX() {
    return scale(-1.0, 1.0);
}

Transform Transform::mirrorY() {
    return scale(1.0, -1.0);
}