//
// Created by gawain on 9/9/25.
//

#include <spdlog/spdlog.h>
#include <dime/Model.h>
#include <dime/Input.h>
#include <dime/entities/Entity.h>
#include <dime/entities/Line.h>

#include "DXFUtils.h"

#include "Line.h"
#include "Point.h"
#include "Geometry.h"
#include "Segment.h"

Geometry loadDXF(const std::filesystem::path &path) {
    spdlog::info("Loading DXF file: {}", path.string());
    dimeModel model;
    dimeInput input;

    if (!input.setFile(path.c_str())) {
        spdlog::error("Failed to load DXF file: {}", path.string());
        throw std::runtime_error("Failed to load DXF file");
    }

    if (!model.read(&input)) {
        spdlog::error("Failed to read DXF file: {}", path.string());
        throw std::runtime_error("Failed to read DXF file");
    }
    spdlog::debug("DXF file loaded successfully");

    std::vector<std::unique_ptr<Segment>> segments;

    dimeCallback callback = [](const dimeState *state, dimeEntity *entity, void *userdata) {
        auto *segmentList = static_cast<std::vector<std::unique_ptr<Segment>>*>(userdata);
        spdlog::trace("Traversing entity: {}", entity->getEntityName());
        if (const auto* line = dynamic_cast<dimeLine*>(entity)) {
            const Point p1 = Point(line->getCoords(0).x, line->getCoords(0).y);
            const Point p2 = Point(line->getCoords(1).x, line->getCoords(1).y);
            Line l(p1, p2);
            segmentList->push_back(std::make_unique<Line>(l));
        }
        return true;
    };

    model.traverseEntities(callback, &segments);

    spdlog::debug("DXF file traversed successfully");

    return Geometry(segments);
}
