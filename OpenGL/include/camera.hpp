#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <algorithm>

#include "shader.hpp"

class Camera
{
    int width_  = 1;
    int height_ = 1;

    glm::vec3 pos_ = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 dir_ = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up_  = glm::vec3(0.0f, 1.0f, 0.0f);

    float base_speed_   = 0.1f;
    float fast_speed_   = 0.4f;
    float sensitivity_  = 0.1f;

    float yaw_   = -90.0f;
    float pitch_ = 0.0f;

    double last_x_ = 0.0;
    double last_y_ = 0.0;

    bool first_mouse_ = true;

    static float clamp_pitch(float pitch) noexcept
    {
        return std::clamp(pitch, -89.0f, 89.0f);
    }

    void update_direction_from_angles() noexcept
    {
        glm::vec3 d;
        d.x = std::cos(glm::radians(yaw_)) * std::cos(glm::radians(pitch_));
        d.y = std::sin(glm::radians(pitch_));
        d.z = std::sin(glm::radians(yaw_)) * std::cos(glm::radians(pitch_));
        dir_ = glm::normalize(d);
    }

public:
    Camera(int width, int height, glm::vec3 pos)
        : width_ (std::max(1, width)),
          height_(std::max(1, height)),
          pos_   (pos) {}

    void Resize(int width, int height) noexcept
    {
        width_  = std::max(1, width);
        height_ = std::max(1, height);
    }

    const glm::vec3& position()  const noexcept { return pos_; }
    const glm::vec3& direction() const noexcept { return dir_; }

    void SetMatrix(float fov_deg, float near_plane, float far_plane,
                   const Shader& shader, const char* uniform) const
    {
        const glm::mat4 view = glm::lookAt(pos_, pos_ + dir_, up_);

        const float aspect =
            static_cast<float>(width_) / static_cast<float>(height_);

        const glm::mat4 proj = glm::perspective(glm::radians(fov_deg),
                                                aspect,
                                                near_plane,
                                                far_plane);

        const GLint loc = glGetUniformLocation(shader.id(), uniform);
        if (loc >= 0)
        {
            const glm::mat4 vp = proj * view;
            glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(vp));
        }
    }

    void Inputs(GLFWwindow* window)
    {
        const float speed =
            (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
                ? fast_speed_
                : base_speed_;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            pos_ += speed * dir_;

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            pos_ -= speed * dir_;

        const glm::vec3 right = glm::normalize(glm::cross(dir_, up_));

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            pos_ += speed * right;

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            pos_ -= speed * right;

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            pos_ += speed * up_;

        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
            pos_ -= speed * up_;

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

            double x = 0.0;
            double y = 0.0;
            glfwGetCursorPos(window, &x, &y);

            if (first_mouse_)
            {
                last_x_ = x;
                last_y_ = y;
                first_mouse_ = false;

                return;
            }

            const float dx = static_cast<float>(x - last_x_);
            const float dy = static_cast<float>(last_y_ - y);

            last_x_ = x;
            last_y_ = y;

            yaw_   += dx * sensitivity_;
            pitch_ += dy * sensitivity_;
            pitch_  = clamp_pitch(pitch_);

            update_direction_from_angles();
        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            first_mouse_ = true;
        }
    }
};