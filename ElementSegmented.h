#ifndef ELEMENT_SEGMENTED_H
#define ELEMENT_SEGMENTED_H

#include "Element.h"
#include "ElementSegment.h"

class ElementSegmented : public Element {
protected:
    std::vector<ElementSegment*> _segments;

public:
    ElementSegmented(CRGB *buffer, size_t segmentCount, size_t ledsPerSegment) : Element(buffer) {
        for (int i = 0; i < segmentCount; i++) {
            ElementSegment *segment = new ElementSegment(&buffer[i * ledsPerSegment], ledsPerSegment);
            _segments.push_back(segment);
        }
    }

    size_t getLedCount() {
        size_t ledCount = 0;

        for (int i = 0; i < _segments.size(); i++) {
            ElementSegment *segment = _segments.at(i);
            ledCount += segment->getLedCount();
        }

        return ledCount;
    }

    size_t getSegmentCount() {
        return _segments.size();
    }

    ElementSegment* getSegmentAt(size_t index) {
        return _segments.at(index);
    }

    void applyFilter() {
        // first apply own segment filters
        for (int i = 0; i < _segments.size(); i++) {
            ElementSegment *elemSegment = _segments.at(i);
            elemSegment->applyFilter();
        }

        applyFilterImpl();
    }
};

#endif // ELEMENT_SEGMENTED_H
