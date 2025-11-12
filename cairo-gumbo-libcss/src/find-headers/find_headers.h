#pragma once

#include <gumbo.h>
#include <string>
#include <vector>

// Структура, описывающая заголовок (h1–h6)
struct HtmlHeader
{
    std::string tag;         // "h1", "h2" и т.д.
    std::string textContent; // текст внутри тега
    std::string color;       // цвет из CSS (по умолчанию "#000000")
};

// Находит все теги <h1>–<h6> и возвращает их имена в tags
void find_headers(GumboNode* node, std::vector<std::string>& tags);

// Возвращает текстовое содержимое узла (рекурсивно)
std::string get_text_content(GumboNode* node);

// Извлекает все заголовки (h1–h6) с текстом и базовым цветом
std::vector<HtmlHeader> extract_headers(GumboNode* node);
