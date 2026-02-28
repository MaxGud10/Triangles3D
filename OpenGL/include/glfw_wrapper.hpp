#pragma once

#include <GLFW/glfw3.h>

#include <stdexcept>
#include <utility>

class GlfwContext
{
public:
    GlfwContext()
    {
        if (!glfwInit())
        throw std::runtime_error("Failed to init GLFW");
    }

    ~GlfwContext() noexcept
    {
        glfwTerminate();
    }

    GlfwContext(const GlfwContext&)            = delete;
    GlfwContextoperator=(const GlfwContext&) = delete;

    GlfwContext(GlfwContext&&)            = delete;
    GlfwContext& operator=(GlfwContext&&) = delete;
};

class GlfwWindow
{
    GLFWwindow* window = nullptr;

public:
    GlfwWindow(int width, int height, const char *title)
    {
        window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (!window)
            throw std::runtime_error("Failed to create GLFW window");
    }

    ~GlfwWindow() noexcept
    {
        if (window)
            glfwDestroyWindow(window);
    }

    GlfwWindow(const GlfwWindow&)            = delete;
    GlfwWindow &operator=(const GlfwWindow&) = delete;

    GlfwWindow(GlfwWindow &&other) noexcept
        : window(std::exchange(other.window, nullptr)) {}

    GlfwWindow &operator=(GlfwWindow &&other) noexcept
    {
        if (this != &other)
        {
            if (window)
                glfwDestroyWindow(window);

            window = std::exchange(other.window, nullptr);
        }

        return *this;
    }

    GLFWwindow *get() const noexcept { return window; }

    void make_current() const
    {
        glfwMakeContextCurrent(window);

        if (glfwGetCurrentContext() != window)
            throw std::runtime_error("glfwMakeContextCurrent failed");
    }

    bool should_close() const noexcept
    {
        return glfwWindowShouldClose(window);
    }

    void swap_buffers() const noexcept
    {
        glfwSwapBuffers(window);
    }
};