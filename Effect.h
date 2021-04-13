#ifndef EFFECT_H
#define EFFECT_H

#include "JsonProcessor.h"

#include <FastLED.h>

class Effect : public JsonProcessor {
private:
    String _name;

public:
    Effect(String name, JsonObject &root) {
        _name = name;
        fromJson(root);
    }

    Effect(String name) {
        _name = name;
    }

    String getName() {
        return _name;
    }

    virtual CRGB getColor(size_t ledIndex, size_t maxLeds, double x, double y) = 0;

    virtual void fromJson(JsonObject &root) {};
    virtual void patchJson(JsonObject &root) {};
    virtual void toJson(JsonObject &root) {
        root["name"] = _name;
    };
};

#endif // EFFECT_H
