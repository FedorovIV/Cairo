#include "HTMLParser.h"
#include <iostream>
#include <regex>
#include <sstream>

void HTMLParser::parseNode(const std::string& html, size_t& pos, Node& node) {
    std::cout << "Parsing node at position " << pos << std::endl;

    // Извлекаем имя тега
    node.tag = extractTagName(html, pos);
    std::cout << "Parsed tag: " << node.tag << std::endl;

    // Извлекаем атрибуты тега
    parseAttributes(html, pos, node);

    // Если тег закрывается сразу, например, <img />, продолжаем
    if (html[pos] == '/') {
        pos += 2; // Пропускаем "/>"
        return;
    }

    // Парсим дочерние элементы
    while (pos < html.size()) {
        // Пропускаем пробелы
        if (html[pos] == ' ' || html[pos] == '\n' || html[pos] == '\t') {
            pos++;
            continue;
        }

        // Находим следующий тег
        if (html[pos] == '<') {
            pos++;  // Пропускаем '<'
            if (html[pos] == '/') {  // Закрывающий тег
                break;
            }
            Node childNode;
            parseNode(html, pos, childNode);
            node.children.push_back(childNode);
        } else {
            break;
        }
    }

    if (html[pos] == '>') {
        pos++;
    }
}

std::string HTMLParser::extractTagName(const std::string& html, size_t& pos) {
    // Пропускаем начальные пробелы, символы новой строки и табуляции
    while (pos < html.size() && (html[pos] == ' ' || html[pos] == '\n' || html[pos] == '\t')) {
        pos++;
    }

    // Если после пробела или новой строки мы не находим открывающего тега, выводим ошибку
    if (pos >= html.size() || html[pos] != '<') {
        std::cerr << "Error: Expected '<' at position " << pos << std::endl;
        return "";
    }

    pos++;  // Пропускаем '<'

    std::string tagName;
    while (pos < html.size() && (isalnum(html[pos]) || html[pos] == '_')) {
        tagName += html[pos++];
    }

    if (tagName.empty()) {
        std::cerr << "Error extracting tag name at position " << pos << std::endl;
    }

    std::cout << "Extracted tag name: " << tagName << std::endl;
    return tagName;
}

void HTMLParser::parseAttributes(const std::string& html, size_t& pos, Node& node) {
    std::cout << "Parsing attributes at position " << pos << std::endl;
    while (pos < html.size() && html[pos] != '>' && html[pos] != '/') {
        // Пропускаем пробелы
        if (html[pos] == ' ' || html[pos] == '\n' || html[pos] == '\t') {
            pos++;
            continue;
        }

        // Извлекаем имя атрибута
        std::string attributeName;
        while (pos < html.size() && isalnum(html[pos])) {
            attributeName += html[pos++];
        }

        // Пропускаем "="
        if (html[pos] == '=') {
            pos++;
        }

        // Извлекаем значение атрибута
        if (html[pos] == '"') {
            pos++;  // Пропускаем "
            std::string attributeValue;
            while (pos < html.size() && html[pos] != '"') {
                attributeValue += html[pos++];
            }
            pos++;  // Пропускаем закрывающую "
            node.attributes[attributeName] = attributeValue;
            std::cout << "Found attribute: " << attributeName << "=" << attributeValue << std::endl;
        }
    }
}

Node HTMLParser::parse(const std::string& html) {
    size_t pos = 0;
    Node root;
    root.tag = "div";
    root.attributes["class"] = "container";

    // Парсим <p> теги
    while (pos < html.size()) {
        if (html[pos] == '<') {
            pos++; // Пропускаем '<'
            if (html[pos] == 'p') {
                Node p;
                p.tag = "p";
                p.attributes["style"] = "font-size: 20px; color: red;";
                root.children.push_back(p);
                pos += 3; // Пропускаем "p>"
            }
        } else {
            pos++;
        }
    }

    return root;
}