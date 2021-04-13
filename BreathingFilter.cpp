#include "BreathingFilter.h"

BreathingFilter::BreathingFilter(uint8_t from, uint8_t to, unsigned long period, unsigned long timeout) : Filter("BreathingFilter", timeout) {
    _from = from;
    _to = to;
    _period = period;
}

BreathingFilter::BreathingFilter(JsonObject &root) : Filter("BreathingFilter", root) {
    
}

void BreathingFilter::fromJson(JsonObject &root) {
    Filter::fromJson(root);

    _from = root["from"] | _from;
    _to = root["to"] | _to;
    _period = root["period"] | _period;
}

void BreathingFilter::toJson(JsonObject &root) {
    Filter::toJson(root);

    root["from"] = _from;
    root["to"] = _to;
    root["period"] = _period;
}

CRGB BreathingFilter::apply(CRGB color) {
    uint8_t val = (uint8_t)((float)(millis() - _startMillis) / _period * 255.0f);
    uint8_t brightness = (uint8_t)(_from + (cos8(val) / 255.0f) * (_to - _from));

    return color % brightness;
}
