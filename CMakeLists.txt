cmake_minimum_required(VERSION 3.16)
project(Triangles LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_definitions(PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

add_library(triangles_headers INTERFACE)
target_include_directories(triangles_headers INTERFACE
    ${CMAKE_SOURCE_DIR}/include
)

add_executable(triag
    src/main.cpp
)
target_link_libraries(triag PRIVATE triangles_headers)

option(GRAPHICS "Build OpenGL support inside triag (--opengl)" OFF)
message(STATUS "GRAPHICS enabled: ${GRAPHICS}")

if (GRAPHICS)
    cmake_policy(SET CMP0072 NEW)
    set(OpenGL_GL_PREFERENCE GLVND)

    find_package(OpenGL REQUIRED)
    find_package(GLEW REQUIRED)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GLFW REQUIRED glfw3)

    target_sources(triag PRIVATE
        src/opengl/viewer.cpp
    )

    target_compile_definitions(triag PRIVATE TRIAG_HAS_GRAPHICS=1)

    target_link_libraries(triag PRIVATE
        OpenGL::GL
        ${GLEW_LIBRARIES}
        ${GLFW_LIBRARIES}
    )

    target_include_directories(triag PRIVATE
        ${CMAKE_SOURCE_DIR}/OpenGL/include
        ${GLEW_INCLUDE_DIRS}
        ${GLFW_INCLUDE_DIRS}
    )

    target_link_directories(triag PRIVATE
        ${GLEW_LIBRARY_DIRS}
        ${GLFW_LIBRARY_DIRS}
    )

    add_custom_command(TARGET triag POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
                $<TARGET_FILE_DIR:triag>/shaders
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/OpenGL/shaders
                $<TARGET_FILE_DIR:triag>/shaders
    )
endif()

enable_testing()
add_subdirectory(tests)

add_custom_target(check
  COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
)
add_dependencies(check triag)