#ifndef ELEMENT_COLON_H
#define ELEMENT_COLON_H

#include "Element.h"

class ElementColon : public Element {
public:
    float POSITION_LOOKUP_X[56] = {
        -7.91,
		-9.59,
		-9.59,
		-7.91,
		-7.91,
		-9.59,
		-9.59,
		-7.91
    };

    float POSITION_LOOKUP_Y[56] = {
        -4.59,
		-4.59,
		-2.91,
		-2.91,
		2.91,
		2.91,
		4.59,
		4.59
    };

    ElementColon(size_t ledCountPerSegment, CRGB *buffer = NULL);
    ~ElementColon();

    void indexToCoords(size_t index, double *x, double *y);
    void exit(double *x, double *y);
};

#endif // ELEMENT_COLON_H
