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
#include <stb_image.h>                                                          // texture
#include <shader.h>                                                             // shader
#include <camera.h>                                                             // camera
#include <model.h>                                                              // model
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
// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

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

    // Depth test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    // Stencil test
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);


    // get Vertex Attributes
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum of vertex attributes supported: " << nrAttributes << std::endl;


    // Build & Compile shader program
    // ------------------------------
    Shader shader1 = Shader("../shaders/cube_vert.shader", "../shaders/cube_frag_multi.shader");
    Shader lampshader = Shader("../shaders/lamp_vert.shader", "../shaders/lamp_frag.shader");
    Shader standard = Shader("../shaders/standard_vert.shader", "../shaders/standard_frag.shader");
    // Setup vertex data
    // -----------------
    Model ourModel("../model/nanosuit/nanosuit.obj");
    Model ourModel2("../model/nanosuit/nanosuit.obj");

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
            glm::vec3( 0.8f,  1.5f,  0.4f),
            glm::vec3(-0.8f,  1.5f,  0.4f),
            glm::vec3( 0.0f,  0.0f,  0.4f),
            glm::vec3( 0.0f,  0.0f,  0.4f),
    };

    glm::vec3 colors[] = {
            glm::vec3( 0.8f,  0.8f,  0.8f),
            glm::vec3( 0.8f,  0.8f,  0.8f),
            glm::vec3( 0.8f,  0.1f,  0.1f),
            glm::vec3( 0.1f,  0.8f,  0.1f),
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
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        shader1.use();

        // camera attributes setting
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shader1.setMat4("projection", projection);
        shader1.setMat4("view", view);

        glStencilMask(0x00);

        shader1.use();
        shader1.setVec3("viewPos", camera.Position);                            // let frag shader know camera's position
        shader1.setFloat("material.shininess", 64.0f);

        // camera attributes setting
        shader1.setMat4("projection", projection);
        shader1.setMat4("view", view);
        // DirLight
        shader1.setVec3("dirLight.direction", -0.2f, -0.2f, -0.6f);
        shader1.setVec3("dirLight.ambient", 0.1f, 0.1f, 0.1f);
        shader1.setVec3("dirLight.diffuse", 0.1f ,0.1f, 0.1f);
        shader1.setVec3("dirLight.specular", 0.1f, 0.1f, 0.1f);
        // PointLight 1
        shader1.setVec3("pointLights[0].position", pointLightPositions[0]);
        shader1.setVec3("pointLights[0].ambient", colors[0] * 0.1f);
        shader1.setVec3("pointLights[0].diffuse", colors[0]);
        shader1.setVec3("pointLights[0].specular", colors[0]);
        shader1.setFloat("pointLights[0].constant", 1.0f);
        shader1.setFloat("pointLights[0].linear", 0.09f);
        shader1.setFloat("pointLights[0].quadratic", 0.032f);
        // PointLight 2
        shader1.setVec3("pointLights[1].position", pointLightPositions[1]);
        shader1.setVec3("pointLights[1].ambient", colors[1] * 0.1f);
        shader1.setVec3("pointLights[1].diffuse", colors[1]);
        shader1.setVec3("pointLights[1].specular", colors[1]);
        shader1.setFloat("pointLights[1].constant", 1.0f);
        shader1.setFloat("pointLights[1].linear", 0.09f);
        shader1.setFloat("pointLights[1].quadratic", 0.032f);
        // PointLight 3
        shader1.setVec3("pointLights[2].position", pointLightPositions[2]);
        shader1.setVec3("pointLights[2].ambient", colors[2] * 0.1f);
        shader1.setVec3("pointLights[2].diffuse", colors[2]);
        shader1.setVec3("pointLights[2].specular", colors[2]);
        shader1.setFloat("pointLights[2].constant", 1.0f);
        shader1.setFloat("pointLights[2].linear", 0.09f);
        shader1.setFloat("pointLights[2].quadratic", 0.032f);
        // PointLight 4
        shader1.setVec3("pointLights[3].position", pointLightPositions[3]);
        shader1.setVec3("pointLights[3].ambient", colors[3] * 0.1f);
        shader1.setVec3("pointLights[3].diffuse", colors[3]);
        shader1.setVec3("pointLights[3].specular", colors[3]);
        shader1.setFloat("pointLights[3].constant", 1.0f);
        shader1.setFloat("pointLights[3].linear", 0.09f);
        shader1.setFloat("pointLights[3].quadratic", 0.032f);
        // SpotLight
        shader1.setVec3("spotLight.position", camera.Position);
        shader1.setVec3("spotLight.direction", camera.Front);
        shader1.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        shader1.setVec3("spotLight.diffuse", 0.0f ,0.0f, 0.0f);
        shader1.setVec3("spotLight.specular", 0.0f, 0.0f, 0.0f);
        shader1.setFloat("spotLight.constant", 1.0f);
        shader1.setFloat("spotLight.linear", 0.09f);
        shader1.setFloat("spotLight.quadratic", 0.032f);
        shader1.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        shader1.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));

        // draw lamp
        lampshader.use();
        lampshader.setMat4("projection", projection);
        lampshader.setMat4("view", view);

        glBindVertexArray(lightVAO);
        for (unsigned int i = 0; i < 4; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f));                             // smaller
            lampshader.setMat4("model", model);
            lampshader.setVec3("color", colors[i]);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // 1st
        // render pass
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);

        shader1.use();

        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));           // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.2f));	                            // it's a bit too big for our scene, so scale it down
        shader1.setMat4("model", model);

        ourModel.Draw(shader1);

        // 2nd
        // render pass
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        // glDisable(GL_DEPTH_TEST);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.8f, 0.0f));
        model = glm::scale(model, glm::vec3(0.205f));
        shader1.setMat4("model", model);
        shader1.setBool("pure", true);
        ourModel2.Draw(shader1);


        glStencilMask(0xFF);
        glEnable(GL_DEPTH_TEST);

        shader1.setBool("pure", false);

        // -----------------------------------------------------------------------------
        glfwSwapBuffers(window);                                                // Double-buufer
        glfwPollEvents();                                                       // check for IO
    }

    // Release resources
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
