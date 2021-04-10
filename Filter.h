#ifndef FILTER_H
#define FILTER_H

#include <FastLED.h>

class Filter {
protected:
    unsigned long _startMillis;
    unsigned long _timeout;

public:
    Filter(unsigned long timeout);

    bool finished();
    virtual CRGB apply(CRGB color) = 0;
};

#endif // FILTER_H
