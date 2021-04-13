#ifndef ELEMENT_DOT_H
#define ELEMENT_DOT_H

#include "Element.h"

class ElementDot : public Element {
public:
    ElementDot(size_t ledCount, CRGB *buffer = NULL);
    ElementDot(JsonObject &root);

    void indexToCoords(size_t index, double *x, double *y);
    void exitCoords(double *x, double *y);
};

#endif // ELEMENT_DOT_H
