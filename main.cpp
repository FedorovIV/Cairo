#include <sstream> 
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include "HTMLParser.h"
#include "CSSParser.h"
#include "LayoutEngine.h"
#include "Renderer.h"

// Чтение файла HTML
std::string readHTMLFile(const std::string& filepath) {
    std::ifstream file(filepath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Чтение файла CSS
std::string readCSSFile(const std::string& filepath) {
    std::ifstream file(filepath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main(int argc, char* argv[]) {
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
    std::string css = readCSSFile(cssFile);

    // Парсим HTML и CSS
    Node root = htmlParser.parse(html);
    cssParser.parse(css);

    // Вычисляем layout
    layoutEngine.calculate(root);

    // Отрисовываем результат
    renderer.render(root);

    std::cout << "Output generated as output.png" << std::endl;
    return 0;
}