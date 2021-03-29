#include "ElementColon.h"

#include "ElementDot.h"

ElementColon::ElementColon(size_t ledCountPerSegment, CRGB *buffer) : Element(ledCountPerSegment * 2, buffer) {
    for (int i = 0; i < 2; i++) {
        ElementDot *elem = new ElementDot(ledCountPerSegment);
        addChild(elem);
    }
}

ElementColon::~ElementColon() {
    for (element_iterator it = childrenBegin(); it != childrenEnd(); it++) {
        Element *elem = *it;
        delete elem;
    }

    clearChildren();
}

void ElementColon::indexToCoords(size_t index, double *x, double *y) {
    // TODO: check if this mf is correct

    /*
    double segmentLength = 17.5;
    double segmentLedLength = 11.7;

    double xMid = -segmentLength / 2.0;
    double yMid = 7.5 / 2.0;

    if (index == 0) {
        *x = xMid + segmentLedLength / 7.0 / 2.0;
        *y = -yMid - segmentLedLength / 7.0 / 2.0;
    } else if (index == 1) {
        *x = xMid - segmentLedLength / 7.0 / 2.0;
        *y = -yMid - segmentLedLength / 7.0 / 2.0;
    } else if (index == 2) {
        *x = xMid - segmentLedLength / 7.0 / 2.0;
        *y = -yMid + segmentLedLength / 7.0 / 2.0;
    } else if (index == 3) {
        *x = xMid + segmentLedLength / 7.0 / 2.0;
        *y = -yMid + segmentLedLength / 7.0 / 2.0;
    } else if (index == 4) {
        *x = xMid + segmentLedLength / 7.0 / 2.0;
        *y = yMid - segmentLedLength / 7.0 / 2.0;
    } else if (index == 5) {
        *x = xMid - segmentLedLength / 7.0 / 2.0;
        *y = yMid - segmentLedLength / 7.0 / 2.0;
    } else if (index == 6) {
        *x = xMid - segmentLedLength / 7.0 / 2.0;
        *y = yMid + segmentLedLength / 7.0 / 2.0;
    } else if (index == 7) {
        *x = xMid + segmentLedLength / 7.0 / 2.0;
        *y = yMid + segmentLedLength / 7.0 / 2.0;
    } else {
        *x = 0.0;
        *y = 0.0;
    }
    */

    index = index % 8;
    *x = POSITION_LOOKUP_X[index];
    *y = POSITION_LOOKUP_Y[index];
}

void ElementColon::exit(double *x, double *y) {
    *x = -17.5;
    *y = 0.0;
}
