#include "BrightnessFilter.h"

BrightnessFilter::BrightnessFilter(uint8_t from, uint8_t to, unsigned long duration, unsigned long timeout) : Filter(timeout) {
    _from = from;
    _to = to;
    _duration = duration;
}

CRGB BrightnessFilter::apply(CRGB color) {
    float frac = (float)(millis() - _startMillis) / _duration;
    if (frac > 1.0) {
        frac = 1.0;
    }

    uint8_t brightness = (uint8_t)(_from + (_to - _from) * frac);

    return color % brightness;
}
