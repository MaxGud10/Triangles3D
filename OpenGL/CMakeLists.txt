cmake_minimum_required(VERSION 3.16)
project(OpenGL LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

cmake_policy(SET CMP0072 NEW)
set(OpenGL_GL_PREFERENCE GLVND)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(GLFW REQUIRED glfw3)

add_executable(opengl_viewer
    src/main.cpp
)

target_include_directories(opengl_viewer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${GLFW_INCLUDE_DIRS}
)

target_link_libraries(opengl_viewer PRIVATE
    OpenGL::OpenGL
    OpenGL::GLX
    GLEW::GLEW
)

if (TARGET PkgConfig::GLFW)
    target_link_libraries(opengl_viewer PRIVATE PkgConfig::GLFW)
else()
    target_link_libraries(opengl_viewer PRIVATE ${GLFW_LIBRARIES})
    target_link_directories(opengl_viewer PRIVATE ${GLFW_LIBRARY_DIRS})
    target_compile_options(opengl_viewer PRIVATE ${GLFW_CFLAGS_OTHER})
endif()

target_compile_options(opengl_viewer PRIVATE
    $<$<CONFIG:Debug>:-Wall -Wextra -Wpedantic>
)