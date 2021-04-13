#ifndef ELEMENT_H
#define ELEMENT_H

#include "IndexCoordsConverter.h"
#include "JsonProcessor.h"
#include "Filter.h"
#include "Effect.h"

#include <FastLED.h>

#include <list>

// forward declaration so we can use it in typedef
class Element;

typedef std::list<Element*>::iterator element_iterator;
typedef std::list<Filter*>::iterator filter_iterator;

class Element : public IndexCoordsConverter, public JsonProcessor {
private:
    String _type;
    CRGB *_buffer = NULL;
    size_t _ledCount;

    std::list<Element*> _children;
    size_t _currentChildrenLedIndex;

    std::list<Filter*> _filters;

protected:
    Effect *_effect = NULL;

public:
    Element(String type, size_t ledCount, CRGB *buffer = NULL);
    Element(String type, JsonObject &root);
    ~Element();

    virtual void fromJson(JsonObject &root);
    virtual void patchJson(JsonObject &root);
    virtual void toJson(JsonObject &root);

    size_t getLedCount();
    String getType();

    void setBuffer(CRGB *buffer);
    CRGB* getBuffer();

    bool addChild(Element *child);
    void removeChild(Element *child);
    void clearChildren();

    Element* getNthTypeChild(String type, size_t n);

    element_iterator childrenBegin();
    element_iterator childrenEnd();

    void setBrightness(uint8_t brightness);

    void setColor(CRGB color);
    void setColorAt(size_t index, CRGB color);

    void addFilter(Filter *filter);
    void removeAllFilters();
    void applyFilter();

    virtual void indexToCoords(size_t index, double *x, double *y) = 0;
    virtual void exitCoords(double *x, double *y) = 0;

    virtual void update();
    void updateChildren();
};

#endif // ELEMENT_H
