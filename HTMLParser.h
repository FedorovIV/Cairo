#ifndef HTMLPARSER_H
#define HTMLPARSER_H

#include <string>
#include <map>
#include <vector>

// Структура для хранения информации о теге
struct Node {
    std::string tag;  // Тег
    std::map<std::string, std::string> attributes;  // Атрибуты тега
    std::vector<Node> children;  // Дочерние элементы
};

// Класс для парсинга HTML
class HTMLParser {
public:
    Node parse(const std::string& html);  // Метод для парсинга HTML
private:
    void parseNode(const std::string& html, size_t& pos, Node& node);  // Метод для парсинга одного тега
    void parseAttributes(const std::string& html, size_t& pos, Node& node);  // Метод для парсинга атрибутов
    std::string extractTagName(const std::string& html, size_t& pos);  // Метод для извлечения имени тега
};

#endif // HTMLPARSER_H
