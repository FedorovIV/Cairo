
# Cairo HTML + CSS Mini Engine

This project demonstrates a simple HTML + CSS layout engine using Cairo for rendering.

## Requirements
- C++ Compiler
- Cairo library
- CMake
- PkgConfig (for Cairo detection)

## Building
1. Install Cairo and dependencies.
2. Run:
   ```
   cmake .
   make
   ```
3. Run the executable:
   ```
   ./cairo_html_css_example
   ```
4. The result will be saved as `output.png`.

Commands:

```
g++ html_css_demo.cpp -o html_css_demo $(pkg-config --cflags --libs libcss) -lgumbo -Drestrict=
```

```
g++ main.cpp -o main $(pkg-config --cflags --libs libcss) -lgumbo -Drestrict=
```

```
g++ main.cpp -o main \
  $(pkg-config --cflags --libs libcss gumbo cairo) \
  -lwapcaplet -Drestrict=
```