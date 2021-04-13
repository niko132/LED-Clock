#ifndef EFFECT_MANAGER_H
#define EFFECT_MANAGER_H

#include "Effect.h"

#include "ColorFade.h"
#include "RandomColor.h"

Effect* createEffect(String name, JsonObject &root) {
    if (String("ColorFade").equals(name)) {
        return new ColorFade(root);
    } else if (String("RandomColor").equals(name)) {
        return new RandomColor(root);
    }

    return NULL;
}

#endif // EFFECT_MANAGER_H
