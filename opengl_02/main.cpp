// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "src/shader_s.h"

// Standard Headers
#include <iostream>
#include <cmath>

// Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
void framebuffer_size_callback (GLFWwindow* window, int width, int height);     // Call-back function statement
void processInput (GLFWwindow* window);

// Main

int main () {
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

    // get Vertex Attributes
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum of vertex attributes supported: " << nrAttributes << std::endl;


    // Build & Compile shader program
    // ------------------------------
    Shader shader1 = Shader("../shaders/3.3.shader.vs", "../shaders/3.3.shader.fs");

    // Setup vertex data
    // -----------------
    float vertices[] = {
            // Pos
            0.5f,  -0.5f,   0.0f,
            0.0f,   0.5f,   0.0f,
            -0.5f,  -0.5f,  0.0f,
    };
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // use one VBO, then change diff VAO to configure diff color shapes
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Pos attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Polygon Mode
    // ------------
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        // -----------------------------------------------------------------------------
        processInput(window);                                                   // Input

        // -----------------------------------------------------------------------------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);                                   // Rendering
        glClear(GL_COLOR_BUFFER_BIT);

        shader1.use();                                                          // draw the triangles
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);


        // -----------------------------------------------------------------------------
        glfwSwapBuffers(window);                                                // Double-buufer
        glfwPollEvents();                                                       // check for IO
    }

    // Release resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;

}

// Process Input
void processInput (GLFWwindow* window) {
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE))
        glfwSetWindowShouldClose(window, true);
}

// To adjust yhe view when user change the window
void framebuffer_size_callback (GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}