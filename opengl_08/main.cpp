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
// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

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
    Shader shader1 = Shader("../shaders/cube_vert.shader", "../shaders/cube_frag.shader");
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
            glm::vec3(-1.3f,  1.0f, -1.5f)
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
        float currentFrame = glfwGetTime();                                     // Settings
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);                                                   // I/O

        // -----------------------------------------------------------------------------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);                                   // Rendering
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // lamp moving around
        lightPos.x = sin(glfwGetTime() / 2.0f) * 1.0f;
        lightPos.z = cos(glfwGetTime() / 2.0f) * 1.0f;

        //activate shader
        shader1.use();

        // camera attributes setting
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shader1.setMat4("projection", projection);
        shader1.setMat4("view", view);
        // shader1.setVec3("light.position", lightPos);                            // let frag shader know light's position
        // shader1.setVec3("light.direction", -0.2f, -1.0f, -0.3f);
        shader1.setVec3("light.position", camera.Position);
        shader1.setVec3("light.direction", camera.Front);
        shader1.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
        shader1.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));
        shader1.setVec3("viewPos", camera.Position);                            // let frag shader know camera's position

        // Material settings
        shader1.setFloat("material.shininess", 64.0f);
        // Light settings
        glm::vec3 lightColor;
        lightColor.x = sin(glfwGetTime() * 2.0f);
        lightColor.y = sin(glfwGetTime() * 0.7f);
        lightColor.z = sin(glfwGetTime() * 0.3f);
        glm::vec3 ambientColor = lightColor * 0.2f;
        glm::vec3 diffuseColor = lightColor * 0.5f;
        shader1.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
        shader1.setVec3("light.diffuse", 0.5f ,0.5f, 0.5f);
        shader1.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        shader1.setFloat("light.constant", 1.0f);
        shader1.setFloat("light.linear", 0.09f);
        shader1.setFloat("light.quadratic", 0.032f);

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



//        // draw lamp
//        lampshader.use();
//        lampshader.setMat4("projection", projection);
//        lampshader.setMat4("view", view);
//        glm::mat4 model = glm::mat4(1.0f);
//        model = glm::translate(model, lightPos);
//        model = glm::scale(model, glm::vec3(0.2f));                             // smaller
//        lampshader.setMat4("model", model);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);


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