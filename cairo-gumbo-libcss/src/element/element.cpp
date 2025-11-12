#include "element.h"

Element::Element(const Element* parent, coord_t width, coord_t height) noexcept
    : parent_(parent), width_(width), height_(height)
{
    if (parent_) {
        // Наследуем позицию родителя
        x_ = parent_->x();
        y_ = parent_->y();
    } else {
        x_ = 0.0;
        y_ = 0.0;
    }
}
