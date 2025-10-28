#include "LayoutEngine.h"
#include <iostream>

void LayoutEngine::calculate(Node& root) {
    int y_offset = 0;
    
    // Проверка начала обработки
    std::cout << "Starting layout calculation..." << std::endl;

    // Проверяем дочерние элементы root
    std::cout << "Root has " << root.children.size() << " children." << std::endl;

    for (auto& child : root.children) {
        std::cout << "Processing tag: " << child.tag << std::endl;

        // Проверка, что <p> попадает в обработку
        if (child.tag == "p") {
            std::cout << "Found <p> tag. Setting 'top' attribute." << std::endl;
            child.attributes["top"] = std::to_string(y_offset);
            std::cout << "Calculated 'top' for <p>: " << y_offset << std::endl;
            y_offset += 30;  // например, между абзацами — 30px
        } else {
            std::cout << "Skipping tag: " << child.tag << std::endl;
        }

        // Выводим значение top после расчёта
        std::cout << "Final 'top' attribute for " << child.tag << ": " 
                  << child.attributes["top"] << std::endl;
    }

    std::cout << "Layout calculation completed." << std::endl;
}
