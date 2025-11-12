#include <catch2/catch_test_macros.hpp>
#include "element.h"

TEST_CASE("Root has (0,0) by default", "[Element]") {
    Element root(nullptr, 800.0, 600.0);
    REQUIRE(root.x() == 0.0);
    REQUIRE(root.y() == 0.0);
    REQUIRE(root.width() == 800.0);
    REQUIRE(root.height() == 600.0);
}

TEST_CASE("Child inherits parent's position", "[Element]") {
    Element root(nullptr, 800.0, 600.0);
    root.set_position(10.0, 20.0);

    Element child(&root, 200.0, 100.0);
    REQUIRE(child.x() == 10.0);
    REQUIRE(child.y() == 20.0);
    REQUIRE(child.width() == 200.0);
    REQUIRE(child.height() == 100.0);
}

TEST_CASE("Manual setters work", "[Element]") {
    Element e(nullptr);
    e.set_position(3.5, 7.25);
    e.set_size(50.0, 40.0);
    REQUIRE(e.x() == 3.5);
    REQUIRE(e.y() == 7.25);
    REQUIRE(e.width() == 50.0);
    REQUIRE(e.height() == 40.0);
}
