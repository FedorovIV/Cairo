#include "element.h"
#include <algorithm> // для std::clamp

Element::Element(Element *parent, double width, double height)
    : parent_(parent),
      x_(0.0),
      y_(0.0),
      width_(width),
      height_(height),
      border_radius_(0.0),
      tag_("div"),
      color_("#000000")
{
    if (parent)
    {
        x_ = parent->x();
        y_ = parent->y();
    }
    else
    {
        x_ = 0.0;
        y_ = 0.0;
    }
}

void Element::set_position(double x, double y)
{
    x_ = x;
    y_ = y;
}

void Element::set_size(double width, double height)
{
    width_ = width;
    height_ = height;
}

void Element::set_tag(const std::string &tag)
{
    tag_ = tag;
}

void Element::set_color(const std::string &color)
{
    color_ = color;
}

void Element::set_border_radius(double percent)
{
    border_radius_ = std::clamp(percent, 0.0, 100.0);
}

void Element::set_text(const std::string &text)
{
    text_ = text;
}

double Element::x() const { return x_; }
double Element::y() const { return y_; }
double Element::width() const { return width_; }
double Element::height() const { return height_; }
const std::string &Element::tag() const { return tag_; }
const std::string &Element::color() const { return color_; }
const std::string &Element::text() const { return text_; }
double Element::border_radius() const { return border_radius_; }

Button::Button(const std::string &text, Element *parent)
    : Element(parent, 140.0, 44.0)
{
    set_tag("button");
    set_color("#4a90e2");
    set_border_radius(12.0);
    set_text(text);
}
