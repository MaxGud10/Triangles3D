#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec3 Color;

out vec4 FragColor;

uniform vec3 lightPos  = vec3(5.0, 5.0, 5.0);
uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);

void main()
{
    float ambientStrength = 0.25;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * Color;

    FragColor = vec4(result, 1.0);
}
