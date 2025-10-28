#include "CSSParser.h"
#include <sstream>
#include <regex>
#include <iostream>

// Простой парсер CSS с использованием регулярных выражений
void CSSParser::parse(const std::string& css) {
    // Регулярное выражение для поиска классов и стилей
    std::regex rgx(R"((\.[a-zA-Z0-9_-]+)\s*\{\s*([^}]+)\s*\})");
    std::smatch match;

    std::string::const_iterator searchStart(css.cbegin());

    // Обрабатываем все совпадения
    while (std::regex_search(searchStart, css.cend(), match, rgx)) {
        // Имя класса
        std::string className = match[1].str();
        
        // Стиль (всё, что внутри {})
        std::string styleBlock = match[2].str();

        // Разбираем стиль
        std::map<std::string, std::string> stylesMap;
        std::regex styleRgx(R"(([^:]+)\s*:\s*([^;]+))");  // для извлечения свойств и значений
        std::smatch styleMatch;
        std::string::const_iterator styleStart(styleBlock.cbegin());

        while (std::regex_search(styleStart, styleBlock.cend(), styleMatch, styleRgx)) {
            // Добавляем свойства в map
            stylesMap[styleMatch[1].str()] = styleMatch[2].str();
            styleStart = styleMatch.suffix().first;
        }

        // Сохраняем парсенные стили для класса
        styles[className] = stylesMap;

        searchStart = match.suffix().first;
    }
}

// Получаем стили для определённого тега или класса
std::map<std::string, std::string> CSSParser::getStyles(const std::string& tagName) {
    if (styles.find(tagName) != styles.end()) {
        return styles[tagName];
    }
    return {};  // Если не найдено, возвращаем пустую карту
}
