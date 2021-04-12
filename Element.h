#ifndef ELEMENT_H
#define ELEMENT_H

#include "IndexCoordsConverter.h"
#include "Filter.h"

#include <FastLED.h>

#include <list>

// forward declaration so we can use it in typedef
class Element;

typedef std::list<Element*>::iterator element_iterator;
typedef std::list<Filter*>::iterator filter_iterator;

class Element : public IndexCoordsConverter {
private:
    CRGB *_buffer = NULL;
    size_t _ledCount;

    std::list<Element*> _children;
    size_t _currentChildrenLedIndex;

    std::list<Filter*> _filters;

public:
    Element(size_t ledCount, CRGB *buffer = NULL);

    size_t getLedCount();

    void setBuffer(CRGB *buffer);
    CRGB* getBuffer();

    bool addChild(Element *child);
    void removeChild(Element *child);
    void clearChildren();

    element_iterator childrenBegin();
    element_iterator childrenEnd();

    void setColor(CRGB color);
    void setColorAt(size_t index, CRGB color);

    void addFilter(Filter *filter);
    void removeAllFilters();
    void applyFilter();

    virtual void indexToCoords(size_t index, double *x, double *y) = 0;
    virtual void exit(double *x, double *y) = 0;

    virtual void update();
    void updateChildren();
};

#endif // ELEMENT_H
