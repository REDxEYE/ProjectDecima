#include "decima/serializable/object/collection.hpp"

void Decima::Collection::draw() {
    for (auto& ref : refs.data()) {
        ref.draw();
    }
}
