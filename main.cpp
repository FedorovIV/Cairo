#include <cairo/cairo.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <map>  // Для std::map
#include <vector>  // Для std::vector
struct Node {
    std::string tag;
    std::map<std::string, std::string> attributes;
    std::vector<Node> children;
};

class HTMLParser {
public:
    Node parse(const std::string& html) {
        Node root;
        root.tag = "div";
        root.attributes["class"] = "container";

        Node p;
        p.tag = "p";
        p.attributes["style"] = "font-size: 20px; color: red;";
        root.children.push_back(p);

        Node textNode;
        textNode.tag = "text";
        textNode.attributes["content"] = "Hello, Cairo!";
        p.children.push_back(textNode);

        return root;
    }
};

class CSSParser {
public:
    void parse(const std::string& css) {
        styles[".container"] = {{"background-color", "lightblue"}, {"width", "400px"}};
        styles["p"] = {{"font-size", "20px"}, {"color", "red"}};
    }

    std::map<std::string, std::string> getStyles(const std::string& tagName) {
        return styles[tagName];
    }

private:
    std::map<std::string, std::map<std::string, std::string>> styles;
};

class LayoutEngine {
public:
    void calculate(Node& root) {
        int y_offset = 0;
        for (auto& child : root.children) {
            if (child.tag == "p") {
                child.attributes["top"] = std::to_string(y_offset);
                y_offset += 30; // например, между абзацами — 30px
            }
        }
    }
};

class Renderer {
public:
    void render(Node& root) {
        cairo_surface_t* surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 800, 600);
        cairo_t* cr = cairo_create(surface);

        // Рисуем контейнер (div)
        cairo_set_source_rgb(cr, 0.7, 0.8, 1.0); // lightblue
        cairo_rectangle(cr, 50, 50, 400, 200);
        cairo_fill(cr);

        // Рисуем текст внутри <p>
        cairo_set_source_rgb(cr, 1.0, 0.0, 0.0); // red
        cairo_select_font_face(cr, "Arial", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
        cairo_set_font_size(cr, 20);
        cairo_move_to(cr, 60, 100); // позиция текста
        cairo_show_text(cr, "Hello, Cairo!");

        cairo_surface_write_to_png(surface, "output.png");

        cairo_destroy(cr);
        cairo_surface_destroy(surface);
    }
};

int main() {
    HTMLParser htmlParser;
    CSSParser cssParser;
    LayoutEngine layoutEngine;
    Renderer renderer;

    // Примерный HTML и CSS
    std::string html = "<div class='container'><p style='font-size: 20px;'>Hello, Cairo!</p></div>";
    std::string css = ".container { width: 400px; height: 200px; background-color: lightblue; } p { font-size: 20px; color: red; }";

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
