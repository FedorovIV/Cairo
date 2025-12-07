#pragma once
#include <cairo/cairo.h>
#include <string>
#include "element.h"

enum class OutputFormat {
    PNG,
    SVG,
    SVG_MEMORY,
};

class CairoRenderer {
public:
    CairoRenderer(int width, int height, OutputFormat format = OutputFormat::PNG, const std::string& filename = "output.png");
    ~CairoRenderer();

    void render(const Element& e);
    void save();
    void finish(); 
    std::string get_svg() const { return svg_buffer_; };
    std::string get_svg_string();

private:
    cairo_surface_t* surface_;
    cairo_t* cr_;
    int width_, height_;
    OutputFormat format_;
    std::string filename_;
    bool isFinished_;
    std::string svg_buffer_;
};
