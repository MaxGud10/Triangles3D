#include <GLFW/glfw3.h>
#include <cmath>
// #include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Настройки
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

glm::vec3 cameraPos = glm::vec3(0.0f, 3.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, -0.5f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Углы Эйлера
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

void processInput(GLFWwindow* window) 
{
    float cameraSpeed = 2.5f * deltaTime;
    
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
        
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) 
{
    if (firstMouse) 
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f) pitch = 89.0f;
    if(pitch < -89.0f) pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void drawGrid() 
{
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_LINES);
    for(int i = -100; i <= 100; i++) 
    {
        glVertex3f(i, 0.0f, -100.0f);
        glVertex3f(i, 0.0f, 100.0f);
        glVertex3f(-100.0f, 0.0f, i);
        glVertex3f(100.0f, 0.0f, i);
    }

    glEnd();
}

void drawTriangle() 
{
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.5f, 0.0f, 1.0f);

    glEnd();
}

int main() 
{
    if (!glfwInit()) 
        return -1;

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "3D Scene", NULL, NULL);
    if (!window) 
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) 
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Матрицы проекции
        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f), 
            (float)SCR_WIDTH/(float)SCR_HEIGHT, 
            0.1f, 100.0f
        );
        
        glm::mat4 view = glm::lookAt(
            cameraPos,
            cameraPos + cameraFront,
            cameraUp
        );

        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(&projection[0][0]);
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(&view[0][0]);

        drawGrid();
        drawTriangle();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}