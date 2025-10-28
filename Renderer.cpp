
#include "Renderer.h"
#include <cairo/cairo.h>

void Renderer::render(Node& root) {
    cairo_surface_t* surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 800, 600);
    cairo_t* cr = cairo_create(surface);

    // Рисуем контейнер (div)
    cairo_set_source_rgb(cr, 0.7, 0.8, 1.0); // lightblue
    cairo_rectangle(cr, 50, 50, 400, 200);
    cairo_fill(cr);

    // Рисуем текст внутри <p>
    cairo_set_source_rgb(cr, 1.0, 0.0, 0.0); // red
    cairo_select_font_face(cr, "Arial", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 20);
    cairo_move_to(cr, 60, 100); // позиция текста
    cairo_show_text(cr, "Hello, Cairo!");

    cairo_surface_write_to_png(surface, "output.png");

    cairo_destroy(cr);
    cairo_surface_destroy(surface);
}
