cmake_minimum_required(VERSION 3.20)
project(html_cairo LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# pkg-config для зависимостей
find_package(PkgConfig REQUIRED)
pkg_check_modules(DEPS REQUIRED libcss gumbo cairo)

# -----------------------
# Библиотеки проекта
# -----------------------


add_library(read_files src/read-files/read_files.cpp)
target_include_directories(read_files PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/read-files)

add_library(find_headers src/find-headers/find_headers.cpp)
target_include_directories(find_headers PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/find-headers)

add_library(libcss_main src/libcss-main/libcss_main.c)
set_target_properties(libcss_main PROPERTIES LINKER_LANGUAGE C)
target_include_directories(libcss_main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/libcss-main)

# -----------------------
# Исполняемый файл
# -----------------------
add_executable(main main.cpp)
target_link_libraries(main
    PRIVATE
        read_files
        find_headers
        libcss_main
        ${DEPS_LIBRARIES}
        wapcaplet
)
target_include_directories(main PRIVATE ${DEPS_INCLUDE_DIRS})

# Чтобы "заглушить" restrict для C++
target_compile_definitions(main PRIVATE restrict=)
