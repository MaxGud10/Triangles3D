cmake_minimum_required(VERSION 3.10)
project(OpenGLProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

include_directories(include)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLU REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

set(GLAD_SOURCE src/glad.c)
set(GLAD_INCLUDE include/glad)

include_directories(${GLEW_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${GLAD_INCLUDE})
link_directories(${GLEW_LIBRARY_DIRS} ${GLFW_LIBRARY_DIRS} ${GLUT_LIBRARY_DIRS})


# -------- function to create executables --------
function(make_program name source)
    add_executable(${name} ${source})
target_link_libraries(${name}
    OpenGL::GL
    GLU
    ${GLEW_LIBRARIES}
    ${GLFW_LIBRARIES}
    ${GLUT_LIBRARIES}
)
endfunction()

# -------- individual programs --------
make_program(camera1 learnOpenGL/camera1.cpp)
make_program(camera2 learnOpenGL/camera2.cpp)
make_program(hello_triangle learnOpenGL/hello_triangle.cpp)
make_program(math_prog learnOpenGL/math.cpp)
make_program(shaders_1 learnOpenGL/shaders_1.cpp)
make_program(shaders_2 learnOpenGL/shaders_2.cpp)
make_program(main src/main.cpp)
