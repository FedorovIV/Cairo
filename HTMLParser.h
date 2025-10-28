
#ifndef HTMLPARSER_H
#define HTMLPARSER_H

#include <string>
#include <map>
#include <vector>

struct Node {
    std::string tag;
    std::map<std::string, std::string> attributes;
    std::vector<Node> children;
};

class HTMLParser {
public:
    Node parse(const std::string& html);
};

#endif // HTMLPARSER_H
