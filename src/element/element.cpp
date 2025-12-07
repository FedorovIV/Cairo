#include "element.h"
#include <algorithm> // для std::clamp

Element::Element(Element* parent, double width, double height)
    : parent_(parent), width_(width), height_(height),
      color_("#000000"), border_radius_(0.0)
{
    if (parent) {
        x_ = parent->x();
        y_ = parent->y();
    } else {
        x_ = 0.0;
        y_ = 0.0;
    }
}

void Element::set_position(double x, double y) {
    x_ = x;
    y_ = y;
}

void Element::set_size(double width, double height) {
    width_ = width;
    height_ = height;
}

void Element::set_color(const std::string& color) {
    color_ = color;
}

void Element::set_border_radius(double percent) {
    border_radius_ = std::clamp(percent, 0.0, 100.0);
}

double Element::x() const { return x_; }
double Element::y() const { return y_; }
double Element::width() const { return width_; }
double Element::height() const { return height_; }
const std::string& Element::color() const { return color_; }
double Element::border_radius() const { return border_radius_; }
