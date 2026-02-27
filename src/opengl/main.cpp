#define GLM_ENABLE_EXPERIMENTAL

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cerrno>
#include <cstdio>
#include <cstdlib>

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <vector>
#include <limits>
#include <cmath>

#include "shader.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "camera.hpp"

#ifndef TRIAG_EXE_PATH
#define TRIAG_EXE_PATH "./triag"
#endif

static constexpr uint32_t screen_width  = 1000;
static constexpr uint32_t screen_height = 800;
static constexpr const char* screen_name = "TRIANGLES VISUALIZER";

using PointTy = double;

struct Point3
{
    PointTy x = 0;
    PointTy y = 0;
    PointTy z = 0;
};

struct TriangleRaw
{
    Point3 a;
    Point3 b;
    Point3 c;
};

static std::vector<TriangleRaw> read_triangles(std::istream& in, std::string& as_text_for_triag)
{
    size_t triag_num = 0;
    if (!(in >> triag_num))
        throw std::runtime_error("Failed to read triangle count");

    std::vector<TriangleRaw> tris;
    tris.reserve(triag_num);

    std::ostringstream ss;
    ss << triag_num << "\n";

    for (size_t i = 0; i < triag_num; ++i)
    {
        TriangleRaw t;

        if (!(in >> t.a.x >> t.a.y >> t.a.z
                 >> t.b.x >> t.b.y >> t.b.z
                 >> t.c.x >> t.c.y >> t.c.z))
        {
            throw std::runtime_error("Failed to read triangle " + std::to_string(i));
        }

        ss << t.a.x << " " << t.a.y << " " << t.a.z << " "
           << t.b.x << " " << t.b.y << " " << t.b.z << " "
           << t.c.x << " " << t.c.y << " " << t.c.z << "\n";

        tris.push_back(t);
    }

    as_text_for_triag = ss.str();
    return tris;
}

static std::unordered_set<size_t> run_triag_and_get_intersections(const std::string& triangles_text)
{
    const std::string tmp_in = "/tmp/triangles_input_for_triag.txt";

    {
        std::ofstream out(tmp_in, std::ios::binary);
        if (!out.is_open())
            throw std::runtime_error("Failed to create temp file: " + tmp_in);

        out << triangles_text;
    }

    std::string cmd = std::string(TRIAG_EXE_PATH) + " < " + tmp_in;

    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe)
        throw std::runtime_error("popen failed (triag). errno=" + std::to_string(errno));

    std::unordered_set<size_t> ids;
    try
    {
        char buf[4096];
        while (std::fgets(buf, sizeof(buf), pipe))
        {
            std::istringstream line(buf);

            size_t id = 0;
            if (line >> id)
                ids.insert(id);
        }
    }
    catch (...)
    {
        pclose(pipe);
        throw;
    }

    const int rc = pclose(pipe);
    if (rc != 0)
    {
        throw std::runtime_error("triag returned non-zero exit code: " + std::to_string(rc));
    }

    return ids;
}

