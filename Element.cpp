#include "Element.h"

#include "BrightnessFilter.h"

#include "ElementManager.h"
#include "FilterManager.h"
#include "EffectManager.h"

Element::Element(String type, size_t ledCount, CRGB *buffer) {
    _type = type;
    _ledCount = ledCount;
    _buffer = buffer;

    _currentChildrenLedIndex = 0;

    // first filter is always the brightness
    // fade with a 500ms animation
    _filters.push_back(new BrightnessFilter(255, 255, 500, 0));
}

Element::Element(String type, JsonObject &root) : Element(type, 0, NULL) {
    fromJson(root);
}

Element::~Element() {
    for (element_iterator it = _children.begin(); it != _children.end(); it++) {
        Element *elem = *it;

        if (elem) {
            delete elem;
        }
    }
    _children.clear();

    for (filter_iterator it = _filters.begin(); it != _filters.end(); it++) {
        Filter *filter = *it;

        if (filter) {
            delete filter;
        }
    }
    _filters.clear();

    if (_effect) {
        delete _effect;
        _effect = NULL;
    }
}

void Element::fromJson(JsonObject &root) {
    _ledCount = root["ledCount"] | _ledCount;

    // delete current children to free memory
    for (element_iterator it = childrenBegin(); it != childrenEnd(); it++) {
        Element *elem = *it;
        if (elem) {
            delete elem;
        }
    }
    _children.clear();
    _currentChildrenLedIndex = 0;

    // add new children
    JsonArray childrenArray = root["children"].as<JsonArray>();
    for (JsonVariant childVariant : childrenArray) {
        JsonObject childObject = childVariant.as<JsonObject>();
        String childType = childObject["type"] | "";

        if (childObject && childType.length()) {
            Element *child = createElement(childType, childObject);
            if (!child) {
                continue;
            }

            addChild(child);
        }
    }

    JsonArray filtersArray = root["filters"].as<JsonArray>();
    if (filtersArray) {
        // delete current filters
        for (filter_iterator it = _filters.begin(); it != _filters.end(); it++) {
            Filter *filter = *it;
            if (filter) {
                delete filter;
            }
        }
        _filters.clear();

        // add new filters
        for (JsonVariant filterVariant : filtersArray) {
            JsonObject filterObject = filterVariant.as<JsonObject>();
            String filterType = filterObject["type"] | "";

            if (filterObject && filterType.length()) {
                Filter *filter = createFilter(filterType, root);
                if (!filter) {
                    continue;
                }

                addFilter(filter);
            }
        }
    }

    JsonObject effectObject = root["effect"].as<JsonObject>();
    String effectName = effectObject["name"] | "";
    if (effectObject && effectName.length()) {
        // delete current effect
        if (_effect) {
            delete _effect;
            _effect = NULL;
        }

        _effect = createEffect(effectName, effectObject);
    }
}

void Element::patchJson(JsonObject &root) {
    JsonVariant ledCountVariant = root["ledCount"];
    if (ledCountVariant) {
        _ledCount = ledCountVariant.as<size_t>();
    }

    JsonArray childrenArray = root["children"].as<JsonArray>();
    element_iterator childIt = childrenBegin();
    for (JsonArray::iterator it = childrenArray.begin(); it != childrenArray.end() && childIt != childrenEnd(); ++it, ++childIt) {
        JsonObject childObject = it->as<JsonObject>();
        Element *elem = *childIt;

        elem->patchJson(childObject);
    }

    JsonArray filtersArray = root["filters"].as<JsonArray>();
    filter_iterator filterIt = _filters.begin();
    for (JsonArray::iterator it = filtersArray.begin(); it != filtersArray.end() && filterIt != _filters.end(); ++it, ++filterIt) {
        JsonObject filterObject = it->as<JsonObject>();
        Filter *filter = *filterIt;

        filter->patchJson(filterObject);
    }

    JsonObject effectObject = root["effect"];
    String effectName = effectObject["name"];
    if (effectObject) {
        if (!_effect || !_effect->getName().equals(effectName)) {
            if (_effect) {
                delete _effect;
                _effect = NULL;
            }

            _effect = createEffect(effectName, effectObject);
        } else {
            _effect->patchJson(effectObject);
        }
    }
}

