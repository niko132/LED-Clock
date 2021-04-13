#include "RandomColor.h"

#include "Arduino.h"

RandomColor::RandomColor() : Effect("RandomColor") {
    _changeSpeed = 2.0;
    _pixelSize = 1;
    _fade = false;
}

RandomColor::RandomColor(JsonObject &root) : Effect("RandomColor", root) {
    _changeSpeed = 2.0;
    _pixelSize = 1;
    _fade = false;
}

void RandomColor::setChangeSpeed(double changeSpeed) {
    _changeSpeed = changeSpeed;
}

void RandomColor::setPixelSize(size_t pixelSize) {
    _pixelSize = pixelSize;
}

void RandomColor::setFading(bool fade) {
    _fade = fade;
}

CRGB RandomColor::getColor(size_t ledIndex, size_t maxLeds, double x, double y) {
    size_t indexSeed = ledIndex / _pixelSize;

    unsigned long seed = (unsigned long)(millis() * _changeSpeed / 10000.0);
    randomSeed(seed);

    for (int i = 0; i < indexSeed; i++) {
        random(1);
    }

    uint8_t r1 = (uint8_t)random(256);

    uint8_t val;

    if (_fade && _changeSpeed != 0) {
        randomSeed(seed + 1);

        for (int i = 0; i < indexSeed; i++) {
            random(1);
        }

        uint8_t r2 = (uint8_t)random(256);

        unsigned long duration = (unsigned long)(10000.0 / _changeSpeed);
        double frac = (double)(millis() % duration) / duration;
        val = (uint8_t)(r1 + (r2 - r1) * frac);
    } else {
        val = r1;
    }

    return CHSV(val, 255, 255);
}
