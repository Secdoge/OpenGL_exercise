#version 330 core

in vec3 color;
in vec2 tex;

out vec4 FragColor;

uniform float mixParam;

uniform sampler2D tex1;
uniform sampler2D tex2;

void main()
{
    // FragColor = texture(tex1, tex) * vec4(color, mixParam);
    // FragColor = vec4(color, 1.0f);
    FragColor = mix(texture(tex1, tex), texture(tex2, tex), mixParam);
}