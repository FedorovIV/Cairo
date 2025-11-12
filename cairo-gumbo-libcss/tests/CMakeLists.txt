include(FetchContent)
FetchContent_Declare(
  catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.6.0
)
FetchContent_MakeAvailable(catch2)

add_executable(test_element test_element.cpp)
target_link_libraries(test_element PRIVATE element Catch2::Catch2WithMain)

include(Catch)
catch_discover_tests(test_element)