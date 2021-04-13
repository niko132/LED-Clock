#ifndef FILTER_H
#define FILTER_H

#include "JsonProcessor.h"

#include <FastLED.h>

class Filter : public JsonProcessor {
protected:
    String _type;
    unsigned long _startMillis;
    unsigned long _timeout;

public:
    Filter(String type, unsigned long timeout);

    virtual void fromJson(JsonObject &root);
    virtual void patchJson(JsonObject &root);
    virtual void toJson(JsonObject &root);

    unsigned long getTimeout();
    bool finished();
    virtual CRGB apply(CRGB color) = 0;
};

#endif // FILTER_H
