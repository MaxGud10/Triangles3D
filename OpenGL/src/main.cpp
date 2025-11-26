// src/main.cpp

#define GLM_ENABLE_EXPERIMENTAL

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <deque>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"
#include "camera.hpp"

#include "octotree.hpp"
#include "triangles.hpp"


const uint32_t SCREEN_WIDTH  = 1000;
const uint32_t SCREEN_HEIGHT = 800;
const char*    SCREEN_NAME   = "TRIANGLES VISUALIZER";

using PointTy    = double;
using Triangle3D = triangle::Triangle<PointTy>;


bool load_triangles_from_stream(std::istream& in,
                                std::vector<Triangle3D>& triangles)
{
    size_t triag_num = 0;
    if (!(in >> triag_num))
    {
        std::cerr << "Failed to read triangle count\n";
        return false;
    }

    triangles.clear();
    triangles.reserve(triag_num);

    for (size_t i = 0; i < triag_num; ++i)
    {
        PointTy x1, y1, z1;
        PointTy x2, y2, z2;
        PointTy x3, y3, z3;

        if (!(in >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3))
        {
            std::cerr << "Failed to read triangle " << i << "\n";
            return false;
        }

        Triangle3D tri(x1, y1, z1,
                       x2, y2, z2,
                       x3, y3, z3);
        tri.id = i;
        triangles.push_back(tri);
    }

    return true;
}

std::vector<bool> mark_intersected_triangles(const std::vector<Triangle3D>& triangles)
{
    using triangle::Octotree;
    using triangle::BoundingBox;

    const size_t n = triangles.size();
    std::vector<bool> intersected(n, false);

    if (n == 0)
        return intersected;

    std::vector<Triangle3D> input = triangles;

    Octotree<PointTy> octotree(input, input.size());
    octotree.divide_tree();

    std::map<size_t, size_t> result;
    std::deque<BoundingBox<PointTy>> cells = octotree.get_cells();

    for (auto cell : cells)
    {
        cell.group_intersections(result);
    }

    for (auto& [id, _] : result)
    {
        if (id < intersected.size())
            intersected[id] = true;
    }

    return intersected;
}

void build_vertex_buffer(const std::vector<Triangle3D>& triangles,
                         const std::vector<bool>& intersected,
                         std::vector<float>& vertices)
{
    vertices.clear();
    vertices.reserve(triangles.size() * 3 * 9);

    for (size_t i = 0; i < triangles.size(); ++i)
    {
        const auto& tri = triangles[i];
        const auto& A = tri.get_a();
        const auto& B = tri.get_b();
        const auto& C = tri.get_c();

        glm::vec3 v1(B.x - A.x, B.y - A.y, B.z - A.z);
        glm::vec3 v2(C.x - A.x, C.y - A.y, C.z - A.z);
        glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

        float r, g, b;
        if (intersected[i]) { r = 0.9f; g = 0.1f; b = 0.1f; }
        else                { r = 0.7f; g = 0.7f; b = 0.7f; }

        auto pushV = [&](const triangle::Point<PointTy>& p)
        {
            vertices.push_back((float)p.x);
            vertices.push_back((float)p.y);
            vertices.push_back((float)p.z);

            vertices.push_back(normal.x);
            vertices.push_back(normal.y);
            vertices.push_back(normal.z);

            vertices.push_back(r);
            vertices.push_back(g);
            vertices.push_back(b);
        };

        pushV(A);
        pushV(B);
        pushV(C);
    }
}


int main(int argc, char** argv)
{
    std::vector<Triangle3D> triangles;

    if (argc > 1)
    {
        std::ifstream fin(argv[1]);
        if (!fin.is_open())
        {
            std::cerr << "Failed to open file: " << argv[1] << "\n";
            return -1;
        }
        if (!load_triangles_from_stream(fin, triangles))
        {
            std::cerr << "Error while reading triangles from file\n";
            return -1;
        }
    }
    else
    {
        if (!load_triangles_from_stream(std::cin, triangles))
        {
            std::cerr << "Error while reading triangles from stdin\n";
            return -1;
        }
    }

    std::cout << "Loaded " << triangles.size() << " triangles\n";

    std::vector<bool> intersected = mark_intersected_triangles(triangles);

    std::vector<float> vertices;
    build_vertex_buffer(triangles, intersected, vertices);

    if (vertices.empty())
    {
        std::cerr << "No vertices to draw\n";
        return 0;
    }

    if (!glfwInit())
    {
        std::cerr << "Failed to init GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_NAME, nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Error creating window!\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to init GLEW\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glEnable(GL_DEPTH_TEST);

    Shader shaderProgram("../shaders/vertex.vert", "../shaders/fragment.frag");

    VAO vao;
    vao.Bind();


    VBO vbo(reinterpret_cast<GLfloat*>(vertices.data()),
            vertices.size() * sizeof(float));

    vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 9 * sizeof(float), (void*)0);
    vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    vao.LinkAttrib(vbo, 2, 3, GL_FLOAT, 9 * sizeof(float), (void*)(6 * sizeof(float)));

    vao.Unbind();
    vbo.Unbind();

    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 5.0f));

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(0.1f, 0.12f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderProgram.Activate();
        glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), 5.0f, 5.0f, 5.0f);

        camera.Inputs(window);
        camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

        vao.Bind();

        GLsizei vertexCount = static_cast<GLsizei>(vertices.size() / 6); 
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);

        glfwSwapBuffers(window);
    }

    vao.Delete();
    vbo.Delete();
    shaderProgram.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
