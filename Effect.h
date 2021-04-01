#ifndef EFFECT_H
#define EFFECT_H

#include <FastLED.h>

class Effect {
public:
    virtual CRGB getColor(size_t ledIndex, size_t maxLeds, double x, double y) = 0;
};

#endif // EFFECT_H
