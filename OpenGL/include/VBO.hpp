#pragma once

#include <GL/glew.h>

class VBO {

    public:
        GLuint ID;
        VBO(GLfloat *vertices, GLsizeiptr size);

        void Bind();
        void Unbind();
        void Delete();
};
