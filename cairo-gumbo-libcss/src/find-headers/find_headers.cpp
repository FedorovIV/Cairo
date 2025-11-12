#include <gumbo.h>
#include <string>
#include <vector>

struct HtmlHeader
{
    std::string tag;         // "h1", "h2" ...
    std::string textContent; // содержимое тега
    std::string color;       // будет заполнено позже
};

// Рекурсивный обход дерева Gumbo и поиск тегов h1–h6
void find_headers(GumboNode *node, std::vector<std::string> &tags)
{
    if (node->type != GUMBO_NODE_ELEMENT)
        return;

    GumboTag tag = node->v.element.tag;
    if (tag >= GUMBO_TAG_H1 && tag <= GUMBO_TAG_H6)
    {
        const char *name = gumbo_normalized_tagname(tag);
        tags.push_back(name);
    }

    GumboVector *children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; ++i)
        find_headers((GumboNode *)children->data[i], tags);
}

std::string get_text_content(GumboNode *node)
{
    if (node->type == GUMBO_NODE_TEXT)
    {
        return node->v.text.text;
    }
    else if (node->type == GUMBO_NODE_ELEMENT)
    {
        std::string result;
        GumboVector *children = &node->v.element.children;
        for (unsigned int i = 0; i < children->length; ++i)
        {
            GumboNode *child = (GumboNode *)children->data[i];
            result += get_text_content(child);
        }
        return result;
    }
    return "";
}

std::vector<HtmlHeader> extract_headers(GumboNode *node)
{
    std::vector<HtmlHeader> headers;

    if (node->type != GUMBO_NODE_ELEMENT)
        return headers;

    GumboTag tag = node->v.element.tag;
    if (tag >= GUMBO_TAG_H1 && tag <= GUMBO_TAG_H6)
    {
        const char *name = gumbo_normalized_tagname(tag);
        HtmlHeader h;
        h.tag = name;
        h.textContent = get_text_content(node);
        h.color = "#000000"; // заполним позже
        headers.push_back(h);
    }

    GumboVector *children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; ++i)
    {
        GumboNode *child = (GumboNode *)children->data[i];
        std::vector<HtmlHeader> child_headers = extract_headers(child);
        headers.insert(headers.end(), child_headers.begin(), child_headers.end());
    }

    return headers;
}