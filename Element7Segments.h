#ifndef ELEMENT_7_SEGMENTS_H
#define ELEMENT_7_SEGMENTS_H

#include "ElementSegmented.h"

#include "FilterBrightness.h"

uint8_t NUMBERS[10][7] = {
    {255, 255, 255, 0, 255, 255, 255},
    {255, 0, 0, 0, 255, 0, 0},
    {255, 255, 0, 255, 0, 255, 255},
    {255, 255, 0, 255, 255, 255, 0},
    {255, 0, 255, 255, 255, 0, 0},
    {0, 255, 255, 255, 255, 255, 0},
    {0, 255, 255, 255, 255, 255, 255},
    {255, 255, 0, 0, 255, 0, 0},
    {255, 255, 255, 255, 255, 255, 255},
    {255, 255, 255, 255, 255, 255, 0}
};

class Element7Segments : public ElementSegmented {
public:
    Element7Segments(CRGB *buffer) : ElementSegmented(buffer, 7, 8) {

    }

    void display(uint8_t digit) {
        digit = digit % 10; // ensure the input is less than 10

        uint8_t *representation = NUMBERS[digit];

        for (int i = 0; i < getSegmentCount(); i++) {
            ElementSegment *elemSegment = getSegmentAt(i);

            uint8_t brightness = representation[i];
            // elemSegment->setColor(CRGB(CHSV(0, 255, brightness)));

            elemSegment->removeAllFilter();
            elemSegment->addFilter(new FilterBrightness(0, brightness / 255.0));
        }
    }
};

#endif // ELEMENT_7_SEGMENTS_H