void Element::toJson(JsonObject &root) {
    root["type"] = _type;
    root["ledCount"] = _ledCount;

    if (_children.size()) {
        JsonArray childrenArray = root.createNestedArray("children");
        for (element_iterator it = childrenBegin(); it != childrenEnd(); it++) {
            Element *elem = *it;
            if (!elem) {
                continue;
            }

            JsonObject childObject = childrenArray.createNestedObject();
            elem->toJson(childObject);
        }
    }

    // skip filter to not blow up the json
    /*
    if (_filters.size()) {
        JsonArray filtersArray = root.createNestedArray("filters");
        for (filter_iterator it = _filters.begin(); it != _filters.end(); it++) {
            Filter *filter = *it;
            if (!filter || filter->getTimeout() != 0) { // only save non-temporary filters
                continue;
            }

            JsonObject filterObject = filtersArray.createNestedObject();
            filter->toJson(filterObject);
        }
    }
    */

    if (_effect) {
        JsonObject effectObject = root.createNestedObject("effect");
        _effect->toJson(effectObject);
    }
}

size_t Element::getLedCount() {
    return _ledCount;
}

String Element::getType() {
    return _type;
}

void Element::setBuffer(CRGB *buffer) {
    _buffer = buffer;

    // update every child buffer
    _currentChildrenLedIndex = 0;
    for (std::list<Element*>::iterator it = _children.begin(); it != _children.end(); it++) {
        Element *elem = *it;
        elem->setBuffer(&_buffer[_currentChildrenLedIndex]);
        _currentChildrenLedIndex += elem->getLedCount();
    }
}

CRGB* Element::getBuffer() {
    return _buffer;
}

bool Element::addChild(Element *child) {
    if (_currentChildrenLedIndex + child->getLedCount() > _ledCount) {
        return false;
    }

    if (_buffer) {
        child->setBuffer(&_buffer[_currentChildrenLedIndex]);
    }
    _children.push_back(child);

    _currentChildrenLedIndex += child->getLedCount();

    return true;
}

void Element::removeChild(Element *child) {
    _children.remove(child);
}

void Element::clearChildren() {
    _children.clear();
}

Element* Element::getNthTypeChild(String type, size_t n) {
    size_t found = 0;

    for (element_iterator it = childrenBegin(); it != childrenEnd(); it++) {
        Element* elem = *it;
        String elemType = elem->getType();

        if (type.equals(elemType)) {
            if (n == found++) {
                return elem;
            }
        }
    }

    return NULL;
}

element_iterator Element::childrenBegin() {
    return _children.begin();
}

element_iterator Element::childrenEnd() {
    return _children.end();
}

void Element::setBrightness(uint8_t brightness) {
    ((BrightnessFilter*)(*_filters.begin()))->setMaxBrightness(brightness);
}

void Element::setColor(CRGB color) {
    for (int i = 0; i < getLedCount(); i++) {
        _buffer[i] = color;
    }
}

void Element::setColorAt(size_t index, CRGB color) {
    _buffer[index] = color;
}

void Element::addFilter(Filter *filter) {
    _filters.push_back(filter);
}

void Element::removeAllFilters() {
    // the first filter is the brightness filter - DON'T DELETE
    for (filter_iterator it = (++_filters.begin()); it != _filters.end(); it++) {
        Filter *filter = *it;

        if (filter) {
            delete filter;
        }
    }

    _filters.clear();
}

void Element::applyFilter() {
    for (size_t i = 0; i < getLedCount(); i++) {
        CRGB color = _buffer[i];

        // TODO maybe in reverse order
        filter_iterator it = _filters.begin();
        while(it != _filters.end()) {
            Filter *filter = *it;

            if (filter->finished()) {
                delete filter;
                _filters.erase(it++);
                continue;
            } else {
                color = filter->apply(color);
                it++;
            }
        }

        _buffer[i] = color;
    }
}

void Element::update() {
    // Place to update own effects
    updateChildren();

    applyFilter();
}

void Element::updateChildren() {
    for (element_iterator it = _children.begin(); it != _children.end(); it++) {
        Element *elem = *it;
        elem->update();
    }
}
