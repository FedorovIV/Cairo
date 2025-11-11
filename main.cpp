#include "libcss_headers.cpp"

int main(void)
{
    const char css[] =
        "h1 { color: blue } "
        "h2 { color: #321; } "
        "h4, h5 { color: #123456; }";

    size_t count;
    header_color_t *headers = get_headers_colors(css, &count);

    if (!headers)
    {
        fprintf(stderr, "Ошибка при парсинге CSS\n");
        return 1;
    }

    for (size_t i = 0; i < count; i++)
    {
        printf("%s → %s\n", headers[i].tag, headers[i].color);
    }

    for (size_t i = 0; i < count; i++)
    {
        free((void *)headers[i].tag);
    }
    free(headers);
    return 0;
}
