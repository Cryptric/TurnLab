//
// Created by gawain on 9/9/25.
//

#ifndef TURNLAB_SEGMENT_H
#define TURNLAB_SEGMENT_H


class Segment {

public:
    virtual ~Segment() = default;
    virtual bool operator==(const Segment& other) const = 0;

};


#endif //TURNLAB_SEGMENT_H