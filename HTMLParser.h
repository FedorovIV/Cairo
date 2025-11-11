#ifndef HTMLPARSER_H
#define HTMLPARSER_H

#include <string>
#include <map>
#include <vector>

// Структура для хранения информации о теге HTML
struct Node {
    std::string tag;  // Имя тега, например "div", "p"
    std::map<std::string, std::string> attributes;  // Атрибуты тега
    std::vector<Node> children;  // Дочерние элементы
};

class HTMLParser {
public:
    // Парсинг тега: извлекает имя тега
    std::string parseTag(const std::string& html, size_t& pos);

    // Парсинг атрибутов: извлекает атрибуты тега
    void parseAttributes(const std::string& html, size_t& pos, Node& node);

    // Парсинг узла: извлекает тег, атрибуты и дочерние элементы
    void parseNode(const std::string& html, size_t& pos, Node& node);

    // Главная функция для парсинга HTML строки
    Node parse(const std::string& html);
};

#endif // HTMLPARSER_H
