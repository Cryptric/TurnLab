//
// Created by gawain on 9/19/25.
//

#ifndef TURNLAB_TLINE_H
#define TURNLAB_TLINE_H

#include "TToolpath.h"
#include "TPoint.h"

class TLine : public TToolpath {
public:
    TPoint start;
    TPoint end;

    TLine() : TToolpath(TToolpathType::Line) {}

    TLine(const TPoint& start, const TPoint& end, int toolNumber = 0, double feedRate = 100.0, double rpm = 1000.0)
        : TToolpath(TToolpathType::Line, toolNumber, feedRate, rpm), start(start), end(end) {}

    TLine(double startX, double startZ, double endX, double endZ, int toolNumber = 0, double feedRate = 100.0, double rpm = 1000.0)
        : TToolpath(TToolpathType::Line, toolNumber, feedRate, rpm), start(startX, startZ), end(endX, endZ) {}

    nlohmann::json toJson() const override {
        nlohmann::json j;
        j["toolNumber"] = toolNumber;
        j["feedRate"] = feedRate;
        j["rpm"] = rpm;
        j["type"] = type;
        j["start"] = start;
        j["end"] = end;
        return j;
    }

    void fromJson(const nlohmann::json& j) override {
        j.at("toolNumber").get_to(toolNumber);
        j.at("feedRate").get_to(feedRate);
        j.at("rpm").get_to(rpm);
        j.at("type").get_to(type);
        j.at("start").get_to(start);
        j.at("end").get_to(end);
    }

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(TLine, toolNumber, feedRate, rpm, type, start, end)
};

#endif //TURNLAB_TLINE_H