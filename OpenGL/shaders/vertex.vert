#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aColor;

out vec3 FragPos;
out vec3 Normal;
out vec3 Color;

uniform mat4 camMatrix;

void main()
{
    FragPos = aPos;
    Normal = aNormal;
    Color = aColor;

    gl_Position = camMatrix * vec4(aPos, 1.0);
}
