#include <iostream>
#include <cstring>

#ifndef restrict
#define restrict
#endif

extern "C" {
#include <libcss/libcss.h>
}

// --- минимальный resolve_url ---
static css_error resolve_url(void *pw, const char *base, lwc_string *rel, lwc_string **abs) {
    *abs = lwc_string_ref(rel);
    return CSS_OK;
}

// --- базовые DOM-заглушки ---
static css_error node_name(void*, void*, css_qname *qname) {
    lwc_intern_string("h1", 2, &qname->name);
    qname->ns = nullptr;
    return CSS_OK;
}
static css_error node_classes(void*, void*, lwc_string ***classes, uint32_t *n) { *classes=nullptr; *n=0; return CSS_OK; }
static css_error node_id(void*, void*, lwc_string **id) { *id=nullptr; return CSS_OK; }
static css_error named_ancestor_node(void*, void*, const css_qname*, void **a){*a=nullptr;return CSS_OK;}
static css_error named_parent_node(void*, void*, const css_qname*, void **a){*a=nullptr;return CSS_OK;}
static css_error named_sibling_node(void*, void*, const css_qname*, void **a){*a=nullptr;return CSS_OK;}
static css_error node_has_name(void*, void*, const css_qname*, bool *m){*m=true;return CSS_OK;}
static css_error node_is_root(void*, void*, bool *m){*m=false;return CSS_OK;}
static css_error node_has_class(void*, void*, lwc_string*, bool *m){*m=false;return CSS_OK;}
static css_error node_has_id(void*, void*, lwc_string*, bool *m){*m=false;return CSS_OK;}

// --- 🔥 новые обязательные функции (libcss ≥0.9) ---
static css_error parent_node(void*, void*, void **parent){*parent=nullptr;return CSS_OK;}
static css_error sibling_node(void*, void*, void **sib){*sib=nullptr;return CSS_OK;}
static css_error get_libcss_node_data(void*, void*, void **data){*data=nullptr;return CSS_OK;}
static css_error set_libcss_node_data(void*, void*, void *data){(void)data;return CSS_OK;}
static css_error ua_default_for_property(void*, uint32_t, css_hint*){return CSS_PROPERTY_NOT_SET;}

int main() {
    const char* css_text = "body { color: red; }";

    // 1️⃣ Создаём таблицу стилей
    css_stylesheet_params params{};
    params.params_version = CSS_STYLESHEET_PARAMS_VERSION_1;
    params.url = "about:blank";
    params.resolve = resolve_url;

    css_stylesheet* sheet = nullptr;
    css_error err = css_stylesheet_create(&params, &sheet);
    if (err != CSS_OK) {
        std::cerr << "❌ Ошибка создания CSS: " << err << std::endl;
        return 1;
    }

    css_stylesheet_append_data(sheet, (const uint8_t*)css_text, std::strlen(css_text));
    css_stylesheet_data_done(sheet);

    css_select_ctx* select_ctx = nullptr;
    css_select_ctx_create(&select_ctx);
    css_select_ctx_append_sheet(select_ctx, sheet, CSS_ORIGIN_AUTHOR, "all");

    css_select_handler handler{};
    handler.node_name = node_name;
    handler.node_classes = node_classes;
    handler.node_id = node_id;
    handler.named_ancestor_node = named_ancestor_node;
    handler.named_parent_node = named_parent_node;
    handler.named_sibling_node = named_sibling_node;
    handler.node_has_name = node_has_name;
    handler.node_is_root = node_is_root;
    handler.node_has_class = node_has_class;
    handler.node_has_id = node_has_id;
    handler.parent_node = parent_node;
    handler.sibling_node = sibling_node;
    handler.get_libcss_node_data = get_libcss_node_data;
    handler.set_libcss_node_data = set_libcss_node_data;
    handler.ua_default_for_property = ua_default_for_property;

    css_select_results* results = nullptr;
    err = css_select_style(
        select_ctx,
        (void*)"body",
        nullptr, nullptr, nullptr,
        &handler, nullptr, &results
    );

    if (err == CSS_OK)
        std::cout << "✅ CSS applied successfully!" << std::endl;
    else
        std::cerr << "❌ Ошибка применения CSS: " << err << std::endl;

    // 6️⃣ Очистка
    if (results) css_select_results_destroy(results);
    css_select_ctx_destroy(select_ctx);
    css_stylesheet_destroy(sheet);
    return 0;
}
