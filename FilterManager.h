#ifndef FILTER_MANAGER_H
#define FILTER_MANAGER_H

#include "Filter.h"

#include "BrightnessFilter.h"
#include "BreathingFilter.h"

Filter* createFilter(String type, JsonObject &root) {
    if (String("BrightnessFilter").equals(type)) {
        return new BrightnessFilter(root);
    } else if (String("BreathingFilter").equals(type)) {
        return new BreathingFilter(root);
    }

    return NULL;
}

#endif // FILTER_MANAGER_H
