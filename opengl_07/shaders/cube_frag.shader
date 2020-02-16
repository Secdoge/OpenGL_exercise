#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 objectColor = vec3(1.0f);
uniform vec3 viewPos;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};
struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;

void main()
{
    // Ambient
    vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * light.ambient;

    // Diffuse
    // first get the direction vector
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    // then get the diffuse
    float diff = max(dot(norm, lightDir), 0.0);                                 // keep positive
    vec3 diffuse = vec3(texture(material.diffuse, TexCoords)) * diff * light.diffuse;

    // Specular
    // first get reflect firection
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    // then get specular
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    vec3 specular = vec3(texture(material.specular, TexCoords)) * spec * light.specular;

    // Emission
    vec3 emission = vec3(texture(material.emission, TexCoords));

    // Result
    vec3 result = (ambient + diffuse + specular + emission) * objectColor;
    FragColor = vec4(result, 1.0f);
}