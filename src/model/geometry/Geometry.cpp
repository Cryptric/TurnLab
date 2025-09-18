//
// Created by gawain on 9/11/25.
//

#include <memory>

#include "Geometry.h"
#include "Line.h"

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

Geometry Geometry::transform(std::vector<Transform> transformations) {
    Geometry transformedGeometry;
    for (const auto& segment : segments) {
        transformedGeometry.segments.push_back(segment->transform(transformations));
    }
    return transformedGeometry;
}

// JSON serialization for Geometry
void to_json(nlohmann::json& j, const Geometry& g) {
    j = nlohmann::json::array();

    for (const auto& segment : g.segments) {
        if (!segment) continue;

        // Check the type of segment and serialize accordingly
        if (auto* line = dynamic_cast<const Line*>(segment.get())) {
            nlohmann::json segmentJson;
            segmentJson["type"] = "Line";
            segmentJson["data"] = *line;
            j.push_back(segmentJson);
        }
        // Add more segment types here as they're implemented
    }
}

void from_json(const nlohmann::json& j, Geometry& g) {
    g.segments.clear();

    if (!j.is_array()) return;

    for (const auto& segmentJson : j) {
        if (!segmentJson.contains("type") || !segmentJson.contains("data")) continue;

        std::string type = segmentJson["type"];

        if (type == "Line") {
            auto line = std::make_unique<Line>();
            *line = segmentJson["data"].get<Line>();
            g.segments.push_back(std::move(line));
        }
        // Add more segment types here as they're implemented
    }
}
