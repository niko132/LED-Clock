#ifndef RANDOM_COLOR_H
#define RANDOM_COLOR_H

#include "Effect.h"

class RandomColor : public Effect {
private:
    double _changeSpeed;
    size_t _pixelSize;
    bool _fade;

public:
    RandomColor(JsonObject &root);
    RandomColor();

    void setChangeSpeed(double changeSpeed);
    void setPixelSize(size_t pixelSize);
    void setFading(bool fade);

    CRGB getColor(size_t ledIndex, size_t maxLeds, double x, double y);
};

#endif // RANDOM_COLOR_H
