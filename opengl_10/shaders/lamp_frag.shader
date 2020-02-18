#version 330 core

out vec4 FragColor;

uniform vec3 color;

void main()
{
    // FragColor = texture(tex1, tex) * vec4(color, mixParam);
    // FragColor = vec4(color, 1.0f);
    FragColor = vec4(color, 1.0f);
}