cmake_minimum_required(VERSION 3.20)
project(html_cairo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(element src/element/element.cpp)
target_include_directories(element PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/element)

add_library(find_headers src/find-headers/find_headers.cpp)
target_include_directories(find_headers PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/find-headers)

add_library(read_files src/read-files/read_files.cpp)
target_include_directories(read_files PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/read-files)

# --- libcss_main (C code) ---
add_library(libcss_main src/libcss-main/libcss_main.c)
set_target_properties(libcss_main PROPERTIES LINKER_LANGUAGE C)
target_include_directories(libcss_main PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/libcss-main
)

# pkg-config для libcss
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBCSS REQUIRED libcss)
target_include_directories(libcss_main PRIVATE ${LIBCSS_INCLUDE_DIRS})
target_link_libraries(libcss_main PRIVATE ${LIBCSS_LIBRARIES})

# --- основное приложение ---
add_executable(main main.cpp)
target_link_libraries(main PRIVATE element find_headers libcss_main read_files)

# --- Тесты ---
enable_testing()
add_subdirectory(tests)
