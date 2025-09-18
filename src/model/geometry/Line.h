//
// Created by gawain on 9/9/25.
//

#ifndef TURNLAB_LINE_H
#define TURNLAB_LINE_H

#include <memory>
#include <vector>
#include <nlohmann/json.hpp>

#include "Point.h"
#include "Segment.h"
#include "Transform.h"

class Line : public Segment {
public:
    Point p1;
    Point p2;

    Line() : p1(), p2() {}
    Line(const Point& start, const Point& end) : p1(start), p2(end) {}
    Line(double x1, double y1, double x2, double y2) : p1(x1, y1), p2(x2, y2) {}
    ~Line() override = default;

    double distance(const Point& point) const override;
    bool operator==(const Segment& other) const override;
    bool operator!=(const Segment& other) const;
    std::unique_ptr<Segment> transform(const std::vector<Transform>& transformations) override;
    std::unique_ptr<Segment> clone() const override;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Line, p1, p2)
};

#endif //TURNLAB_LINE_H