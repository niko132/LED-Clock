#ifndef ELEMENT_MANAGER_H
#define ELEMENT_MANAGER_H

#include "Element.h"

#include "ElementSegment.h"
#include "ElementDot.h"
#include "ElementColon.h"
#include "Element7Segments.h"
#include "ElementClock.h"

Element* createElement(String type, JsonObject &root) {
    if (String("ElementSegment").equals(type)) {
        return new ElementSegment(root);
    } else if (String("ElementDot").equals(type)) {
        return new ElementDot(root);
    } else if (String("ElementColon").equals(type)) {
        return new ElementColon(root);
    } else if (String("Element7Segments").equals(type)) {
        return new Element7Segments(root);
    } else if (String("ElementClock").equals(type)) {
        return new ElementClock(root);
    }

    return NULL;
}

#endif // ELEMENT_MANAGER_H
