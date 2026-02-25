cmake_minimum_required(VERSION 3.16)
project(Triangles LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_definitions(PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

add_executable(triag
    src/cli/main.cpp
)

target_include_directories(triag PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

enable_testing()

find_package(GTest REQUIRED)
add_executable(google_test src/google_test.cpp)

target_include_directories(google_test PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(google_test PRIVATE GTest::gtest_main)
add_test(NAME google_test COMMAND google_test)

add_test(
  NAME e2e_all
  COMMAND ${CMAKE_COMMAND} -E env
          PYTHONUNBUFFERED=1
          python3 ${CMAKE_SOURCE_DIR}/end2end/run_e2e_tests.py
          $<TARGET_FILE:triag>
)

add_custom_target(check
  COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
)
add_dependencies(check triag google_test)


option(GRAPHICS "Build OpenGL viewer" OFF)
message(STATUS "GRAPHICS enabled: ${GRAPHICS}")

if (GRAPHICS)
    cmake_policy(SET CMP0072 NEW)
    set(OpenGL_GL_PREFERENCE GLVND)

    find_package(OpenGL REQUIRED)
    find_package(GLEW REQUIRED)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GLFW REQUIRED glfw3)

    add_executable(opengl_viewer
        src/opengl/main.cpp
    )

    target_include_directories(opengl_viewer PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/OpenGL/include
        ${GLEW_INCLUDE_DIRS}
        ${GLFW_INCLUDE_DIRS}
    )

    target_link_directories(opengl_viewer PRIVATE
        ${GLEW_LIBRARY_DIRS}
        ${GLFW_LIBRARY_DIRS}
    )

    target_link_libraries(opengl_viewer PRIVATE
        OpenGL::GL
        ${GLEW_LIBRARIES}
        ${GLFW_LIBRARIES}
    )

    add_custom_command(TARGET opengl_viewer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
                $<TARGET_FILE_DIR:opengl_viewer>/shaders
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/OpenGL/shaders
                $<TARGET_FILE_DIR:opengl_viewer>/shaders
    )
endif()