#ifndef ELEMENT_COLON_H
#define ELEMENT_COLON_H

#include "Element.h"

class ElementColon : public Element {
public:
    ElementColon(size_t ledCountPerSegment, CRGB *buffer = NULL) : Element(ledCountPerSegment * 2, buffer) {
        for (int i = 0; i < 2; i++) {
            Element *elem = new Element(ledCountPerSegment);
            addChild(elem);
        }
    }

    ~ElementColon() {
        for (element_iterator it = childrenBegin(); it != childrenEnd(); it++) {
            Element *elem = *it;
            delete elem;
        }

        clearChildren();
    }
};

#endif // ELEMENT_COLON_H
