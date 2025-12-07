cmake_minimum_required(VERSION 3.20)
project(cairo LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- libraries ---

add_library(element src/element/element.cpp)
target_include_directories(element PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/element)

add_library(renderer src/renderer/cairo_renderer.cpp)
target_include_directories(renderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/renderer ${CMAKE_CURRENT_SOURCE_DIR}/src/element)
target_link_libraries(renderer PRIVATE cairo)

# --- main executable ---

add_executable(main main.cpp)

# Линкуем все библиотеки и зависимости
target_link_libraries(main
    PRIVATE
    renderer
    element
    ${DEPS_LIBRARIES}
)
target_include_directories(main PRIVATE ${DEPS_INCLUDE_DIRS})

# Чтобы избежать ошибки restrict при C++
target_compile_definitions(main PRIVATE restrict=)
