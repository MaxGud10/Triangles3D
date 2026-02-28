#define GLM_ENABLE_EXPERIMENTAL

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <unordered_set>
#include <vector>

#include "viewer.hpp"
#include "shader.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "camera.hpp"
#include "glfw_wrapper.hpp"
#include "glew_init.hpp"

namespace triag_viewer
{
static constexpr uint32_t    screen_width  = 1000;
static constexpr uint32_t    screen_height = 800;
static constexpr const char *screen_name   = "TRIANGLES VISUALIZER";

struct TriangleRaw
{
    glm::vec3 a;
    glm::vec3 b;
    glm::vec3 c;
};

static std::vector<TriangleRaw> to_raw(const std::vector<triangle::ShapeWithId<double>> &shapes)
{
    std::vector<TriangleRaw> tris;
    tris.reserve(shapes.size());

    for (const auto &s : shapes)
    {
        if (auto t = std::get_if<triangle::Triangle<double>>(&s.shape))
        {
            TriangleRaw r;

            r.a = glm::vec3((float)t->get_a().x, (float)t->get_a().y, (float)t->get_a().z);
            r.b = glm::vec3((float)t->get_b().x, (float)t->get_b().y, (float)t->get_b().z);
            r.c = glm::vec3((float)t->get_c().x, (float)t->get_c().y, (float)t->get_c().z);

            tris.push_back(r);
        }
    }

    return tris;
}

static void build_vertex_buffer(const std::vector<TriangleRaw>   &triangles,
                                const std::unordered_set<size_t> &intersected_ids,
                                std::vector<float>               &vertices)
{
    vertices.clear();
    vertices.reserve(triangles.size() * 3 * 9);

    for (size_t i = 0; i < triangles.size(); ++i)
    {
        const auto &tri = triangles[i];

        const glm::vec3 A = tri.a;
        const glm::vec3 B = tri.b;
        const glm::vec3 C = tri.c;

        const glm::vec3 v1 = B - A;
        const glm::vec3 v2 = C - A;

        glm::vec3 normal = glm::cross(v1, v2);
        const float len  = glm::length(normal);

        if (len > 0.0f)
            normal /= len;
        else
            normal = glm::vec3(0.0f, 0.0f, 1.0f);

        float r = 0.1f, g = 0.2f, b = 0.9f; // blue
        if (intersected_ids.contains(i))
        {
            r = 0.9f;
            g = 0.1f;
            b = 0.1f; // red
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

static SceneBounds compute_scene_bounds(const std::vector<TriangleRaw> &triangles)
{
    SceneBounds b;
    b.min = glm::vec3( std::numeric_limits<float>::infinity());
    b.max = glm::vec3(-std::numeric_limits<float>::infinity());

    auto expand = [&](const glm::vec3& p)
    {
        b.min.x = std::min(b.min.x, p.x);
        b.min.y = std::min(b.min.y, p.y);
        b.min.z = std::min(b.min.z, p.z);

        b.max.x = std::max(b.max.x, p.x);
        b.max.y = std::max(b.max.y, p.y);
        b.max.z = std::max(b.max.z, p.z);
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

static void build_bbox_lines_vertices(const SceneBounds    &b,
                                        std::vector<float> &out_vertices,
                                        const glm::vec3    &color,
                                        float               padding = 0.0f)
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

int run_opengl_viewer(const std::vector<triangle::ShapeWithId<double>> &shapes,
                      const std::unordered_set<size_t>                 &intersected_ids)
{
    const auto triangles = to_raw(shapes);

    std::vector<float> vertices;
    build_vertex_buffer(triangles, intersected_ids, vertices);

    if (vertices.empty())
        throw std::runtime_error("No triangle vertices to draw");

    const SceneBounds bounds = compute_scene_bounds(triangles);

    const glm::vec3 center = (bounds.min + bounds.max) * 0.5f;
    const glm::vec3 diag   = (bounds.max - bounds.min);
    const float     radius = glm::length(diag) * 0.5f;

    const float fov_deg = 45.0f;
    const float fov_rad = glm::radians(fov_deg);
          float dist    = radius / std::tan(fov_rad * 0.5f);

    if (!std::isfinite(dist) || dist < 1.0f)
        dist = 5.0f;

    dist *= 0.6f;

    std::vector<float> bbox_vertices;
    build_bbox_lines_vertices(bounds,
                              bbox_vertices,
                              glm::vec3(0.2f, 0.6f, 1.0f),
                              0.0f);

    GlfwContext glfw;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GlfwWindow window(screen_width, screen_height, screen_name);
    window.make_current();

    init_glew_or_throw();

    int width  = 0;
    int height = 0;

    glfwGetFramebufferSize(window.get(), &width, &height);
    glViewport(0, 0, width, height);

    glEnable(GL_DEPTH_TEST);

    Shader shaderProgram("shaders/vertex.vert", "shaders/fragment.frag");

    VAO vao;
    vao.bind();

    VBO vbo(vertices.data(),
            static_cast<GLsizeiptr>(vertices.size() * sizeof(float)));

    vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 9 * sizeof(float), (void*)0);
    vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    vao.LinkAttrib(vbo, 2, 3, GL_FLOAT, 9 * sizeof(float), (void*)(6 * sizeof(float)));

    VAO::unbind();
    VBO::unbind();

    VAO bbox_vao;
    bbox_vao.bind();

    VBO bbox_vbo(bbox_vertices.data(),
                    static_cast<GLsizeiptr>(bbox_vertices.size() * sizeof(float)));

    bbox_vao.LinkAttrib(bbox_vbo, 0, 3, GL_FLOAT, 9 * sizeof(float), (void*)0);
    bbox_vao.LinkAttrib(bbox_vbo, 1, 3, GL_FLOAT, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    bbox_vao.LinkAttrib(bbox_vbo, 2, 3, GL_FLOAT, 9 * sizeof(float), (void*)(6 * sizeof(float)));

    VAO::unbind();
    VBO::unbind();

    Camera camera(width, height, center + glm::vec3(0.0f, 0.0f, dist));
    camera.LookAt(center);

    const float near_plane = std::max(0.01f, dist * 0.01f);
    const float far_plane  = dist + radius * 4.0f + 10.0f;

    while (!window.should_close())
    {
        glfwPollEvents();

        glClearColor(0.1f, 0.12f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderProgram.Activate();
        glUniform3f(glGetUniformLocation(shaderProgram.id(), "lightPos"), 5.0f, 5.0f, 5.0f);

        camera.Inputs(window.get());
        camera.SetMatrix(45.0f, near_plane, far_plane, shaderProgram, "camMatrix");

        vao.bind();
        const GLsizei tri_vertex_count = static_cast<GLsizei>(vertices.size() / 9);
        glDrawArrays(GL_TRIANGLES, 0, tri_vertex_count);

        glDisable(GL_DEPTH_TEST);
        bbox_vao.bind();
        const GLsizei bbox_vertex_count = static_cast<GLsizei>(bbox_vertices.size() / 9);
        glDrawArrays(GL_LINES, 0, bbox_vertex_count);
        glEnable(GL_DEPTH_TEST);

        window.swap_buffers();
    }

    return 0;
}

} // namespace triag_viewer