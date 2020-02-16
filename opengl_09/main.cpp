#define STB_IMAGE_IMPLEMENTATION
// Headers
// ---------------------------------------------------------------------------------------------------------------------
// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// Standard Headers
#include <iostream>
#include <cmath>
// Other Headers
#include "src/stb_image.h"                                                      // texture
#include "src/shader.h"                                                         // shader
#include "src/camera.h"                                                         // camera
#include <glm/glm.hpp>                                                          // vec&matrix
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Settings
// ---------------------------------------------------------------------------------------------------------------------
// basic attributes
const unsigned int SCR_WIDTH    = 800;                                          // window
const unsigned int SCR_HEIGHT   = 600;
float deltaTime                 = 0.0f;                                         // timer
float lastFrame                 = 0.0f;
float lastX                     = 400;
float lastY                     = 300;                                          // cursor
bool firstMouse                 = true;
// basic functions
void framebuffer_size_callback  (GLFWwindow* window, int width, int height);    // Call-back function statement
void processInput               (GLFWwindow* window);
void mouse_callback             (GLFWwindow* window, double xpos, double ypos);
void scroll_callback            (GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture        (const char *path);
// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

enum Mode {
    DESERT,
    FACTORY,
    HORROR,
    BIOCHEMIC,
};
Mode mode = BIOCHEMIC;

// Main
// ---------------------------------------------------------------------------------------------------------------------
int main ()
{
    // glfw initialization
    // -------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                              // Use OpenGL3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);              // Use Core-Profile
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);                        // especially for Mac
    // Create a window
    // ---------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL", nullptr, nullptr);
    if (nullptr == window) {
        std::cout << "Failed to create a GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);                                             // set to current thread
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);          // register a call-back function
    // glad initialization
    // -------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to init glad" << std::endl;
        return -1;
    }
    // Cursor Control
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);                // disable cursor
    glfwSetCursorPosCallback(window, mouse_callback);                           // set function able
    glfwSetScrollCallback(window, scroll_callback);
    glEnable(GL_DEPTH_TEST);


    // get Vertex Attributes
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum of vertex attributes supported: " << nrAttributes << std::endl;


    // Build & Compile shader program
    // ------------------------------
    Shader shader1 = Shader("../shaders/cube_vert.shader", "../shaders/cube_frag_multi.shader");
    Shader lampshader = Shader("../shaders/lamp_vert.shader", "../shaders/lamp_frag.shader");
    // Setup vertex data
    // -----------------
    float vertices[] = {
            // positions          // normals           // texture coords
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
    };
    // positions all containers
    glm::vec3 cubePositions[] = {
            glm::vec3( 0.0f,  0.0f,  0.0f),
            glm::vec3( 2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3( 1.3f, -2.0f, -2.5f),
            glm::vec3( 1.5f,  2.0f, -2.5f),
            glm::vec3( 1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f),
    };
    // positions of the point lights
    glm::vec3 pointLightPositions[] = {
            glm::vec3( 0.7f,  0.2f,  2.0f),
            glm::vec3( 2.3f, -3.3f, -4.0f),
            glm::vec3(-4.0f,  2.0f, -12.0f),
            glm::vec3( 0.0f,  0.0f, -3.0f),
    };
    glm::vec3 colors_desert[] = {
            glm::vec3( 0.8f,  0.1f,  0.1f),
            glm::vec3( 0.8f,  0.4f,  0.2f),
            glm::vec3( 0.8f,  0.4f,  0.2f),
            glm::vec3( 0.8f,  0.1f,  0.1f),
    };
    glm::vec3 colors_factory[] = {
            glm::vec3( 0.3f,  0.1f,  0.8f),
            glm::vec3( 0.3f,  0.1f,  0.8f),
            glm::vec3( 0.3f,  0.1f,  0.8f),
            glm::vec3( 0.3f,  0.1f,  0.8f),
    };
    glm::vec3 colors_horror[] = {
            glm::vec3( 0.4f,  0.1f,  0.1f),
            glm::vec3( 0.4f,  0.1f,  0.1f),
            glm::vec3( 0.4f,  0.1f,  0.1f),
            glm::vec3( 0.4f,  0.1f,  0.1f),
    };
    glm::vec3 colors_biochemic[] = {
            glm::vec3( 0.3f,  0.6f,  0.1f),
            glm::vec3( 0.3f,  0.6f,  0.1f),
            glm::vec3( 0.3f,  0.6f,  0.1f),
            glm::vec3( 0.3f,  0.6f,  0.1f),
    };

    // configure VAO&VBO
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // use one VBO, then change diff VAO to configure diff color shapes
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // in VAO[0]
    glBindVertexArray(VAO);
    // Pos attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // light VAO
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Texture Loading
    // -----------------
    unsigned int diffuseMap = loadTexture("../texture/box_diffuse.png");
    unsigned int specularMap = loadTexture("../texture/box_specular.png");
    unsigned int emissionMap = loadTexture("../texture/matrix.jpg");
    shader1.use();
    shader1.setInt("material.diffuse", 0);
    shader1.setInt("material.specular", 1);
    shader1.setInt("material.emission", 2);


    // Eroor caught
    // -----------------
    // std::cout << glGetError() << std::endl;


    // Polygon Mode
    // ------------
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        // -----------------------------------------------------------------------------


        // -----------------------------------------------------------------------------
        float currentFrame = glfwGetTime();                                     // Settings
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);                                                   // I/O

        // -----------------------------------------------------------------------------
                                                                                // Rendering
        switch (mode)
        {
            case DESERT:
                glClearColor(0.8f, 0.5f, 0.2f, 1.0f);
                break;
            case FACTORY:
                glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
                break;
            case HORROR:
                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                break;
            case BIOCHEMIC:
                glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
                break;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader1.use();
        shader1.setVec3("viewPos", camera.Position);                            // let frag shader know camera's position
        shader1.setFloat("material.shininess", 64.0f);

        // camera attributes setting
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shader1.setMat4("projection", projection);
        shader1.setMat4("view", view);

        // Lights Settings
        glm::vec3 lightColor;
        glm::vec3 ambientColor;
        glm::vec3 diffuseColor;


        switch (mode)
        {
            case DESERT:
                // DirLight
                shader1.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
                shader1.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.1f);
                shader1.setVec3("dirLight.diffuse", 0.2f ,0.2f, 0.7f);
                shader1.setVec3("dirLight.specular", 0.7f, 0.7f, 0.7f);
                // PointLight 1
                shader1.setVec3("pointLights[0].position", pointLightPositions[0]);
                shader1.setVec3("pointLights[0].ambient", colors_desert[0] * 0.1f);
                shader1.setVec3("pointLights[0].diffuse", colors_desert[0]);
                shader1.setVec3("pointLights[0].specular", colors_desert[0]);
                shader1.setFloat("pointLights[0].constant", 1.0f);
                shader1.setFloat("pointLights[0].linear", 0.09f);
                shader1.setFloat("pointLights[0].quadratic", 0.032f);
                // PointLight 2
                shader1.setVec3("pointLights[1].position", pointLightPositions[1]);
                shader1.setVec3("pointLights[1].ambient", colors_desert[1] * 0.1f);
                shader1.setVec3("pointLights[1].diffuse", colors_desert[1]);
                shader1.setVec3("pointLights[1].specular", colors_desert[1]);
                shader1.setFloat("pointLights[1].constant", 1.0f);
                shader1.setFloat("pointLights[1].linear", 0.09f);
                shader1.setFloat("pointLights[1].quadratic", 0.032f);
                // PointLight 3
                shader1.setVec3("pointLights[2].position", pointLightPositions[2]);
                shader1.setVec3("pointLights[2].ambient", colors_desert[2] * 0.1f);
                shader1.setVec3("pointLights[2].diffuse", colors_desert[2]);
                shader1.setVec3("pointLights[2].specular", colors_desert[2]);
                shader1.setFloat("pointLights[2].constant", 1.0f);
                shader1.setFloat("pointLights[2].linear", 0.09f);
                shader1.setFloat("pointLights[2].quadratic", 0.032f);
                // PointLight 4
                shader1.setVec3("pointLights[3].position", pointLightPositions[3]);
                shader1.setVec3("pointLights[3].ambient", colors_desert[3] * 0.1f);
                shader1.setVec3("pointLights[3].diffuse", colors_desert[3]);
                shader1.setVec3("pointLights[3].specular", colors_desert[3]);
                shader1.setFloat("pointLights[3].constant", 1.0f);
                shader1.setFloat("pointLights[3].linear", 0.09f);
                shader1.setFloat("pointLights[3].quadratic", 0.032f);
                // SpotLight
                shader1.setVec3("spotLight.position", camera.Position);
                shader1.setVec3("spotLight.direction", camera.Front);
                shader1.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
                shader1.setVec3("spotLight.diffuse", 0.8f ,0.8f, 0.0f);
                shader1.setVec3("spotLight.specular", 0.8f, 0.8f, 0.0f);
                shader1.setFloat("spotLight.constant", 1.0f);
                shader1.setFloat("spotLight.linear", 0.09f);
                shader1.setFloat("spotLight.quadratic", 0.032f);
                shader1.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
                shader1.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
                break;
            case FACTORY:
                // DirLight
                shader1.setVec3("dirLight.direction", 0.2f, 0.2f, 0.6f);
                shader1.setVec3("dirLight.ambient", 0.4f, 0.2f, 0.8f);
                shader1.setVec3("dirLight.diffuse", 0.7f ,0.4f, 0.3f);
                shader1.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
                // PointLight 1
                shader1.setVec3("pointLights[0].position", pointLightPositions[0]);
                shader1.setVec3("pointLights[0].ambient", colors_factory[0] * 0.1f);
                shader1.setVec3("pointLights[0].diffuse", colors_factory[0]);
                shader1.setVec3("pointLights[0].specular", colors_factory[0]);
                shader1.setFloat("pointLights[0].constant", 1.0f);
                shader1.setFloat("pointLights[0].linear", 0.09f);
                shader1.setFloat("pointLights[0].quadratic", 0.032f);
                // PointLight 2
                shader1.setVec3("pointLights[1].position", pointLightPositions[1]);
                shader1.setVec3("pointLights[1].ambient", colors_factory[1] * 0.1f);
                shader1.setVec3("pointLights[1].diffuse", colors_factory[1]);
                shader1.setVec3("pointLights[1].specular", colors_factory[1]);
                shader1.setFloat("pointLights[1].constant", 1.0f);
                shader1.setFloat("pointLights[1].linear", 0.09f);
                shader1.setFloat("pointLights[1].quadratic", 0.032f);
                // PointLight 3
                shader1.setVec3("pointLights[2].position", pointLightPositions[2]);
                shader1.setVec3("pointLights[2].ambient", colors_factory[2] * 0.1f);
                shader1.setVec3("pointLights[2].diffuse", colors_factory[2]);
                shader1.setVec3("pointLights[2].specular", colors_factory[2]);
                shader1.setFloat("pointLights[2].constant", 1.0f);
                shader1.setFloat("pointLights[2].linear", 0.09f);
                shader1.setFloat("pointLights[2].quadratic", 0.032f);
                // PointLight 4
                shader1.setVec3("pointLights[3].position", pointLightPositions[3]);
                shader1.setVec3("pointLights[3].ambient", colors_factory[3] * 0.1f);
                shader1.setVec3("pointLights[3].diffuse", colors_factory[3]);
                shader1.setVec3("pointLights[3].specular", colors_factory[3]);
                shader1.setFloat("pointLights[3].constant", 1.0f);
                shader1.setFloat("pointLights[3].linear", 0.09f);
                shader1.setFloat("pointLights[3].quadratic", 0.032f);
                // SpotLight
                shader1.setVec3("spotLight.position", camera.Position);
                shader1.setVec3("spotLight.direction", camera.Front);
                shader1.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
                shader1.setVec3("spotLight.diffuse", 1.0f ,1.0f, 1.0f);
                shader1.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
                shader1.setFloat("spotLight.constant", 1.0f);
                shader1.setFloat("spotLight.linear", 0.09f);
                shader1.setFloat("spotLight.quadratic", 0.032f);
                shader1.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
                shader1.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
                break;
            case HORROR:
                // DirLight
                shader1.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
                shader1.setVec3("dirLight.ambient", 0.0f, 0.0f, 0.0f);
                shader1.setVec3("dirLight.diffuse", 0.05f ,0.05f, 0.05f);
                shader1.setVec3("dirLight.specular", 0.2f, 0.2f, 0.2f);
                // PointLight 1
                shader1.setVec3("pointLights[0].position", pointLightPositions[0]);
                shader1.setVec3("pointLights[0].ambient", colors_horror[0] * 0.1f);
                shader1.setVec3("pointLights[0].diffuse", colors_horror[0]);
                shader1.setVec3("pointLights[0].specular", colors_horror[0]);
                shader1.setFloat("pointLights[0].constant", 1.0f);
                shader1.setFloat("pointLights[0].linear", 0.09f);
                shader1.setFloat("pointLights[0].quadratic", 0.032f);
                // PointLight 2
                shader1.setVec3("pointLights[1].position", pointLightPositions[1]);
                shader1.setVec3("pointLights[1].ambient", colors_horror[1] * 0.1f);
                shader1.setVec3("pointLights[1].diffuse", colors_horror[1]);
                shader1.setVec3("pointLights[1].specular", colors_horror[1]);
                shader1.setFloat("pointLights[1].constant", 1.0f);
                shader1.setFloat("pointLights[1].linear", 0.09f);
                shader1.setFloat("pointLights[1].quadratic", 0.032f);
                // PointLight 3
                shader1.setVec3("pointLights[2].position", pointLightPositions[2]);
                shader1.setVec3("pointLights[2].ambient", colors_horror[2] * 0.1f);
                shader1.setVec3("pointLights[2].diffuse", colors_horror[2]);
                shader1.setVec3("pointLights[2].specular", colors_horror[2]);
                shader1.setFloat("pointLights[2].constant", 1.0f);
                shader1.setFloat("pointLights[2].linear", 0.09f);
                shader1.setFloat("pointLights[2].quadratic", 0.032f);
                // PointLight 4
                shader1.setVec3("pointLights[3].position", pointLightPositions[3]);
                shader1.setVec3("pointLights[3].ambient", colors_horror[3] * 0.1f);
                shader1.setVec3("pointLights[3].diffuse", colors_horror[3]);
                shader1.setVec3("pointLights[3].specular", colors_horror[3]);
                shader1.setFloat("pointLights[3].constant", 1.0f);
                shader1.setFloat("pointLights[3].linear", 0.09f);
                shader1.setFloat("pointLights[3].quadratic", 0.032f);
                // SpotLight
                shader1.setVec3("spotLight.position", camera.Position);
                shader1.setVec3("spotLight.direction", camera.Front);
                shader1.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
                shader1.setVec3("spotLight.diffuse", 0.8f ,0.8f, 0.8f);
                shader1.setVec3("spotLight.specular", 0.8f, 0.8f, 0.8f);
                shader1.setFloat("spotLight.constant", 1.0f);
                shader1.setFloat("spotLight.linear", 0.09f);
                shader1.setFloat("spotLight.quadratic", 0.032f);
                shader1.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
                shader1.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
                break;
            case BIOCHEMIC:
                // DirLight
                shader1.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
                shader1.setVec3("dirLight.ambient", 0.5f, 0.5f, 0.5f);
                shader1.setVec3("dirLight.diffuse", 0.7f ,0.7f, 0.7f);
                shader1.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
                // PointLight 1
                shader1.setVec3("pointLights[0].position", pointLightPositions[0]);
                shader1.setVec3("pointLights[0].ambient", colors_biochemic[0] * 0.1f);
                shader1.setVec3("pointLights[0].diffuse", colors_biochemic[0]);
                shader1.setVec3("pointLights[0].specular", colors_biochemic[0]);
                shader1.setFloat("pointLights[0].constant", 1.0f);
                shader1.setFloat("pointLights[0].linear", 0.09f);
                shader1.setFloat("pointLights[0].quadratic", 0.032f);
                // PointLight 2
                shader1.setVec3("pointLights[1].position", pointLightPositions[1]);
                shader1.setVec3("pointLights[1].ambient", colors_biochemic[1] * 0.1f);
                shader1.setVec3("pointLights[1].diffuse", colors_biochemic[1]);
                shader1.setVec3("pointLights[1].specular", colors_biochemic[1]);
                shader1.setFloat("pointLights[1].constant", 1.0f);
                shader1.setFloat("pointLights[1].linear", 0.09f);
                shader1.setFloat("pointLights[1].quadratic", 0.032f);
                // PointLight 3
                shader1.setVec3("pointLights[2].position", pointLightPositions[2]);
                shader1.setVec3("pointLights[2].ambient", colors_biochemic[2] * 0.1f);
                shader1.setVec3("pointLights[2].diffuse", colors_biochemic[2]);
                shader1.setVec3("pointLights[2].specular", colors_biochemic[2]);
                shader1.setFloat("pointLights[2].constant", 1.0f);
                shader1.setFloat("pointLights[2].linear", 0.09f);
                shader1.setFloat("pointLights[2].quadratic", 0.032f);
                // PointLight 4
                shader1.setVec3("pointLights[3].position", pointLightPositions[3]);
                shader1.setVec3("pointLights[3].ambient", colors_biochemic[3] * 0.1f);
                shader1.setVec3("pointLights[3].diffuse", colors_biochemic[3]);
                shader1.setVec3("pointLights[3].specular", colors_biochemic[3]);
                shader1.setFloat("pointLights[3].constant", 1.0f);
                shader1.setFloat("pointLights[3].linear", 0.09f);
                shader1.setFloat("pointLights[3].quadratic", 0.032f);
                // SpotLight
                shader1.setVec3("spotLight.position", camera.Position);
                shader1.setVec3("spotLight.direction", camera.Front);
                shader1.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
                shader1.setVec3("spotLight.diffuse", 0.2f ,0.8f, 0.0f);
                shader1.setVec3("spotLight.specular", 0.2f, 0.8f, 0.0f);
                shader1.setFloat("spotLight.constant", 1.0f);
                shader1.setFloat("spotLight.linear", 0.09f);
                shader1.setFloat("spotLight.quadratic", 0.032f);
                shader1.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
                shader1.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
                break;
        }



        // bind maps
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, emissionMap);

        // Cube party
        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < 10; i++)
        {
            // draw cube
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shader1.setMat4("model", model);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }



        // draw lamp
        lampshader.use();
        lampshader.setMat4("projection", projection);
        lampshader.setMat4("view", view);

        glBindVertexArray(lightVAO);
        for (unsigned int i = 0; i < 4; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f));                             // smaller
            lampshader.setMat4("model", model);
            switch (mode)
            {
                case DESERT:
                    lampshader.setVec3("color", colors_desert[i]);
                    break;
                case FACTORY:
                    lampshader.setVec3("color", colors_factory[i]);
                    break;
                case HORROR:
                    lampshader.setVec3("color", colors_horror[i]);
                    break;
                case BIOCHEMIC:
                    lampshader.setVec3("color", colors_biochemic[i]);
                    break;
            }


            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glBindVertexArray(0);

        // -----------------------------------------------------------------------------
        glfwSwapBuffers(window);                                                // Double-buufer
        glfwPollEvents();                                                       // check for IO
    }

    // Release resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;

}

// Process Input
// ---------------------------------------------------------------------------------------------------------------------
void processInput (GLFWwindow* window) {
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE))
        glfwSetWindowShouldClose(window, true);

    // WASD to control camera
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_W))
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_S))
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_A))
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_D))
        camera.ProcessKeyboard(RIGHT, deltaTime);


    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_1))
        mode = DESERT;
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_2))
        mode = FACTORY;
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_3))
        mode = HORROR;
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_4))
        mode = BIOCHEMIC;


}

// To adjust yhe view when user change the window
// ---------------------------------------------------------------------------------------------------------------------
void framebuffer_size_callback (GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// MouseMovement control
// ---------------------------------------------------------------------------------------------------------------------
void mouse_callback (GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = ypos - lastY;
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, -yoffset);

}

// MouseScroll control
// ---------------------------------------------------------------------------------------------------------------------
void scroll_callback (GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

// Load Texture
// ---------------------------------------------------------------------------------------------------------------------
unsigned int loadTexture (const char *path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format = GL_RGB;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}