#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

// #include "camera.hpp"

const uint32_t SCREEN_WIDTH  = 1000;
const uint32_t SCREEN_HEIGHT = 1000;
const char* SCREEN_NAME = "TRIANGLES BRO.";

int main(void) {
    if (!glfwInit()) {
        return -1;
    }
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_NAME, nullptr, nullptr);
    if (!window) {
        std::cout << "Error creating window!" << "\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window); // context is an object
                                    // that contains important info for running our program

    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}
