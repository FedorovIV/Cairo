#include "element.h"
#include "cairo_renderer.h"

int main() {
    Element rect(nullptr, 200, 100);
    rect.set_position(50, 50);
    rect.set_color("#3498db");
    rect.set_border_radius(25);

    Element circle(nullptr, 100, 100);
    circle.set_position(300, 50);
    circle.set_color("#e74c3c");
    circle.set_border_radius(100);

    // SVG-рендерер
    CairoRenderer renderer(500, 200, OutputFormat::PNG, "output.png");
    renderer.render(rect);
    renderer.render(circle);
    renderer.save();

    return 0;
}