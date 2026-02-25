cmake_minimum_required(VERSION 3.10)
project(OpenGL)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)

cmake_policy(SET CMP0072 NEW)
set(OpenGL_GL_PREFERENCE GLVND)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

add_executable(main
    src/main.cpp
    src/shader.cpp
    src/VAO.cpp
    src/VBO.cpp
    src/EBO.cpp
    src/camera.cpp
)

target_include_directories(main PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${GLEW_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
)

target_link_directories(main PRIVATE
    ${GLEW_LIBRARY_DIRS}
    ${GLFW_LIBRARY_DIRS}
)

target_link_libraries(main PRIVATE
    OpenGL::GL
    ${GLEW_LIBRARIES}
    ${GLFW_LIBRARIES}
)