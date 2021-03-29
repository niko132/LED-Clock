#ifndef FILTER_H
#define FILTER_H

class Filter {
private:
    unsigned long _startMillis;
    unsigned long _timeout;

public:
    Filter(unsigned long timeout) {
        _timeout = timeout;
        _startMillis = millis();
    }

    bool finished() {
        return _timeout != 0 && (millis() - _startMillis) > _timeout;
    }

    virtual CRGB apply(CRGB color) = 0;
};

#endif // FILTER_H
