#ifndef JSON_PROCESSOR_H
#define JSON_PROCESSOR_H

#include <ArduinoJson.h>

class JsonProcessor {
public:
    virtual void fromJson(JsonObject &root) = 0;
    virtual void patchJson(JsonObject &root) = 0;
    virtual void toJson(JsonObject &root) = 0;
};

#endif // JSON_PROCESSOR_H
