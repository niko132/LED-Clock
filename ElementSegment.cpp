#include "ElementSegment.h"

ElementSegment::ElementSegment(size_t ledCount, CRGB *buffer) : Element("ElementSegment", ledCount, buffer) {

}

ElementSegment::ElementSegment(JsonObject &root) : Element("ElementSegment", root) {

}

void ElementSegment::indexToCoords(size_t index, double *x, double *y) {
    *x = 3.0 + index * 11.7 / 7.0;
    *y = 0.0;
}

void ElementSegment::exitCoords(double *x, double *y) {
    *x = 17.7;
    *y = 0.0;
}
