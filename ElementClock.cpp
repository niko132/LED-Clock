#include "ElementClock.h"

#include "Element7Segments.h"
#include "ElementColon.h"

#include "RTC.h"

ElementClock::ElementClock(CRGB *buffer) : Element(232, buffer) { // the number of leds needed for a clock
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

ElementClock::~ElementClock() {
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

void ElementClock::indexToCoords(size_t index, double *x, double *y) {
    double currentX = 0.0;
    double currentY = 0.0;

    size_t currentIndex = 0;
    size_t currentElementIndex = 0;

    for (element_iterator it = childrenBegin(); it != childrenEnd(); it++, currentElementIndex++) {
        IndexCoordsConverter *converter = (IndexCoordsConverter*)*it;
        Element *elem = (Element*)*it;

        double startX;
        double startY;
        double endX;
        double endY;

        converter->entry(&startX, &startY);

        if (index < currentIndex + elem->getLedCount()) {
            converter->indexToCoords(index - currentIndex, &endX, &endY);

            *x = currentX + endX - startX;
            *y = currentY + endY - startY;

            return;
        } else {
            converter->exit(&endX, &endY);
            currentX += endX - startX;
            currentY += endY - startY;

            if (currentElementIndex == 1 || currentElementIndex == 2) {
                currentX -= 0.0; // do nothing at the colon
            } else {
                currentX -= 5.2; // the next element begins at 5.2cm offset
            }
        }

        currentIndex += elem->getLedCount();
    }
}

void ElementClock::exit(double *x, double *y) {
    *x = -97.9;
    *y = 0.0;
}

void ElementClock::update() {
    // TODO: update effect

    // current placeholder effect
    double startX, startY, endX, endY;
    double width;

    entry(&startX, &startY);
    exit(&endX, &endY);

    width = startX - endX;

    for (size_t i = 0; i < getLedCount(); i++) {
        double x;
        double y;

        indexToCoords(i, &x, &y);
        x += width / 2.0;

        uint8_t angle = (uint8_t) (millis() / 28.0 / 360.0 * 255.0);

        double val = ((cos8(angle) - 128.0) / 128.0) * x + ((sin8(angle) - 128.0) / 128.0) * y;
        uint8_t hue = (uint8_t) (val / (width * 1.1) * 255.0 + millis() / 390.0);

        /*
        // use threshold to make the effect 2 colored
        if (hue < 128) {
            hue = 0;
        } else {
            hue = 127;
        }
        */

        getBuffer()[i] = CHSV(hue, 255, 255);
    }


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
        elem->setDigit(data[currentIndex]);
    }

    // TODO: update children effect
    updateChildren();
}
