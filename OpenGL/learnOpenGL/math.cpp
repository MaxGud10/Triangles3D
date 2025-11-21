#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
#include <vector>

void drawCircle(float cx, float cy, float r, int numSegments) 
{
    glBegin(GL_TRIANGLE_FAN); // Используем GL_TRIANGLE_FAN для рисования круга
    glVertex2f(cx, cy); // Центр круга

    for (int i = 0; i <= numSegments; i++) {
        float theta = 2.0f * M_PI * float(i) / float(numSegments); // Угол
        float x = r * cosf(theta); // Координата X
        float y = r * sinf(theta); // Координата Y
        glVertex2f(x + cx, y + cy); // Вершина круга
    }

    glEnd();
}

int main() 
{
    // Инициализация GLFW
    if (!glfwInit()) {
        std::cerr << "Ошибка инициализации GLFW" << std::endl;
        return -1;
    }

    int width = 800;
    int height = 600;

    // Создание окна
    GLFWwindow* window = glfwCreateWindow(width, height, "Falling Circle", NULL, NULL);
    if (!window) 
    {
        std::cerr << "Ошибка создания окна" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Установка текущего контекста
    glfwMakeContextCurrent(window);

    // Настройка проекции
    // int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1); // 2D-проекция
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Начальные параметры круга
    float circleRadius = 50.0f;   // Радиус круга

    float mass = 15.0f;
    std::vector<float> position = {(float)(width / 2), 0.0f};
    std::vector<float> velocity = {(float)(100/mass), 0.0f};


    // Основной цикл
    while (!glfwWindowShouldClose(window)) 
    {
        // Очистка буфера цвета
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Рисование круга
        drawCircle(position[0], position[1], circleRadius, 100);

        // Обновление позиции круга
        position[0] += velocity[0];
        position[1] += velocity[1];
        // velocity[0] += 9.81 / mass / 100;
        velocity[1] += 9.81 / mass;


        // Если круг выходит за нижнюю границу окна, сбрасываем его вверх
        if(position[1] < 0 || position[1] > height) 
        {
          velocity[1] *= -0.95;
        }
        
        // if(position[0] < 0 || position[0] > width) {
        //   velocity[0] *= -0.8;
        // }
        
        if (position[0] - circleRadius > width-50) 
        {
          position[0] = 50;
        }
        // Обмен буферов
        glfwSwapBuffers(window);

        // Обработка событий
        glfwPollEvents();
    }

    // Очистка ресурсов
    glfwTerminate();
    return 0;
}