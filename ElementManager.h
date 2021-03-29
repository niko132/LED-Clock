#ifndef ELEMENT_MANAGER_H
#define ELEMENT_MANAGER_H

#define ELEMENT_7_SEGMENTS 1
#define ELEMENT_COLON 2

#include "Element.h"
#include "Element7Segments.h"
#include "ElementSegment.h"
#include "ElementColon.h"

#include <FastLED.h>
#include <vector>

class ElementManager {
private:
    CRGB *_buffer = NULL;
    size_t _maxLedCount;
    size_t _currentIndex;

    std::vector<Element*> _elements;

public:
    ElementManager(CRGB *buffer, size_t maxLedCount) {
        _buffer = buffer;
        _maxLedCount = maxLedCount;
        _currentIndex = 0;
    }

    Element* addElement(unsigned int type) {
        Element *element = NULL;

        CRGB *currentBuffer = &_buffer[_currentIndex];

        switch (type) {
            case ELEMENT_7_SEGMENTS:
                element = new Element7Segments(currentBuffer);
                break;
            case ELEMENT_COLON:
                element = new ElementColon(currentBuffer);
                break;
        }

        if (element == NULL) {
            return NULL;
        }

        size_t ledCount = element->getLedCount();

        if (_currentIndex + ledCount > _maxLedCount) {
            // we don't have enough leds for this element
            delete element;
            element = NULL;
            return NULL;
        }

        _elements.push_back(element);
        _currentIndex += ledCount;
        return element;
    }

    size_t getElementCount() {
        return _elements.size();
    }

    Element* getElementAt(size_t index) {
        return _elements.at(index);
    }

    void applyFilter() {
        for (int i = 0; i < _elements.size(); i++) {
            Element *elem = _elements.at(i);
            elem->applyFilter();
        }
    }
};

#endif // ELEMENT_MANAGER_H
