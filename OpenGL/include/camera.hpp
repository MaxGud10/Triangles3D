#pragma once


#define GLM_ENABLE_EXPERIMENTAL

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "shader.hpp"

class Camera {

    public: // default camera parameters
        int width;
        int height;

        glm::vec3 camPos; // camera position
        glm::vec3 camDir = glm::vec3(0.0f, 0.0f, -1.0f); // camera direction (or orientation)
        glm::vec3 upDir  = glm::vec3(0.0f, 1.0f, 0.0f);


        float speed     = 0.1f;
        float sensivity = 0.1f;

        float yaw       = -90.0f;
        float pitch     = 0.0f;

        float lastX;
        float lastY;
        bool  firstClick = true;


        Camera(int widthIn, int heightIn, glm::vec3 posIn) : width(widthIn), height(heightIn), camPos(posIn) {}

        void Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform);
        void Inputs(GLFWwindow* window);

};
