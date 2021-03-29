#ifndef ELEMENT_COLON_H
#define ELEMENT_COLON_H

#include "ElementSegmented.h"

class ElementColon : public ElementSegmented {
public:
    ElementColon(CRGB *buffer) : ElementSegmented(buffer, 2, 4) {

    }
};

#endif // ELEMENT_COLON_H
