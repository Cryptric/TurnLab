//
// Created by gawain on 9/11/25.
//

#include <memory>

#include "Geometry.h"

Geometry::Geometry(std::vector<std::unique_ptr<Segment>>& segments) {
    this->segments = std::move(segments);
}
