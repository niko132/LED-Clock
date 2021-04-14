#ifndef JSON_PROCESSOR_H
#define JSON_PROCESSOR_H

#include <ArduinoJson.h>

class JsonProcessor {
public:
    virtual void fromJson(JsonObject &root) = 0;
    virtual void patchJson(JsonObject &root) = 0;
    virtual void toJson(JsonObject &root) = 0;

    template <typename T>
    void patchProperty(JsonObject &root, String key, T *p) {
        JsonVariant variant = root[key];
        // doesn't work with 0 values -> fix it!!!
        if (variant) {
            *p = variant.as<T>();
        }
    }
};

#endif // JSON_PROCESSOR_H
