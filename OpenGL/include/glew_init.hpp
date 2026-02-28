#pragma once

#include <GL/glew.h>
#include <stdexcept>

inline void init_glew_or_throw()
{
    glewExperimental = GL_TRUE;
    
    const GLenum rc = glewInit();
    if (rc != GLEW_OK)
        throw std::runtime_error("Failed to init GLEW");
}