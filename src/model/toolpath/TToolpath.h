//
// Created by gawain on 9/19/25.
//

#ifndef TURNLAB_TTOOLPATH_H
#define TURNLAB_TTOOLPATH_H

#include <nlohmann/json.hpp>

enum class TToolpathType {
    Line
};

NLOHMANN_JSON_SERIALIZE_ENUM(TToolpathType, {
    {TToolpathType::Line, "Line"}
})

inline std::string toString(TToolpathType type) {
    switch (type) {
        case TToolpathType::Line: return "Line";
        default: return "Unknown";
    }
}

class TToolpath {
public:
    int toolNumber = 0;
    double feedRate = 100.0;  // mm/min
    double rpm = 1000.0;
    TToolpathType type;

    TToolpath(TToolpathType type, int toolNumber = 0, double feedRate = 100.0, double rpm = 1000.0)
        : type(type), toolNumber(toolNumber), feedRate(feedRate), rpm(rpm) {}

    virtual ~TToolpath() = default;

    // Pure virtual method for JSON serialization
    virtual nlohmann::json toJson() const = 0;
    virtual void fromJson(const nlohmann::json& j) = 0;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(TToolpath, toolNumber, feedRate, rpm, type)
};

#endif //TURNLAB_TTOOLPATH_H