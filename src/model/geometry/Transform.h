//
// Created by gawain on 9/14/25.
//

#ifndef TURNLAB_TRANSFORM_H
#define TURNLAB_TRANSFORM_H
#include <array>


class Transform {
public:
    std::array<std::array<double, 2>, 2> linear = {{{1.0, 0.0}, {0.0, 1.0}}};
    std::array<double, 2> translation = {0.0, 0.0};

    static Transform identity();
    static Transform translate(double x, double y);
    static Transform scale(double x, double y);
    static Transform rotate(double angle);
    static Transform mirrorX();
    static Transform mirrorY();

};


#endif //TURNLAB_TRANSFORM_H