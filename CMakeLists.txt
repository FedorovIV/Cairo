cmake_minimum_required(VERSION 3.15)
project(cairo_html_css_example CXX)

# Указание на правильный путь к MSYS2 и Cairo
set(CAIRO_INCLUDE_DIR "C:/msys64/mingw64/include")
set(CAIRO_LIB_DIR "C:/msys64/mingw64/lib")

# Указываем путь к pkg-config
set(PKG_CONFIG_PATH "C:/msys64/mingw64/lib/pkgconfig")

# Сообщаем CMake, где искать заголовки и библиотеки
include_directories(${CAIRO_INCLUDE_DIR})
link_directories(${CAIRO_LIB_DIR})

# Создаем исполнимый файл
add_executable(cairo_html_css_example main.cpp HTMLParser.cpp CSSParser.cpp LayoutEngine.cpp Renderer.cpp)

# Линкуем с библиотекой Cairo
target_link_libraries(cairo_html_css_example PRIVATE cairo)

# Проверка на pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(CAIRO cairo)

