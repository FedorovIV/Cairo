
cmake_minimum_required(VERSION 3.15)
project(cairo_html_css_example CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules(CAIRO REQUIRED cairo)

add_executable(cairo_html_css_example main.cpp)

target_include_directories(cairo_html_css_example PRIVATE ${CAIRO_INCLUDE_DIRS})
target_link_libraries(cairo_html_css_example PRIVATE ${CAIRO_LIBRARIES})
