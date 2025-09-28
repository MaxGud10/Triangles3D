cmake_minimum_required(VERSION 3.11)

project("Triangles")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include/)

add_executable(triag src/main.cpp src/config.cpp)

enable_testing()
add_test(NAME google_test COMMAND google_test)

find_package(GTest REQUIRED)
enable_testing()
add_executable(google_test src/google_test.cpp)

target_link_libraries(google_test GTest::gtest_main)