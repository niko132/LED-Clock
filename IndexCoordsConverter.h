#ifndef INDEX_COORDS_CONVERTER_H
#define INDEX_COORDS_CONVERTER_H

#include <stddef.h>

class IndexCoordsConverter {
public:
    virtual void indexToCoords(size_t index, double *x, double *y) = 0;
    virtual void entryCoords(double *x, double *y);
    virtual void exitCoords(double *x, double *y) = 0;
};

#endif // INDEX_COORDS_CONVERTER_H
