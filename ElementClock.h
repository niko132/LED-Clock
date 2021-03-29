#ifndef ELEMENT_CLOCK_H
#define ELEMENT_CLOCK_H

#include "Element.h"

#include "Element7Segments.h"
#include "ElementColon.h"

#include "RTC.h"

class ElementClock : public Element {
private:
    Element *_firstHourElem;
    Element *_secondHourElem;
    Element *_colonElem;
    Element *_firstMinuteElem;
    Element *_secondMinuteElem;

public:
    ElementClock(CRGB *buffer) : Element(232, buffer) { // the number of leds needed for a clock
        _secondMinuteElem = new Element7Segments(8);
        _firstMinuteElem = new Element7Segments(8);
        _colonElem = new ElementColon(4);
        _secondHourElem = new Element7Segments(8);
        _firstHourElem = new Element7Segments(8);

        addChild(_secondMinuteElem);
        addChild(_firstMinuteElem);
        addChild(_colonElem);
        addChild(_secondHourElem);
        addChild(_firstHourElem);
    }

    ~ElementClock() {
        clearChildren();

        if (_firstHourElem) {
            delete _firstHourElem;
            _firstHourElem = NULL;
        }

        if (_secondHourElem) {
            delete _secondHourElem;
            _secondHourElem = NULL;
        }

        if (_colonElem) {
            delete _colonElem;
            _colonElem = NULL;
        }

        if (_firstMinuteElem) {
            delete _firstMinuteElem;
            _firstMinuteElem = NULL;
        }

        if (_secondMinuteElem) {
            delete _secondMinuteElem;
            _secondMinuteElem = NULL;
        }
    }

    void update() {
        int firstHourDigit = RTC.getHours() / 10;
        int secondHourDigit = RTC.getHours() % 10;
        int firstMinuteDigit = RTC.getMinutes() / 10;
        int secondMinuteDigit = RTC.getMinutes() % 10;

        // insert a 0 as a placeholder for the colon
        int data[] = { secondMinuteDigit, firstMinuteDigit, 0, secondHourDigit, firstHourDigit };

        size_t currentIndex = 0;
        for (element_iterator it = childrenBegin(); it != childrenEnd(); it++, currentIndex++) {
            if (currentIndex == 2) {
                continue;
            }

            Element7Segments *elem = (Element7Segments*)*it;
            elem->setDigit(data[currentIndex], CRGB(0, 255, 0));
        }
    }
};

#endif // ELEMENT_CLOCK_H
