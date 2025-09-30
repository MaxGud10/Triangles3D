cmake_minimum_required(VERSION 3.11)

project("Triangles")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include/)

add_executable(triag src/main.cpp src/config.cpp)


enable_testing()

find_package(GTest REQUIRED)
add_executable(google_test src/google_test.cpp)
target_link_libraries(google_test GTest::gtest_main)

add_test(NAME google_test COMMAND google_test)


add_test(
  NAME e2e_all
  COMMAND ${CMAKE_COMMAND} -E env
          PYTHONUNBUFFERED=1
          python3 ${CMAKE_SOURCE_DIR}/end2end/run_e2e_tests.py
          $<TARGET_FILE:triag>
)

set_tests_properties(google_test PROPERTIES LABELS "unit")
set_tests_properties(e2e_all    PROPERTIES LABELS "e2e")
