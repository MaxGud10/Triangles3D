find_package(GTest REQUIRED)

add_executable(google_test_calc
    ${CMAKE_CURRENT_SOURCE_DIR}/unit/google_test_calc.cpp
)

target_link_libraries(google_test_calc PRIVATE
    GTest::gtest_main
    triangles_headers
)

add_test(NAME unit_calc COMMAND google_test_calc)
set_tests_properties(unit_calc PROPERTIES LABELS "unit")

if (GRAPHICS)
    add_executable(google_test_gl
        ${CMAKE_CURRENT_SOURCE_DIR}/unit/google_test_gl.cpp
    )

    target_link_libraries(google_test_gl PRIVATE
        GTest::gtest_main
        triangles_headers
        OpenGL::GL
        ${GLEW_LIBRARIES}
        ${GLFW_LIBRARIES}
    )

    target_include_directories(google_test_gl PRIVATE
        ${CMAKE_SOURCE_DIR}/OpenGL/include
        ${GLEW_INCLUDE_DIRS}
        ${GLFW_INCLUDE_DIRS}
    )

    target_link_directories(google_test_gl PRIVATE
        ${GLEW_LIBRARY_DIRS}
        ${GLFW_LIBRARY_DIRS}
    )

    add_test(NAME unit_gl COMMAND google_test_gl)
    set_tests_properties(unit_gl PROPERTIES LABELS "unit")
endif()


add_test(
  NAME e2e_all
  COMMAND ${CMAKE_COMMAND} -E env
          PYTHONUNBUFFERED=1
          python3 ${CMAKE_CURRENT_SOURCE_DIR}/end2end/run_e2e_tests.py
          $<TARGET_FILE:triag>
)

set_tests_properties(e2e_all PROPERTIES LABELS "e2e")