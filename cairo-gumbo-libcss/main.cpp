#include <gumbo.h>
#include <cairo/cairo.h>
#include <string.h>
#include <vector>

#include "read_files.h"
#include "find_headers.h"
#include "libcss_main.h"

void parse_color(const char *hex, double *r, double *g, double *b);

int main(void)
{
    char *css = read_file_all("/mnt/c/programming/Cairo/cairo-gumbo-libcss/for-test/style.css", NULL);
    char *html = read_file_all("/mnt/c/programming/Cairo/cairo-gumbo-libcss/for-test/index.html", NULL);

    if (!css || !html)
    {
        free(css);
        free(html);
        return 1;
    }

    printf("CSS (фрагмент): %.60s\n", css);
    printf("HTML (фрагмент): %.60s\n", html);

    // парсим HTML
    GumboOutput *output = gumbo_parse(html);
    std::vector<HtmlHeader> headers_html = extract_headers(output->root);
    gumbo_destroy_output(&kGumboDefaultOptions, output);

    // --- CSS обработка через libcss ---
    size_t count;
    header_color_t *css_headers = get_headers_colors(css, &count);

    // --- Связываем CSS цвета с HTML элементами ---
    for (auto &h : headers_html)
    {
        for (size_t i = 0; i < count; i++)
        {
            if (strcmp(css_headers[i].tag, h.tag.c_str()) == 0)
            {
                h.color = css_headers[i].color;
                break;
            }
        }
    }

    // --- Отрисовка Cairo ---
    const int width = 500;
    const int height = 900;

    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    cairo_t *cr = cairo_create(surface);

    // фон
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);

    // шрифт
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);

    double y = 60.0;
    for (auto &h : headers_html)
    {
        double r, g, b;
        parse_color(h.color.c_str(), &r, &g, &b);
        cairo_set_source_rgb(cr, r, g, b);

        // размер шрифта в зависимости от тега
        int level = h.tag[1] - '0'; // 'h1' → 1, 'h2' → 2 ...
        double size = 60.0 - (level - 1) * 5.0;
        cairo_set_font_size(cr, size);

        cairo_move_to(cr, 50, y);
        cairo_show_text(cr, h.textContent.c_str());
        y += size + 15;
    }

    cairo_surface_write_to_png(surface, "output.png");

    cairo_destroy(cr);
    cairo_surface_destroy(surface);

    printf("✅ Сохранено в output.png\n");

    // освобождаем память из libcss
    for (size_t i = 0; i < count; i++)
        free((void *)css_headers[i].tag);
    free(css_headers);

    free(css);
    free(html);

    return 0;
}

void parse_color(const char *hex, double *r, double *g, double *b)
{
    if (hex[0] == '#')
        hex++;
    unsigned int rgb = 0;
    sscanf(hex, "%x", &rgb);
    *r = ((rgb >> 16) & 0xFF) / 255.0;
    *g = ((rgb >> 8) & 0xFF) / 255.0;
    *b = (rgb & 0xFF) / 255.0;
}