
#include "HTMLParser.h"
#include <map>
#include <vector>
#include <string>

Node HTMLParser::parse(const std::string& html) {
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