static void build_vertex_buffer(const std::vector<TriangleRaw>& triangles,
                                const std::unordered_set<size_t>& intersected_ids,
                                std::vector<float>& vertices)
{
    vertices.clear();
    vertices.reserve(triangles.size() * 3 * 9);

    for (size_t i = 0; i < triangles.size(); ++i)
    {
        const auto& tri = triangles[i];

        const glm::vec3 A((float)tri.a.x, (float)tri.a.y, (float)tri.a.z);
        const glm::vec3 B((float)tri.b.x, (float)tri.b.y, (float)tri.b.z);
        const glm::vec3 C((float)tri.c.x, (float)tri.c.y, (float)tri.c.z);

        const glm::vec3 v1 = B - A;
        const glm::vec3 v2 = C - A;

        glm::vec3 normal = glm::cross(v1, v2);
        const float len = glm::length(normal);
        if (len > 0.0f)
            normal /= len;
        else
            normal = glm::vec3(0.0f, 0.0f, 1.0f);

        float r = 0.1f, g = 0.2f, b = 0.9f;
        if (intersected_ids.contains(i))
        {
            r = 0.9f; g = 0.1f; b = 0.1f;
        }

        auto pushV = [&](const glm::vec3& p)
        {
            vertices.push_back(p.x);
            vertices.push_back(p.y);
            vertices.push_back(p.z);

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

struct SceneBounds
{
    glm::vec3 min;
    glm::vec3 max;
};

static SceneBounds compute_scene_bounds(const std::vector<TriangleRaw>& triangles)
{
    SceneBounds b;
    b.min = glm::vec3( std::numeric_limits<float>::infinity());
    b.max = glm::vec3(-std::numeric_limits<float>::infinity());

    auto expand = [&](const Point3& p)
    {
        b.min.x = std::min(b.min.x, static_cast<float>(p.x));
        b.min.y = std::min(b.min.y, static_cast<float>(p.y));
        b.min.z = std::min(b.min.z, static_cast<float>(p.z));

        b.max.x = std::max(b.max.x, static_cast<float>(p.x));
        b.max.y = std::max(b.max.y, static_cast<float>(p.y));
        b.max.z = std::max(b.max.z, static_cast<float>(p.z));
    };

    for (const auto& t : triangles)
    {
        expand(t.a);
        expand(t.b);
        expand(t.c);
    }

    if (!std::isfinite(b.min.x) || !std::isfinite(b.max.x))
    {
        b.min = glm::vec3(-1.0f);
        b.max = glm::vec3( 1.0f);
    }

    return b;
}

// линии bbox рисуем тем же layout: pos(3) normal(3) color(3) => 9 float
void build_bbox_lines_vertices(const SceneBounds& b,
                               std::vector<float>& out_vertices,
                               const glm::vec3& color,
                               float padding = 0.0f)
{
    out_vertices.clear();

    const glm::vec3 mn = b.min - glm::vec3(padding);
    const glm::vec3 mx = b.max + glm::vec3(padding);

    const glm::vec3 v000(mn.x, mn.y, mn.z);
    const glm::vec3 v100(mx.x, mn.y, mn.z);
    const glm::vec3 v010(mn.x, mx.y, mn.z);
    const glm::vec3 v110(mx.x, mx.y, mn.z);

    const glm::vec3 v001(mn.x, mn.y, mx.z);
    const glm::vec3 v101(mx.x, mn.y, mx.z);
    const glm::vec3 v011(mn.x, mx.y, mx.z);
    const glm::vec3 v111(mx.x, mx.y, mx.z);

    auto push_vertex = [&](const glm::vec3& p)
    {
        out_vertices.push_back(p.x);
        out_vertices.push_back(p.y);
        out_vertices.push_back(p.z);

        out_vertices.push_back(0.0f);
        out_vertices.push_back(0.0f);
        out_vertices.push_back(0.0f);

        out_vertices.push_back(color.x);
        out_vertices.push_back(color.y);
        out_vertices.push_back(color.z);
    };

    auto push_edge = [&](const glm::vec3& a, const glm::vec3& c)
    {
        push_vertex(a);
        push_vertex(c);
    };

    push_edge(v000, v100);
    push_edge(v100, v110);
    push_edge(v110, v010);
    push_edge(v010, v000);

    push_edge(v001, v101);
    push_edge(v101, v111);
    push_edge(v111, v011);
    push_edge(v011, v001);

    push_edge(v000, v001);
    push_edge(v100, v101);
    push_edge(v110, v111);
    push_edge(v010, v011);
}

int main(int argc, char** argv)
{
    std::vector<TriangleRaw> triangles;
    std::string triangles_text_for_triag;

    try
    {
        if (argc > 1)
        {
            std::ifstream fin(argv[1]);
            if (!fin.is_open())
                throw std::runtime_error(std::string("Failed to open file: ") + argv[1]);

            triangles = read_triangles(fin, triangles_text_for_triag);
        }
        else
        {
            triangles = read_triangles(std::cin, triangles_text_for_triag);
        }

        std::cout << "Loaded " << triangles.size() << " triangles\n";

        const auto intersected_ids = run_triag_and_get_intersections(triangles_text_for_triag);

        std::vector<float> vertices;
        build_vertex_buffer(triangles, intersected_ids, vertices);

        if (vertices.empty())
        {
            std::cerr << "No vertices to draw\n";
            return 0;
        }

        SceneBounds bounds = compute_scene_bounds(triangles);

        std::vector<float> bbox_vertices;
        build_bbox_lines_vertices(bounds, bbox_vertices,
                                glm::vec3(0.2f, 0.6f, 1.0f),
                                0.0f);

        if (!glfwInit())
        {
            std::cerr << "Failed to init GLFW\n";
            return -1;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        GLFWwindow* window = glfwCreateWindow(screen_width, screen_height, screen_name, nullptr, nullptr);
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

        int width = 0, height = 0;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        glEnable(GL_DEPTH_TEST);

        Shader shaderProgram("shaders/vertex.vert", "shaders/fragment.frag");

        VAO vao;
        vao.bind();

        VBO vbo(reinterpret_cast<GLfloat*>(vertices.data()),
                static_cast<GLsizeiptr>(vertices.size() * sizeof(float)));

        vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 9 * sizeof(float), (void*)0);
        vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, 9 * sizeof(float), (void*)(3 * sizeof(float)));
        vao.LinkAttrib(vbo, 2, 3, GL_FLOAT, 9 * sizeof(float), (void*)(6 * sizeof(float)));

        vao.unbind();
        vbo.unbind();

        VAO bbox_vao;
        bbox_vao.bind();

        VBO bbox_vbo(bbox_vertices.data(),
                    static_cast<GLsizeiptr>(bbox_vertices.size() * sizeof(float)));

        bbox_vao.LinkAttrib(bbox_vbo, 0, 3, GL_FLOAT, 9 * sizeof(float), (void*)0);
        bbox_vao.LinkAttrib(bbox_vbo, 1, 3, GL_FLOAT, 9 * sizeof(float), (void*)(3 * sizeof(float)));
        bbox_vao.LinkAttrib(bbox_vbo, 2, 3, GL_FLOAT, 9 * sizeof(float), (void*)(6 * sizeof(float)));

        bbox_vao.unbind();
        bbox_vbo.unbind();

        Camera camera(width, height, glm::vec3(0.0f, 0.0f, 5.0f));

        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();

            glClearColor(0.1f, 0.12f, 0.15f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            shaderProgram.Activate();
            glUniform3f(glGetUniformLocation(shaderProgram.id(), "lightPos"), 5.0f, 5.0f, 5.0f);

            camera.Inputs(window);
            camera.SetMatrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

            vao.bind();

            const GLsizei vertexCount = static_cast<GLsizei>(vertices.size() / 9);
            glDrawArrays(GL_TRIANGLES, 0, vertexCount);

            vao.bind();
            const GLsizei tri_vertex_count = static_cast<GLsizei>(vertices.size() / 9);
            glDrawArrays(GL_TRIANGLES, 0, tri_vertex_count);

            glDisable(GL_DEPTH_TEST);
            bbox_vao.bind();
            const GLsizei bbox_vertex_count = static_cast<GLsizei>(bbox_vertices.size() / 9);
            glDrawArrays(GL_LINES, 0, bbox_vertex_count);
            glEnable(GL_DEPTH_TEST);

            glfwSwapBuffers(window);
        }

        glfwDestroyWindow(window);
        glfwTerminate();
        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    }
}