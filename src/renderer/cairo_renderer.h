#pragma once
#include <cairo/cairo.h>
#include <string>
#include "element.h"

enum class OutputFormat {
    PNG,
    SVG
};

class CairoRenderer {
public:
    CairoRenderer(int width, int height, OutputFormat format = OutputFormat::PNG, const std::string& filename = "output.png");
    ~CairoRenderer();

    void render(const Element& e);
    void save(); // сохраняет результат в выбранный формат

private:
    cairo_surface_t* surface_;
    cairo_t* cr_;
    int width_, height_;
    OutputFormat format_;
    std::string filename_;
};
