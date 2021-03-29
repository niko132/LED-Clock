#ifndef ELEMENT_7_SEGMENTS_H
#define ELEMENT_7_SEGMENTS_H

#include "Element.h"

/*
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
*/

uint8_t NUMBER_BM[10] = {
    0b1110111,
    0b0010001,
    0b1101011,
    0b0111011,
    0b0011101,
    0b0111110,
    0b1111110,
    0b0010011,
    0b1111111,
    0b0111111
};

class Element7Segments : public Element {
public:
    Element7Segments(size_t ledCountPerSegment, CRGB *buffer = NULL) : Element(ledCountPerSegment * 7, buffer) {
        for (int i = 0; i < 7; i++) {
            Element *elem = new Element(ledCountPerSegment);
            addChild(elem);
        }
    }

    ~Element7Segments() {
        for (element_iterator it = childrenBegin(); it != childrenEnd(); it++) {
            Element *elem = *it;
            delete elem;
        }

        clearChildren();
    }

    void setDigit(uint8_t digit, CRGB color) {
        digit = digit % 10; // ensure it's a single digit

        size_t currentIndex = 0;
        for (element_iterator it = childrenBegin(); it != childrenEnd(); it++, currentIndex++) {
            bool lit = (NUMBER_BM[digit] >> currentIndex) & 0x1;

            Element *elem = *it;

            if (lit) {
                elem->setColor(color);
            } else {
                elem->setColor(CRGB(0, 0, 0));
            }
        }
    }
};

#endif // ELEMENT_7_SEGMENTS_H
