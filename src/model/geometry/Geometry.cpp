//
// Created by gawain on 9/11/25.
//

#include <memory>

#include "Geometry.h"

Geometry::Geometry(std::vector<std::unique_ptr<Segment>>& segments) {
    this->segments = std::move(segments);
}

Geometry::Geometry(const Geometry& other) {
    segments.reserve(other.segments.size());
    for (const auto& seg : other.segments) {
        if (seg) {
            segments.push_back(seg->clone());
        }
    }
}

Geometry& Geometry::operator=(const Geometry& other) {
    if (this != &other) {
        segments.clear();
        segments.reserve(other.segments.size());
        for (const auto& seg : other.segments) {
            if (seg) {
                segments.push_back(seg->clone());
            }
        }
    }
    return *this;
}
