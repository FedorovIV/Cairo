#include "Renderer.h"
#include <cairo/cairo.h>
#include <iostream>

void Renderer::render(Node &root)
{
    std::cout << "Starting rendering..." << std::endl;

    // Создание поверхности для рендеринга
    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 800, 600);
    if (cairo_surface_status(surface) != CAIRO_STATUS_SUCCESS)
    {
        std::cerr << "Error creating cairo surface! Error code: "
                  << cairo_status_to_string(cairo_surface_status(surface)) << std::endl;
        return;
    }

    // Создание контекста
    cairo_t* cr = cairo_create(surface);
    if (!cr) {
        std::cerr << "Error creating cairo context!" << std::endl;
        cairo_surface_destroy(surface);
        return;
    }

    // Рисуем контейнер (div)
    std::cout << "Drawing container..." << std::endl;
    cairo_set_source_rgb(cr, 0.7, 0.8, 1.0); // lightblue
    cairo_rectangle(cr, 50, 50, 400, 200); // Прямоугольник для контейнера
    cairo_fill(cr);

    // Рисуем дочерние элементы
    for (auto& child : root.children) {
        if (child.tag == "p") {
            // Выводим информацию о позиционировании
            std::cout << "Drawing <p> at top=" << child.attributes["top"] << std::endl;

            // Применяем позицию top
            double top = 0.0;
            try {
                top = std::stod(child.attributes["top"]);
            } catch (const std::invalid_argument&) {
                std::cerr << "Invalid top value for <p>: " << child.attributes["top"] << std::endl;
            }

            cairo_set_source_rgb(cr, 1.0, 0.0, 0.0); // красный цвет для текста
            cairo_select_font_face(cr, "Courier", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL); // Используем стандартный шрифт
            cairo_set_font_size(cr, 20);
            cairo_move_to(cr, 60, 50 + top); // позиция текста с учётом top
            cairo_show_text(cr, "Hello, Cairo!"); // Отрисовываем текст
        }
    }

    // Пишем на поверхность
    if (cairo_surface_write_to_png(surface, "output.png") != CAIRO_STATUS_SUCCESS) {
        std::cerr << "Error writing to PNG file!" << std::endl;
    }

    cairo_destroy(cr);
    cairo_surface_destroy(surface);

    std::cout << "Rendering completed, output saved to output.png" << std::endl;
}
