#include "HTMLParser.h"
#include <iostream>
#include <cctype>
#include <sstream>

std::string HTMLParser::parseTag(const std::string& html, size_t& pos) {
    std::string tag;
    
    // Пропускаем все пробелы перед тегом
    while (pos < html.size() && std::isspace(html[pos])) {
        pos++;
    }
    
    if (html[pos] != '<') {
        std::cerr << "Expected '<' at position " << pos << std::endl;
        return "";
    }
    
    pos++; // Пропускаем '<'
    
    // Читаем имя тега
    while (pos < html.size() && std::isalnum(html[pos])) {
        tag += html[pos++];
    }

    return tag;
}

void HTMLParser::parseAttributes(const std::string& html, size_t& pos, Node& node) {
    while (pos < html.size() && html[pos] != '>' && html[pos] != '/') {
        // Пропускаем пробелы
        if (html[pos] == ' ' || html[pos] == '\n' || html[pos] == '\t' || html[pos] == '\r') {
            pos++;
            continue;
        }

        // Извлекаем имя атрибута
        std::string attributeName;
        while (pos < html.size() && std::isalnum(html[pos])) {
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
        }
    }
}

void HTMLParser::parseNode(const std::string& html, size_t& pos, Node& node) {
    node.tag = parseTag(html, pos);

    // Парсим атрибуты
    parseAttributes(html, pos, node);

    // Пропускаем пробелы до закрывающего тега
    while (pos < html.size() && (html[pos] == ' ' || html[pos] == '\n' || html[pos] == '\t')) {
        pos++;
    }

    // Если тег закрывается сразу, например, <img />, продолжаем
    if (html[pos] == '/') {
        pos += 2; // Пропускаем "/>"
        return;
    }

    // Парсим дочерние элементы
    if (html[pos] == '>') {
        pos++; // Пропускаем '>'
    }

    while (pos < html.size()) {
        if (html[pos] == '<') {
            // Пропускаем открывающий тег
            pos++;
            if (html[pos] == '/') {
                break; // Закрывающий тег
            }
            Node childNode;
            parseNode(html, pos, childNode);
            node.children.push_back(childNode);
        } else {
            pos++;
        }
    }

    // Пропускаем закрывающий тег
    if (html[pos] == '<' && html[pos + 1] == '/') {
        pos += 3 + node.tag.size(); // Пропускаем "</tag>"
    }
}

Node HTMLParser::parse(const std::string& html) {
    size_t pos = 0;
    Node root;
    parseNode(html, pos, root);
    return root;
}
