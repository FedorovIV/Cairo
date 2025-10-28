
#include "LayoutEngine.h"

void LayoutEngine::calculate(Node& root) {
    int y_offset = 0;
    for (auto& child : root.children) {
        if (child.tag == "p") {
            child.attributes["top"] = std::to_string(y_offset);
            y_offset += 30; // например, между абзацами — 30px
        }
    }
}
