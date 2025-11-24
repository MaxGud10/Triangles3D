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
