#include "ElementDot.h"

ElementDot::ElementDot(size_t ledCount, CRGB *buffer) : Element(ledCount, buffer) {

}

void ElementDot::indexToCoords(size_t index, double *x, double *y) {
    if (index == 0) {
        *x = 11.7 / 7.0 / 2.0;
        *y = -11.7 / 7.0 / 2.0;
    } else if (index == 1) {
        *x = -11.7 / 7.0 / 2.0;
        *y = -11.7 / 7.0 / 2.0;
    } else if (index == 2) {
        *x = -11.7 / 7.0 / 2.0;
        *y = 11.7 / 7.0 / 2.0;
    } else if (index == 3) {
        *x = 11.7 / 7.0 / 2.0;
        *y = 11.7 / 7.0 / 2.0;
    } else {
        *x = 0.0;
        *y = 0.0;
    }
}

void ElementDot::exit(double *x, double *y) {
    *x = 0.0;
    *y = 0.0;
}
