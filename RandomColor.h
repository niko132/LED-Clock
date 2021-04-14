#ifndef RANDOM_COLOR_H
#define RANDOM_COLOR_H

#include "Effect.h"

class RandomColor : public Effect {
private:
    double _changeSpeed = 2.0;
    size_t _pixelSize = 1;
    bool _fade = false;

public:
    RandomColor(JsonObject &root);
    RandomColor();

    void fromJson(JsonObject &root);
    void patchJson(JsonObject &root);

    void toJson(JsonObject &root);

    void setChangeSpeed(double changeSpeed);
    void setPixelSize(size_t pixelSize);
    void setFading(bool fade);

    CRGB getColor(size_t ledIndex, size_t maxLeds, double x, double y);
};

#endif // RANDOM_COLOR_H
