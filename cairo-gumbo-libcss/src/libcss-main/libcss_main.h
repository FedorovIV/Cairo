#pragma once

#include <stddef.h>

// Оборачиваем подключение C-библиотеки libcss в extern "C"
#ifdef __cplusplus
extern "C" {
#endif

#include <libcss/libcss.h>

/**
 * Структура описывает цвет заголовков <h1>-<h6>.
 */
typedef struct
{
    const char *tag;   ///< "h1", "h2", ... "h6"
    char color[8];     ///< "#rrggbb" или "inherit"
} header_color_t;

/**
 * Парсит CSS и возвращает цвета для h1–h6.
 */
header_color_t *get_headers_colors(const char *css_source, size_t *out_count);

/**
 * Простейший обработчик URL (заглушка для LibCSS).
 */
css_error resolve_url(void *pw, const char *base, lwc_string *rel, lwc_string **abs);

/**
 * Печатает сообщение об ошибке и завершает выполнение.
 */
void die(const char *text, css_error code);

#ifdef __cplusplus
}
#endif
