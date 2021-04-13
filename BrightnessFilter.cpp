#include "BrightnessFilter.h"

BrightnessFilter::BrightnessFilter(uint8_t from, uint8_t to, unsigned long duration, unsigned long timeout) : Filter("BrightnessFilter", timeout) {
    _from = from;
    _to = to;
    _duration = duration;
}

BrightnessFilter::BrightnessFilter(JsonObject &root) : Filter("BrightnessFilter", root) {
    
}

void BrightnessFilter::fromJson(JsonObject &root) {
    Filter::fromJson(root);

    _from = root["from"] | _from;
    _to = root["to"] | _to;
    _duration = root["duration"] | _duration;
}

void BrightnessFilter::toJson(JsonObject &root) {
    Filter::toJson(root);

    root["from"] = _from;
    root["to"] = _to;
    root["duration"] = _duration;
}

void BrightnessFilter::setMaxBrightness(uint8_t max) {
    _from = _to;
    _to = max;
    _startMillis = millis();
}

CRGB BrightnessFilter::apply(CRGB color) {
    float frac;

    if (_duration == 0) {
        frac = 1.0; // instant end value
    } else {
        frac = (float)(millis() - _startMillis) / _duration;
        if (frac > 1.0) {
            frac = 1.0;
        }
    }

    uint8_t brightness = (uint8_t)(_from + (_to - _from) * frac);

    return color % brightness;
}
