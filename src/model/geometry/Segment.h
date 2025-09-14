//
// Created by gawain on 9/9/25.
//

#ifndef TURNLAB_SEGMENT_H
#define TURNLAB_SEGMENT_H

#include <memory>
#include <vector>

#include "Point.h"
#include "Transform.h"

class Segment {

public:
    virtual ~Segment() = default;
    virtual double distance(const Point& point) const = 0;
    virtual bool operator==(const Segment& other) const = 0;
    virtual std::unique_ptr<Segment> transform(const std::vector<Transform>& transformations) = 0;
    virtual std::unique_ptr<Segment> clone() const = 0;

};


#endif //TURNLAB_SEGMENT_H