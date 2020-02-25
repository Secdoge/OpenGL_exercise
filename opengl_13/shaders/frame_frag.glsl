#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

const float offset = 1.0f / 300.0f;

void main()
{
    vec2 offsets[9] = vec2[] (
        vec2 (-offset,  offset),    // left top
        vec2 (0.0f,     offset),    // top middle
        vec2 (offset,   offset),    // right top
        vec2 (-offset,  0.0f),      // left
        vec2 (0.0f,     0.0f),      // middle
        vec2 (offset,   0.0f),      // right
        vec2 (-offset,  -offset),   // left bottom
        vec2 (0.0f,     -offset),   // bottom middle
        vec2 (offset,   -offset)    // right bottm
    );
    float kernel[9] = float[] (
         2,  2,  2,
         2,-15,  2,
         2,  2,  2
    );

    vec3 sampleTex[9];
    for (int i = 0; i < 9; i++) {
        sampleTex[i] = texture(screenTexture, TexCoords.st + offsets[i]).rgb;
    }

    vec3 col = vec3(0.0f);
    for (int i= 0; i < 9; i++) {
        col += sampleTex[i] * kernel[i];
    }

    FragColor = vec4(col, 1.0f);
}