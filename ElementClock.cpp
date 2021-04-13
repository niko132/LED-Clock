#include "ElementClock.h"

#include "Element7Segments.h"
#include "ElementColon.h"

#include "RTC.h"

#include "ColorFade.h"
#include "RandomColor.h"

ElementClock::ElementClock(CRGB *buffer, Espalexa *espalexa) : Element("ElementClock", 232, buffer) { // the number of leds needed for a clock
    addChild(new Element7Segments(8));
    addChild(new Element7Segments(8));
    addChild(new ElementColon(4));
    addChild(new Element7Segments(8));
    addChild(new Element7Segments(8));

    _effect = new ColorFade();
    ((ColorFade*)_effect)->setAngleSpeed(2.0);

    _espalexa = espalexa;
    setAlexaName("Regal");
}

ElementClock::ElementClock(JsonObject &root) : Element("ElementClock", root) {
    // do nothing here
}

ElementClock::~ElementClock() {
    // TODO proper handle alexa device deleting - reference at espalexa is still there
    if (_alexaDevice) {
        delete _alexaDevice;
        _alexaDevice = NULL;
    }
}

Element7Segments* ElementClock::getFirstHourElement() {
    return (Element7Segments*)getNthTypeChild("Element7Segments", 3);
}

Element7Segments* ElementClock::getSecondHourElement() {
    return (Element7Segments*)getNthTypeChild("Element7Segments", 2);
}

Element7Segments* ElementClock::getFirstMinuteElement() {
    return (Element7Segments*)getNthTypeChild("Element7Segments", 1);
}

Element7Segments* ElementClock::getSecondMinuteElement() {
    return (Element7Segments*)getNthTypeChild("Element7Segments", 0);
}

ElementColon* ElementClock::getColonElement() {
    return (ElementColon*)getNthTypeChild("ElementColon", 0);
}

void ElementClock::setAlexaName(String name) {
    if (!_alexaDevice) {
        _alexaDevice = new EspalexaDevice(name, std::bind(&ElementClock::onAlexaChange, this, std::placeholders::_1), EspalexaDeviceType::extendedcolor);
        _espalexa->addDevice(_alexaDevice);
    } else if (_alexaDevice->getName() != name) {
        _alexaDevice->setName(name);
    }
}

void ElementClock::onAlexaChange(EspalexaDevice *device) {
    EspalexaDeviceProperty prop = _alexaDevice->getLastChangedProperty();

    if (prop == EspalexaDeviceProperty::on) {
        setBrightness(255);
    } else if (prop == EspalexaDeviceProperty::off) {
        setBrightness(0);
    } else if (prop == EspalexaDeviceProperty::bri) {
        setBrightness(_alexaDevice->getValue());
    } else { // color
        if (_alexaDevice->getColorMode() == EspalexaColorMode::ct) { // shade of white
            uint16_t ct = _alexaDevice->getCt();
            // TODO handle white
        } else { // rgb
            uint32_t color = _alexaDevice->getRGB();
            uint8_t r = (color >> 16) & 0xFF;
            uint8_t g = (color >> 8) & 0xFF;
            uint8_t b = color & 0xFF;
            // TODO handle RGB
        }
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

        converter->entryCoords(&startX, &startY);

        if (index < currentIndex + elem->getLedCount()) {
            converter->indexToCoords(index - currentIndex, &endX, &endY);

            *x = currentX + endX - startX;
            *y = currentY + endY - startY;

            return;
        } else {
            converter->exitCoords(&endX, &endY);
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

void ElementClock::exitCoords(double *x, double *y) {
    *x = -97.9;
    *y = 0.0;
}

void ElementClock::update() {
    if (!_effect) {
        setColor(CRGB(0, 0, 0));
        return; // do nothing when there is no effect
    }

    // current placeholder effect
    double startX, startY, endX, endY;
    double width;

    entryCoords(&startX, &startY);
    exitCoords(&endX, &endY);

    width = startX - endX;

    for (size_t i = 0; i < getLedCount(); i++) {
        double x, y;
        indexToCoords(i, &x, &y);
        x += width / 2.0; // center the animation in the middle

        CRGB color = _effect->getColor(i, getLedCount(), x, y);
        setColorAt(i, color);
    }


    int firstHourDigit = RTC.getHours() / 10;
    int secondHourDigit = RTC.getHours() % 10;
    int firstMinuteDigit = RTC.getMinutes() / 10;
    int secondMinuteDigit = RTC.getMinutes() % 10;

    // insert a 0 as a placeholder for the colon
    int data[] = { secondMinuteDigit, firstMinuteDigit, secondHourDigit, firstHourDigit };

    for (size_t i = 0; i < 4; i++) {
        Element7Segments *elem = (Element7Segments*)getNthTypeChild("Element7Segments", i);
        if (elem) {
            elem->setDigit(data[i]);
        }
    }

    Element::update();
}
