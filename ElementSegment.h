#ifndef ELEMENT_SEGMENT_H
#define ELEMENT_SEGMENT_H

#include "Element.h"

class ElementSegment : public Element {
private:
    size_t _ledCount;

public:
    ElementSegment(CRGB *buffer, size_t ledCount = 8) : Element(buffer) {
        _ledCount = ledCount;
    }

    size_t getLedCount() {
        return _ledCount;
    }
};

#endif // ELEMENT_SEGMENT_H
