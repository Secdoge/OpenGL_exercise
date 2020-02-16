#version 330 core

in vec3 color;
uniform vec4 globalColor;
out vec4 FragColor;

void main()
{
    FragColor = globalColor;
}