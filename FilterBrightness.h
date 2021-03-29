#ifndef FILTER_BRIGHTNESS_H
#define FILTER_BRIGHTNESS_H

#include "Filter.h"

class FilterBrightness : public Filter {
private:
    uint8_t _brightness;

public:
    FilterBrightness(unsigned long timeout, double frac) : Filter(timeout) {
        _brightness = 255 * frac;
    }

    CRGB apply(CRGB color) {
        color %= _brightness;
        return color;
    }
};

#endif FILTER_BRIGHTNESS_H
