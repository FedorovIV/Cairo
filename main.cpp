#include <sstream>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include "HTMLParser.h"
#include "CSSParser.h"
#include "LayoutEngine.h"
#include "Renderer.h"

std::string readHTMLFile(const std::string& filepath) {
    std::ifstream file(filepath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::string readCSSFile(const std::string& filepath) {
    std::ifstream file(filepath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void printNode(const Node& node, int level = 0) {
    std::string indent(level * 2, ' ');
    std::cout << indent << "Tag: " << node.tag << std::endl;
    for (const auto& attribute : node.attributes) {
        std::cout << indent << "  " << attribute.first << ": " << attribute.second << std::endl;
    }
    for (const auto& child : node.children) {
        printNode(child, level + 1);
    }
}

int main(int argc, char *argv[]) {

    std::cout << "Output generated as output.png" << std::endl;

    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <index.html> <styles.css>" << std::endl;
        return 1;
    }

    std::string htmlFile = argv[1];
    std::string cssFile = argv[2];

    HTMLParser htmlParser;
    CSSParser cssParser;
    LayoutEngine layoutEngine;
    Renderer renderer;

    // Читаем файлы
    std::string html = readHTMLFile(htmlFile);
    std::cout << "HTML content:" << std::endl;
    std::cout << html << std::endl;
    std::string css = readCSSFile(cssFile);

    // Парсим HTML и CSS
    Node root = htmlParser.parse(html);
    printNode(root);

    std::cout << "Parsing CSS" << std::endl;
    cssParser.parse(css);

    // Вычисляем layout
    layoutEngine.calculate(root);

    // Отрисовываем результат
    renderer.render(root);

    std::cout << "Output generated as output.png" << std::endl;
    return 0;
}