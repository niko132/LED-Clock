#ifndef FILTER_LIGHT_UP_H
#define FILTER_LIGHT_UP_H

#include "Filter.h"

class FilterLightUp : public Filter {
private:
    unsigned long _timeout;
    unsigned long _startMillis;

public:
    FilterLightUp(unsigned long timeout) : Filter(timeout) {
        _timeout = timeout;
        _startMillis = millis();
    }

    CRGB apply(CRGB color) {
        double frac = (double)(millis() - _startMillis) / _timeout;
        if (frac > 1.0) {
            frac = 1.0;
        }

        uint8_t brightness = 255 * frac;

        color %= brightness;
        return color;
    }
};

#endif FILTER_LIGHT_UP_H
