#include "ColorFade.h"

ColorFade::ColorFade(double angle, double scale) : Effect("ColorFade") {
    _angle = angle;
    _scale = scale;
    _angleSpeed = 0.0;
    _shiftSpeed = 0.0;
}

ColorFade::ColorFade(JsonObject &root) : Effect("ColorFade") {
    fromJson(root);
}

void ColorFade::fromJson(JsonObject &root) {
    Effect::fromJson(root);

    _angle = root["angle"] | _angle;
    _scale = root["scale"] | _scale;
    _angleSpeed = root["angleSpeed"] | _angleSpeed;
    _shiftSpeed = root["shiftSpeed"] | _shiftSpeed;
}

void ColorFade::patchJson(JsonObject &root) {
    Effect::patchJson(root);

    patchProperty(root, "angle", &_angle);
    patchProperty(root, "scale", &_scale);
    patchProperty(root, "angleSpeed", &_angleSpeed);
    patchProperty(root, "shiftSpeed", &_shiftSpeed);
}

void ColorFade::toJson(JsonObject &root) {
    Effect::toJson(root);

    root["angle"] = _angle;
    root["scale"] = _scale;
    root["angleSpeed"] = _angleSpeed;
    root["shiftSpeed"] = _shiftSpeed;
}

void ColorFade::setAngleSpeed(double angleSpeed) {
    _angleSpeed = angleSpeed; // 10... 1 Rev/Sec... 360° / Sec... 360° / 1000ms
}

void ColorFade::setShiftSpeed(double shiftSpeed) {
    _shiftSpeed = shiftSpeed;
}

CRGB ColorFade::getColor(size_t ledIndex, size_t maxLeds, double x, double y) {
    uint8_t angle = (uint8_t)(_angle / 360.0 * 255.0);

    angle += (uint8_t)(millis() * _angleSpeed / 10000.0 * 255.0);
    // angle += (uint8_t) (millis() / 28.0 / 360.0 * 255.0);

    double val = ((cos8(angle) - 128.0) / 128.0) * x + ((sin8(angle) - 128.0) / 128.0) * y;
    uint8_t hue = (uint8_t) (val / (100.0 * _scale) * 255.0); // complete spectrum on 100cm * scale

    hue -= (uint8_t)(millis() * _shiftSpeed / 10000.0 * 255);
    // hue -= (uint8_t)(millis() / 390.0);

    return CHSV(hue, 255, 255);
}
