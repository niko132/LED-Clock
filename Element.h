#ifndef ELEMENT_H
#define ELEMENT_H

#include "Filter.h"

#include <FastLED.h>

#include <vector>
#include <algorithm>

class Element {
private:
    CRGB *_buffer = NULL;
    std::vector<Filter*> _filters;

protected:
    void applyFilterImpl() {
        for (int i = 0; i < getLedCount(); i++) {
            CRGB color = _buffer[i];

            // apply every Filter
            // TODO: maybe process them in normal order
            for (int j = _filters.size() - 1; j >= 0; j--) {
                Filter *filter = _filters.at(j);

                if (filter->finished()) {
                    // this is a timed filter which should be deleted when it has finished

                    // remove filter from list
                    std::vector<Filter*>::iterator it = _filters.begin() + j;
                    _filters.erase(it);

                    // delete filter
                    delete filter;
                    filter = NULL;
                } else {
                    color = filter->apply(color);
                }
            }

            _buffer[i] = color;
        }
    }

public:
    Element(CRGB *buffer) {
        _buffer = buffer;
    }

    virtual size_t getLedCount() = 0;

    void addFilter(Filter *filter) {
        _filters.push_back(filter);
    }

    void removeFilter(Filter *filter) {
        // TODO: double check - why the _filters.end() at the end???
        _filters.erase(std::remove(_filters.begin(), _filters.end(), filter), _filters.end());
    }

    void removeAllFilter() {
        for (int i = _filters.size() - 1; i >= 0; i--) {
            Filter *filter = _filters.at(i);
            delete filter;
        }

        _filters.clear();
    }

    size_t getFilterCount() {
        return _filters.size();
    }

    void setColor(CRGB color) {
        for (int i = 0; i < getLedCount(); i++) {
            _buffer[i] = color;
        }
    }

    void setColorAt(size_t index, CRGB color) {
        _buffer[index] = color;
    }

    virtual void applyFilter() {
        applyFilterImpl();
    }
};

#endif // ELEMENT_H
