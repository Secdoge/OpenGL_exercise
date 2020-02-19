#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{
    FragColor = vec4(0.4f, 0.4f, 0.1f, 0.5f);
}