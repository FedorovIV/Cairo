g++ main.cpp -o main   $(pkg-config --cflags --libs libcss gumbo cairo)   -lwapcaplet -Drestrict=

cmake --build build -j

cmake -S . -B build