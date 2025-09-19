//
// Created by gawain on 9/19/25.
//

#ifndef TURNLAB_TTOOLPATHSEQUENCE_H
#define TURNLAB_TTOOLPATHSEQUENCE_H

#include <vector>
#include <memory>
#include <nlohmann/json.hpp>
#include "TToolpath.h"
#include "TLine.h"

class TToolpathSequence {
public:
    std::vector<std::unique_ptr<TToolpath>> toolpaths;

    TToolpathSequence() = default;
    TToolpathSequence(std::vector<std::unique_ptr<TToolpath>> toolpaths) : toolpaths(std::move(toolpaths)) {}

    // Move constructor
    TToolpathSequence(TToolpathSequence&& other) noexcept
        : toolpaths(std::move(other.toolpaths)) {}

    // Move assignment operator
    TToolpathSequence& operator=(TToolpathSequence&& other) noexcept {
        if (this != &other) {
            toolpaths = std::move(other.toolpaths);
        }
        return *this;
    }

    // Delete copy constructor and copy assignment operator
    TToolpathSequence(const TToolpathSequence&) = delete;
    TToolpathSequence& operator=(const TToolpathSequence&) = delete;

    void addToolpath(std::unique_ptr<TToolpath> toolpath) {
        toolpaths.push_back(std::move(toolpath));
    }

    void addLine(const TPoint& start, const TPoint& end, int toolNumber = 0, double feedRate = 100.0, double rpm = 1000.0) {
        auto line = std::make_unique<TLine>(start, end, toolNumber, feedRate, rpm);
        addToolpath(std::move(line));
    }

    void addLine(double startX, double startZ, double endX, double endZ, int toolNumber = 0, double feedRate = 100.0, double rpm = 1000.0) {
        auto line = std::make_unique<TLine>(startX, startZ, endX, endZ, toolNumber, feedRate, rpm);
        addToolpath(std::move(line));
    }

    size_t size() const {
        return toolpaths.size();
    }

    bool empty() const {
        return toolpaths.empty();
    }

    void clear() {
        toolpaths.clear();
    }

    // JSON serialization
    nlohmann::json toJson() const {
        nlohmann::json j = nlohmann::json::array();
        for (const auto& toolpath : toolpaths) {
            j.push_back(toolpath->toJson());
        }
        return j;
    }

    void fromJson(const nlohmann::json& j) {
        clear();
        for (const auto& item : j) {
            TToolpathType type = item.at("type").get<TToolpathType>();

            switch (type) {
                case TToolpathType::Line: {
                    auto line = std::make_unique<TLine>();
                    line->fromJson(item);
                    addToolpath(std::move(line));
                    break;
                }
                default:
                    // Skip unknown types
                    break;
            }
        }
    }
};

// Custom JSON serialization for TToolpathSequence
inline void to_json(nlohmann::json& j, const TToolpathSequence& sequence) {
    j = sequence.toJson();
}

inline void from_json(const nlohmann::json& j, TToolpathSequence& sequence) {
    sequence.fromJson(j);
}

#endif //TURNLAB_TTOOLPATHSEQUENCE_H