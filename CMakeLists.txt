cmake_minimum_required(VERSION 3.15)
project(cairo_example CXX)

# Укажи путь к твоему vcpkg (измени при необходимости)
set(VCPKG_ROOT "C:/Dev/vcpkg")
set(CAIRO_INCLUDE_DIR "${VCPKG_ROOT}/installed/x64-windows/include")
set(CAIRO_LIB_DIR "${VCPKG_ROOT}/installed/x64-windows/lib")

# Сообщаем CMake, где искать заголовки и библиотеки
add_executable(cairo_example main.cpp)
target_include_directories(cairo_example PRIVATE ${CAIRO_INCLUDE_DIR})
target_link_directories(cairo_example PRIVATE ${CAIRO_LIB_DIR})
target_link_libraries(cairo_example PRIVATE cairo)
