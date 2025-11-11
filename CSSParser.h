
#ifndef CSSPARSER_H
#define CSSPARSER_H

#include <string>
#include <map>

class CSSParser {
public:
    void parse(const std::string& css);
    std::map<std::string, std::string> getStyles(const std::string& tagName);

private:
    std::map<std::string, std::map<std::string, std::string>> styles;
};

#endif // CSSPARSER_H
