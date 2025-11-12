#include <stdio.h>
#include <stdlib.h>

/* Читает весь файл в нуль-терминированный буфер.
 * Возвращает malloc-нутый указатель (нужно free()) или NULL при ошибке.
 * out_len опционален.
 */
static char* read_file_all(const char* path, size_t* out_len) {
    FILE* f = fopen(path, "rb");
    if (!f) {
        fprintf(stderr, "Не удалось открыть файл: %s\n", path);
        return NULL;
    }

    if (fseek(f, 0, SEEK_END) != 0) {
        fprintf(stderr, "fseek конец не удался: %s\n", path);
        fclose(f);
        return NULL;
    }

    long sz = ftell(f);
    if (sz < 0) {
        fprintf(stderr, "ftell не удался: %s\n", path);
        fclose(f);
        return NULL;
    }
    rewind(f);

    char* buf = (char*)malloc((size_t)sz + 1);
    if (!buf) {
        fprintf(stderr, "Нет памяти для чтения: %s\n", path);
        fclose(f);
        return NULL;
    }

    size_t readn = fread(buf, 1, (size_t)sz, f);
    fclose(f);

    if (readn != (size_t)sz) {
        fprintf(stderr, "Прочитано меньше, чем ожидалось (%zu/%ld): %s\n",
                readn, sz, path);
        free(buf);
        return NULL;
    }

    buf[sz] = '\0';
    if (out_len) *out_len = (size_t)sz;
    return buf;
}