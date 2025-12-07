#include "cairo_renderer.h"
#include <cairo/cairo-svg.h> // <--- нужно для SVG
#include <cmath>
#include <sstream>
#include <iomanip>

CairoRenderer::CairoRenderer(int width, int height, OutputFormat format, const std::string& filename)
    : width_(width), height_(height), format_(format), filename_(filename)
{
    if (format == OutputFormat::SVG) {
        surface_ = cairo_svg_surface_create(filename.c_str(), width, height);
    } else {
        surface_ = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    }

    cr_ = cairo_create(surface_);

    // Белый фон
    cairo_set_source_rgb(cr_, 1, 1, 1);
    cairo_paint(cr_);
}

CairoRenderer::~CairoRenderer() {
    cairo_destroy(cr_);
    cairo_surface_destroy(surface_);
}

// --- вспомогательная функция ---
static void parse_color(const std::string& hex, double& r, double& g, double& b) {
    if (hex.size() != 7 || hex[0] != '#') { r = g = b = 0; return; }
    unsigned int color = 0;
    std::stringstream ss;
    ss << std::hex << hex.substr(1);
    ss >> color;
    r = ((color >> 16) & 0xFF) / 255.0;
    g = ((color >> 8) & 0xFF) / 255.0;
    b = (color & 0xFF) / 255.0;
}

void CairoRenderer::render(const Element& e) {
    double r, g, b;
    parse_color(e.color(), r, g, b);
    cairo_set_source_rgb(cr_, r, g, b);

    double x = e.x();
    double y = e.y();
    double w = e.width();
    double h = e.height();
    double radius_percent = e.border_radius();
    double radius_px = std::min(w, h) * (radius_percent / 100.0) / 2.0;

    if (radius_percent >= 100.0) {
        // Круг
        double cx = x + w / 2.0;
        double cy = y + h / 2.0;
        double r_circle = std::min(w, h) / 2.0;
        cairo_arc(cr_, cx, cy, r_circle, 0, 2 * M_PI);
        cairo_fill(cr_);
        return;
    }

    // Прямоугольник с закруглёнными углами
    double rpx = std::min(radius_px, std::min(w, h) / 2.0);
    cairo_new_path(cr_);
    cairo_move_to(cr_, x + rpx, y);
    cairo_line_to(cr_, x + w - rpx, y);
    cairo_arc(cr_, x + w - rpx, y + rpx, rpx, -M_PI/2, 0);
    cairo_line_to(cr_, x + w, y + h - rpx);
    cairo_arc(cr_, x + w - rpx, y + h - rpx, rpx, 0, M_PI/2);
    cairo_line_to(cr_, x + rpx, y + h);
    cairo_arc(cr_, x + rpx, y + h - rpx, rpx, M_PI/2, M_PI);
    cairo_line_to(cr_, x, y + rpx);
    cairo_arc(cr_, x + rpx, y + rpx, rpx, M_PI, 3*M_PI/2);
    cairo_close_path(cr_);
    cairo_fill(cr_);
}

void CairoRenderer::save() {
    if (format_ == OutputFormat::PNG) {
        cairo_surface_write_to_png(surface_, filename_.c_str());
    } else if (format_ == OutputFormat::SVG) {
        cairo_show_page(cr_); // завершает SVG-документ
    }
}
