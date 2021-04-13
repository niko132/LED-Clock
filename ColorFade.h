#ifndef COLOR_FADE_H
#define COLOR_FADE_H

#include "Effect.h"

class ColorFade : public Effect {
private:
    double _angle;
    double _scale;
    double _angleSpeed;
    double _shiftSpeed;

public:
    ColorFade(double angle = 0.0, double scale = 1.0);
    ColorFade(JsonObject &root);

    void fromJson(JsonObject &root);
    void toJson(JsonObject &root);

    void setAngleSpeed(double angleSpeed);
    void setShiftSpeed(double shiftSpeed);

    CRGB getColor(size_t ledIndex, size_t maxLeds, double x, double y);
};

#endif // COLOR_FADE_H
