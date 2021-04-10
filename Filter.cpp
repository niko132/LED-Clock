#include "Filter.h"

Filter::Filter(unsigned long timeout) {
    _timeout = timeout;
    _startMillis = millis();
}

bool Filter::finished() {
    return _timeout != 0 && (millis() - _startMillis) > _timeout;
}
