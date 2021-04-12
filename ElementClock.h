#ifndef ELEMENT_CLOCK_H
#define ELEMENT_CLOCK_H

#include "Element.h"

#include "Espalexa.h"

class ElementClock : public Element {
private:
    Element *_firstHourElem;
    Element *_secondHourElem;
    Element *_colonElem;
    Element *_firstMinuteElem;
    Element *_secondMinuteElem;

    Espalexa *_espalexa = NULL;
    EspalexaDevice *_alexaDevice = NULL;

protected:
    virtual void onAlexaChange(EspalexaDevice *device);

public:
    ElementClock(CRGB *buffer, Espalexa *espalexa);
    ~ElementClock();

    void setAlexaName(String name);

    void indexToCoords(size_t index, double *x, double *y);
    void exit(double *x, double *y);

    void update();
};

#endif // ELEMENT_CLOCK_H
