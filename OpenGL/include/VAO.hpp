#pragma once

#include <utility>
#include <stdexcept>
#include <GL/glew.h>

#include "VBO.hpp"

class VAO
{
    GLuint id_ = 0;

    void reset() noexcept
    {
        if (id_ != 0)
        {
            glDeleteVertexArrays(1, &id_);
            id_ = 0;
        }
    }

public:
    VAO() { glGenVertexArrays(1, &id_); }

    ~VAO() noexcept {reset();}

    VAO(const VAO&)            = delete;
    VAO& operator=(const VAO&) = delete;

    VAO(VAO&& other) noexcept
      : id_(std::exchange(other.id_, 0)) {}

    VAO& operator=(VAO&& other) noexcept
    {
        if (this != &other)
        {
            reset();
            id_ = std::exchange(other.id_, 0);
        }
        return *this;
    }

           void bind() const noexcept { glBindVertexArray(id_); }
    static void unbind()     noexcept { glBindVertexArray(0);   }

    GLuint id() const noexcept { return id_; }

    void LinkAttrib(const VBO&  vbo,
                    GLuint      layout,
                    GLint       numComponents,
                    GLenum      type,
                    GLsizei     stride,
                    const void* offset) const noexcept
    {
        vbo.bind();
        glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
        glEnableVertexAttribArray(layout);
        VBO::unbind();
    }
};