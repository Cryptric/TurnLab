//
// Created by gawain on 9/9/25.
//

#include "Line.h"

bool Line::operator==(const Segment &other) const {
    if (auto *otherLine = dynamic_cast<const Line*>(&other)) {
        return (p1 == otherLine->p1 && p2 == otherLine->p2) || (p1 == otherLine->p2 && p2 == otherLine->p1);
    }
    return false;
}
