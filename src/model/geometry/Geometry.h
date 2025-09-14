//
// Created by gawain on 9/9/25.
//

#ifndef TURNLAB_GEOMETRY_H
#define TURNLAB_GEOMETRY_H

#include <vector>

#include "Segment.h"
#include "Transform.h"

class Geometry {
public:
    Geometry() = default;
    explicit Geometry(std::vector<std::unique_ptr<Segment>>& segments);
    Geometry(const Geometry& other);
    virtual ~Geometry() = default;

    Geometry& operator=(const Geometry& other);

    // Make movable
    Geometry(Geometry&&) = default;
    Geometry& operator=(Geometry&&) = default;

    Geometry transform(std::vector<Transform> transformations);

    std::vector<std::unique_ptr<Segment>> segments;
};

#endif //TURNLAB_GEOMETRY_H