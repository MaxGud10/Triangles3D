#include "camera.hpp"

void Camera::Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char *uniform) {
    // view is a
    glm::mat4 view = glm::lookAt(
        camPos,          // this is camera position
        camPos + camDir, // this is the point where camera looks at
        upDir            // up direction vector
    );
    glm::mat4 projection = glm::perspective(
        glm::radians(FOVdeg),                   // the fov angle
        static_cast<float>(width / height),     // screen aspect
        nearPlane,
        farPlane);

    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(projection * view));
}

void Camera::Inputs(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camPos += speed * camDir;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camPos += speed * (-glm::normalize(glm::cross(camDir, upDir)));
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camPos += speed * -camDir;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camPos += speed * glm::normalize(glm::cross(camDir, upDir));
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camPos += speed * upDir;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        camPos += speed * -upDir;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        speed = 0.4f;
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
        speed = 0.1f;
    }



}
