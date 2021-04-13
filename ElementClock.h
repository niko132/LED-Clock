#ifndef ELEMENT_CLOCK_H
#define ELEMENT_CLOCK_H

#include "Element.h"

#include "Element7Segments.h"
#include "ElementColon.h"

#include "Espalexa.h"

class ElementClock : public Element {
private:
    Espalexa *_espalexa = NULL;
    EspalexaDevice *_alexaDevice = NULL;

protected:
    virtual void onAlexaChange(EspalexaDevice *device);

public:
    ElementClock(CRGB *buffer, Espalexa *espalexa);
    ElementClock(JsonObject &root);
    ~ElementClock();

    Element7Segments* getFirstHourElement();
    Element7Segments* getSecondHourElement();
    Element7Segments* getFirstMinuteElement();
    Element7Segments* getSecondMinuteElement();

    ElementColon* getColonElement();

    void setAlexaName(String name);

    void indexToCoords(size_t index, double *x, double *y);
    void exitCoords(double *x, double *y);

    void update();
};

#endif // ELEMENT_CLOCK_H
