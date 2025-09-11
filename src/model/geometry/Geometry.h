//
// Created by gawain on 9/9/25.
//

#ifndef TURNLAB_GEOMETRY_H
#define TURNLAB_GEOMETRY_H

#include <vector>

#include "Segment.h"

class Geometry {
public:
    Geometry() = default;
    explicit Geometry(std::vector<std::unique_ptr<Segment>>& segments);
    virtual ~Geometry() = default;

    std::vector<std::unique_ptr<Segment>> segments;
};

#endif //TURNLAB_GEOMETRY_H