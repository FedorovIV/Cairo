#include <iostream>
#include <gumbo.h>
#include <cairo/cairo.h>

void search_for_links(GumboNode* node) {
    if (node->type != GUMBO_NODE_ELEMENT) return;

    GumboElement* el = &node->v.element;
    if (el->tag == GUMBO_TAG_A) {
        GumboAttribute* href = gumbo_get_attribute(&el->attributes, "href");
        if (href)
            std::cout << "Link: " << href->value << std::endl;
    }

    const GumboVector* children = &el->children;
    for (unsigned int i = 0; i < children->length; ++i)
        search_for_links(static_cast<GumboNode*>(children->data[i]));
}

int main() {
    // --- Пример работы с gumbo ---
    std::string html = R"(
        <html><body>
          <a href='https://example.com'>Example</a>
          <a href='/local'>Local</a>
        </body></html>
    )";
    GumboOutput* output = gumbo_parse(html.c_str());
    search_for_links(output->root);
    gumbo_destroy_output(&kGumboDefaultOptions, output);

    // --- Пример работы с Cairo ---
    cairo_surface_t* surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 200, 100);
    cairo_t* cr = cairo_create(surface);
    cairo_set_source_rgb(cr, 0.2, 0.6, 0.9);
    cairo_paint(cr);
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 24);
    cairo_move_to(cr, 20, 60);
    cairo_show_text(cr, "Hello, Cairo!");
    cairo_destroy(cr);
    cairo_surface_write_to_png(surface, "output.png");
    cairo_surface_destroy(surface);

    std::cout << "Saved image: output.png\n";
    return 0;
}
