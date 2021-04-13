#ifndef BRIGHTNESS_FILTER_H
#define BRIGHTNESS_FILTER_H

#include "Filter.h"

class BrightnessFilter : public Filter {
private:
    uint8_t _from;
    uint8_t _to;
    unsigned long _duration;

public:
    BrightnessFilter(uint8_t from, uint8_t to, unsigned long duration, unsigned long timeout = 0);
    BrightnessFilter(JsonObject &root);

    void fromJson(JsonObject &root);
    void toJson(JsonObject &root);

    void setMaxBrightness(uint8_t max);

    CRGB apply(CRGB color);
};

#endif // BRIGHTNESS_FILTER_H
