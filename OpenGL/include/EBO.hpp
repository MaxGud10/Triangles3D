#pragma once

#include <GL/glew.h>

#include <utility>

class EBO
{
    GLuint id_ = 0;

    void reset() noexcept
    {
        if (id_ != 0)
        {
            glDeleteBuffers(1, &id_);
            id_ = 0;
        }
    }

public:
    EBO() = default;

    EBO(const GLuint* indices, GLsizeiptr size)
    {
        glGenBuffers(1, &id_);
        if (id_ == 0)
            return;

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
    }

    ~EBO() noexcept { reset(); }

    EBO(const EBO&)            = delete;
    EBO& operator=(const EBO&) = delete;

    EBO(EBO&& other) noexcept
        : id_(std::exchange(other.id_, 0)) {}

    EBO& operator=(EBO&& other) noexcept
    {
        if (this != &other)
        {
            reset();
            id_ = std::exchange(other.id_, 0);
        }

        return *this;
    }

    void bind() const noexcept
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
    }

    static void unbind() noexcept
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    GLuint id() const noexcept { return id_; }
};