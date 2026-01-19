#pragma once
#include <string>

class Element
{
public:
    Element(Element *parent = nullptr, double width = 0.0, double height = 0.0);

    void set_position(double x, double y);
    void set_size(double width, double height);
    void set_tag(const std::string &tag);
    void set_color(const std::string &color);
    void set_border_radius(double percent); // 0-100%
    void set_text(const std::string &text);

    double x() const;
    double y() const;
    double width() const;
    double height() const;
    const std::string &tag() const;
    const std::string &color() const;
    const std::string &text() const;
    double border_radius() const;

private:
    Element *parent_;
    double x_, y_, width_, height_;
    double border_radius_; // от 0 до 100
    std::string tag_;      // по умолчанию "div"
    std::string color_;    // по умолчанию "#000000"
    std::string text_;
};

class Button : public Element
{
public:
    explicit Button(const std::string &text, Element *parent = nullptr);
};
