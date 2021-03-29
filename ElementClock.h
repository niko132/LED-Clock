#ifndef ELEMENT_CLOCK_H
#define ELEMENT_CLOCK_H

#include "Element.h"

class ElementClock : public Element {
private:
    Element *_firstHourElem;
    Element *_secondHourElem;
    Element *_colonElem;
    Element *_firstMinuteElem;
    Element *_secondMinuteElem;

public:
    ElementClock(CRGB *buffer);
    ~ElementClock();

    void indexToCoords(size_t index, double *x, double *y);
    void exit(double *x, double *y);

    void update();
};

#endif // ELEMENT_CLOCK_H
