#pragma once

#include <GL/glew.h>
#include <utility>

class VBO
{
    GLuint id_ = 0;

public:
    VBO() = default;

    VBO(const void* data, GLsizeiptr  size, GLenum usage = GL_STATIC_DRAW)
    {
        glGenBuffers(1, &id_);
        glBindBuffer(GL_ARRAY_BUFFER, id_);
        glBufferData(GL_ARRAY_BUFFER, size, data, usage);
    }

    ~VBO() noexcept {reset();}

    VBO(const VBO&)            = delete;
    VBO& operator=(const VBO&) = delete;

    VBO(VBO&& other) noexcept
        : id_(std::exchange(other.id_, 0)) {}

    VBO& operator=(VBO&& other) noexcept
    {
        if (this != &other)
        {
            reset();
            id_ = std::exchange(other.id_, 0);
        }

        return *this;
    }

           void bind() const noexcept{ glBindBuffer(GL_ARRAY_BUFFER, id_);}
    static void unbind()     noexcept{ glBindBuffer(GL_ARRAY_BUFFER, 0);  }

    void reset() noexcept
    {
        if (id_ != 0)
        {
            glDeleteBuffers(1, &id_);
            id_ = 0;
        }
    }

    void set_data(const void* data, GLsizeiptr size, GLenum usage = GL_STATIC_DRAW)
    {
        if (id_ == 0)
            glGenBuffers(1, &id_);

        glBindBuffer(GL_ARRAY_BUFFER, id_);
        glBufferData(GL_ARRAY_BUFFER, size, data, usage);
    }

    GLuint id()    const noexcept { return id_;      }
    bool   valid() const noexcept { return id_ != 0; }
};