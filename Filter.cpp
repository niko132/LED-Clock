#include "Filter.h"

Filter::Filter(String type, unsigned long timeout) {
    _type = type;
    _timeout = timeout;
    _startMillis = millis();
}

void Filter::fromJson(JsonObject &root) {
    _timeout = root["timeout"] | _timeout;
    _startMillis = millis() - (root["diffMillis"] | 0L);
}

void Filter::patchJson(JsonObject &root) {
    JsonVariant timeoutVariant = root["timeout"];
    if (timeoutVariant) {
        _timeout = timeoutVariant.as<unsigned long>();
    }

    JsonVariant diffMillisVariant = root["diffMillis"];
    if (diffMillisVariant) {
        _startMillis = millis() - diffMillisVariant.as<unsigned long>();
    }
}

void Filter::toJson(JsonObject &root) {
    root["type"] = _type;
    root["timeout"] = _timeout;
    root["diffMillis"] = millis() - _startMillis; // store the difference to make it relative
}

unsigned long Filter::getTimeout() {
    return _timeout;
}

bool Filter::finished() {
    return _timeout != 0 && (millis() - _startMillis) > _timeout;
}
