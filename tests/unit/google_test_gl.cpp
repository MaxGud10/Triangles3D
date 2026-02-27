#include <gtest/gtest.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "shader.hpp"
#include "VAO.hpp"
#include "VBO.hpp"

// -------------------------
// helpers
// -------------------------
static void write_text_file(const std::string& path, const std::string& text)
{
    std::ofstream out(path, std::ios::binary);
    if (!out.is_open())
        throw std::runtime_error("Failed to create file: " + path);

    out << text;
    out.close();
}

static void expect_no_gl_error(const char* where)
{
    const GLenum err = glGetError();
    EXPECT_EQ(err, GL_NO_ERROR) << where << " glGetError() = " << err;
}

// -------------------------
// fixture: one GL context for tests
// -------------------------
class GLContextFixture : public ::testing::Test
{
protected:
    GLFWwindow* window_ = nullptr;

    void SetUp() override
    {
        if (glfwInit() != GLFW_TRUE)
            throw std::runtime_error("glfwInit failed");

        // invisible window + core profile
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window_ = glfwCreateWindow(64, 64, "unit_gl", nullptr, nullptr);
        if (!window_)
        {
            glfwTerminate();
            throw std::runtime_error("glfwCreateWindow failed");
        }

        glfwMakeContextCurrent(window_);

        glewExperimental = GL_TRUE;
        const GLenum glew_ok = glewInit();
        if (glew_ok != GLEW_OK)
        {
            glfwDestroyWindow(window_);
            window_ = nullptr;
            glfwTerminate();

            throw std::runtime_error(
                std::string("glewInit failed: ") +
                reinterpret_cast<const char*>(glewGetErrorString(glew_ok)));
        }

        // after glewInit бывает GL_INVALID_ENUM — чистим
        while (glGetError() != GL_NO_ERROR) {}

        glViewport(0, 0, 64, 64);
        glDisable(GL_DEPTH_TEST);

        expect_no_gl_error("SetUp");
    }

    void TearDown() override
    {
        if (window_)
        {
            glfwDestroyWindow(window_);
            window_ = nullptr;
        }

        glfwTerminate();
    }
};

// -------------------------
// tests
// -------------------------
TEST_F(GLContextFixture, CanCompileLinkShaderProgram)
{
    // делаем временные шейдеры рядом с тестом (cwd = build)
    const std::string vpath = "tmp_unit_vertex.vert";
    const std::string fpath = "tmp_unit_fragment.frag";

    write_text_file(vpath,
        "#version 330 core\n"
        "layout(location = 0) in vec3 aPos;\n"
        "void main() { gl_Position = vec4(aPos, 1.0); }\n"
    );

    write_text_file(fpath,
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main() { FragColor = vec4(1.0, 0.0, 0.0, 1.0); }\n"
    );

    Shader shader(vpath.c_str(), fpath.c_str());
    shader.Activate();

    EXPECT_NE(shader.id(), 0u);

    expect_no_gl_error("CanCompileLinkShaderProgram");
}

TEST_F(GLContextFixture, CanCreateBuffersAndDrawTriangleWithoutErrors)
{
    const std::string vpath = "tmp_unit_vertex.vert";
    const std::string fpath = "tmp_unit_fragment.frag";

    write_text_file(vpath,
        "#version 330 core\n"
        "layout(location = 0) in vec3 aPos;\n"
        "void main() { gl_Position = vec4(aPos, 1.0); }\n"
    );

    write_text_file(fpath,
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main() { FragColor = vec4(0.2, 0.8, 0.3, 1.0); }\n"
    );

    Shader shader(vpath.c_str(), fpath.c_str());

    // 3 вершины, только position
    const std::vector<float> vertices = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    VAO vao;
    vao.bind();

    VBO vbo(const_cast<GLfloat*>(vertices.data()),
            static_cast<GLsizeiptr>(vertices.size() * sizeof(float)));

    // layout=0, vec3, stride = 3 float
    vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

    vao.unbind();
    vbo.unbind();

    glClear(GL_COLOR_BUFFER_BIT);
    shader.Activate();
    vao.bind();

    glDrawArrays(GL_TRIANGLES, 0, 3);
    glFinish();

    expect_no_gl_error("CanCreateBuffersAndDrawTriangleWithoutErrors");
}