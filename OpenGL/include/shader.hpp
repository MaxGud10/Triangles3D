#pragma once

#include <GL/glew.h>

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

inline std::string read_text_file(const char* filename)
{
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open())
        throw std::runtime_error(std::string("Failed to open file: ") + filename);

    std::ostringstream ss;
    ss << in.rdbuf();

    return ss.str();
}

inline std::string shader_info_log(GLuint shader)
{
    GLint len = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

    std::string log;
    if (len > 1)
    {
        log.resize(static_cast<size_t>(len));
        glGetShaderInfoLog(shader, len, nullptr, log.data());
    }

    return log;
}

inline std::string program_info_log(GLuint program)
{
    GLint len = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);

    std::string log;
    if (len > 1)
    {
        log.resize(static_cast<size_t>(len));
        glGetProgramInfoLog(program, len, nullptr, log.data());
    }

    return log;
}

inline GLuint compile_shader(GLenum type, const char* src, const char* debug_name)
{
    GLuint shader = glCreateShader(type);
    if (shader == 0)
        throw std::runtime_error("glCreateShader failed");

    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint ok = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (ok != GL_TRUE)
    {
        std::string log = shader_info_log(shader);
        glDeleteShader(shader);

        throw std::runtime_error(
            std::string("Shader compile failed (") + debug_name + "):\n" + log);
    }

    return shader;
}

class Shader
{
    GLuint id_ = 0;

    void reset() noexcept
    {
        if (id_ != 0)
        {
            glDeleteProgram(id_);
            id_ = 0;
        }
    }

public:
    Shader(const char* vertexFile, const char* fragmentFile)
    {
        const std::string vertexCode   = read_text_file(vertexFile);
        const std::string fragmentCode = read_text_file(fragmentFile);

        const char* vsrc = vertexCode.c_str();
        const char* fsrc = fragmentCode.c_str();

        GLuint vs = 0;
        GLuint fs = 0;

        try
        {
            vs = compile_shader(GL_VERTEX_SHADER,   vsrc, vertexFile);
            fs = compile_shader(GL_FRAGMENT_SHADER, fsrc, fragmentFile);

            id_ = glCreateProgram();
            if (id_ == 0)
                throw std::runtime_error("glCreateProgram failed");

            glAttachShader(id_, vs);
            glAttachShader(id_, fs);
            glLinkProgram(id_);

            GLint ok = GL_FALSE;
            glGetProgramiv(id_, GL_LINK_STATUS, &ok);
            if (ok != GL_TRUE)
                throw std::runtime_error(std::string("Program link failed:\n") + program_info_log(id_));
        }
        catch (...)
        {
            reset();

            if (vs != 0) glDeleteShader(vs);
            if (fs != 0) glDeleteShader(fs);

            throw;
        }

        if (vs != 0) glDeleteShader(vs);
        if (fs != 0) glDeleteShader(fs);
    }

    ~Shader() noexcept { reset(); }

    Shader(const Shader&)            = delete;
    Shader& operator=(const Shader&) = delete;

    Shader(Shader&& other) noexcept
        : id_(std::exchange(other.id_, 0)) {}

    Shader& operator=(Shader&& other) noexcept
    {
        if (this != &other)
        {
            reset();
            id_ = std::exchange(other.id_, 0);
        }

        return *this;
    }

           void Activate() const noexcept { glUseProgram(id_); }
    static void Deactivate()     noexcept { glUseProgram(0);   }

    GLuint id() const noexcept { return id_; }
};