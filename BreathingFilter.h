#ifndef BREATHING_FILTER_H
#define BREATHING_FILTER_H

#include "Filter.h"

class BreathingFilter : public Filter {
private:
    uint8_t _from;
    uint8_t _to;
    unsigned long _period;

public:
    BreathingFilter(uint8_t from, uint8_t to, unsigned long period, unsigned long timeout = 0);
    BreathingFilter(JsonObject &root);

    void fromJson(JsonObject &root);
    void toJson(JsonObject &root);

    CRGB apply(CRGB color);
};

#endif // BREATHING_FILTER_H
