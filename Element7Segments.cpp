#include "Element7Segments.h"

#include "ElementSegment.h"
#include "BrightnessFilter.h"

Element7Segments::Element7Segments(size_t ledCountPerSegment, CRGB *buffer) : Element(ledCountPerSegment * 7, buffer) {
    for (int i = 0; i < 7; i++) {
        ElementSegment *elem = new ElementSegment(ledCountPerSegment);
        addChild(elem);
    }
}

Element7Segments::~Element7Segments() {
    for (element_iterator it = childrenBegin(); it != childrenEnd(); it++) {
        Element *elem = *it;
        delete elem;
    }

    clearChildren();
}

void Element7Segments::indexToCoords(size_t index, double *x, double *y) {
    // TODO: check if this mf is correct

    /*
    double segmentLength = 17.5;
    double segmentLedLength = 11.7;

    double offset = (segmentLength - segmentLedLength) / 2.0;

    if (index < 8) {
        *x = 0.0;
        *y = -offset - index * segmentLedLength / 7.0;
    } else if (index < 16) {
        *x = -offset - (index - 8) * segmentLedLength / 7.0;
        *y = -segmentLength;
    } else if (index < 24) {
        *x = -segmentLength;
        *y = -segmentLength + offset + (index - 16) * segmentLedLength / 7.0;
    } else if (index < 32) {
        *x = -segmentLength + offset + (index - 24) * segmentLedLength / 7.0;
        *y = 0.0;
    } else if (index < 40) {
        *x = 0.0;
        *y = offset + (index - 32) * segmentLedLength / 7.0;
    } else if (index < 48) {
        *x = -offset - (index - 40) * segmentLedLength / 7.0;
        *y = segmentLength;
    } else if (index < 56) {
        *x = -segmentLength;
        *y = segmentLength - offset - (index - 48) * segmentLedLength / 7.0;
    } else {
        *x = 0.0;
        *y = 0.0;
    }
    */

    index = index % 56; // fit in the lookup table
    *x = POSITION_LOOKUP_X[index];
    *y = POSITION_LOOKUP_Y[index];
}

void Element7Segments::exit(double *x, double *y) {
    *x = -17.5;
    *y = 0.0;
}

void Element7Segments::setDigit(uint8_t digit) {
    digit = digit % 10; // ensure it's a single digit

    if (_currentDigit == digit) {
        return; // do nothing
    }

    /*
    size_t currentIndex = 0;
    for (element_iterator it = childrenBegin(); it != childrenEnd(); it++, currentIndex++) {
        bool lit = (NUMBER_BM[digit] >> currentIndex) & 0x1;

        Element *elem = *it;

        if (!lit) {
            elem->setColor(CRGB(0, 0, 0));
        } else {
            // do nothing to show the current effect
        }
    }
    */

    // TODO
    // for every child
    // remove every filter
    // add filter if needed

    size_t currentIndex = 0;
    for (element_iterator it = childrenBegin(); it != childrenEnd(); it++, currentIndex++) {
        bool fromLit = (NUMBER_BM[_currentDigit] >> currentIndex) & 0x1;
        bool toLit = (NUMBER_BM[digit] >> currentIndex) & 0x1;

        Element *elem = *it;

        // only update the filters when needed
        // used to improve performance - maybe delete it later
        if (fromLit != toLit) {
            elem->removeAllFilters();
            // add a new animation filter to fade from old to new segment brightness in 500ms
            elem->addFilter(new BrightnessFilter((uint8_t)(fromLit * 255), (uint8_t)(toLit * 255), 500));
        }
    }

    _currentDigit = digit;
}
