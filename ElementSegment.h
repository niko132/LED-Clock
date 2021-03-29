#ifndef ELEMENT_SEGMENT_H
#define ELEMENT_SEGMENT_H

#include "Element.h"

class ElementSegment : public Element {
public:
    ElementSegment(size_t ledCount, CRGB *buffer = NULL);

    void indexToCoords(size_t index, double *x, double *y);
    void exit(double *x, double *y);
};

#endif // ELEMENT_SEGMENT_H
