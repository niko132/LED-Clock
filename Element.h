#ifndef ELEMENT_H
#define ELEMENT_H

#include <FastLED.h>

#include <list>

// forward declaration so we can use it in typedef
class Element;

typedef std::list<Element*>::iterator element_iterator;

class Element {
private:
    CRGB *_buffer = NULL;
    size_t _ledCount;

    std::list<Element*> _children;
    size_t _currentChildrenLedIndex;

public:
    Element(size_t ledCount, CRGB *buffer = NULL) {
        _buffer = buffer;
        _ledCount = ledCount;

        _currentChildrenLedIndex = 0;
    }

    size_t getLedCount() {
        return _ledCount;
    }

    void setBuffer(CRGB *buffer) {
        _buffer = buffer;

        // update every child buffer
        _currentChildrenLedIndex = 0;
        for (std::list<Element*>::iterator it = _children.begin(); it != _children.end(); it++) {
            Element *elem = *it;
            elem->setBuffer(&_buffer[_currentChildrenLedIndex]);
            _currentChildrenLedIndex += elem->getLedCount();
        }
    }

    CRGB* getBuffer() {
        return _buffer;
    }

    bool addChild(Element *child) {
        if (_currentChildrenLedIndex + child->getLedCount() > _ledCount) {
            return false;
        }

        if (_buffer) {
            child->setBuffer(&_buffer[_currentChildrenLedIndex]);
        }
        _children.push_back(child);

        _currentChildrenLedIndex += child->getLedCount();

        return true;
    }

    void removeChild(Element *child) {
        _children.remove(child);
    }

    void clearChildren() {
        _children.clear();
    }

    element_iterator childrenBegin() {
        return _children.begin();
    }

    element_iterator childrenEnd() {
        return _children.end();
    }

    void setColor(CRGB color) {
        for (int i = 0; i < getLedCount(); i++) {
            _buffer[i] = color;
        }
    }

    void setColorAt(size_t index, CRGB color) {
        _buffer[index] = color;
    }
};

#endif // ELEMENT_H
