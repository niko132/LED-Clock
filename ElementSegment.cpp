#include "ElementSegment.h"

ElementSegment::ElementSegment(size_t ledCount, CRGB *buffer) : Element(ledCount, buffer) {

}

void ElementSegment::indexToCoords(size_t index, double *x, double *y) {
    *x = 3.0 + index * 11.7 / 7.0;
    *y = 0.0;
}

void ElementSegment::exit(double *x, double *y) {
    *x = 17.7;
    *y = 0.0;
}
