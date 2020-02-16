#define STB_IMAGE_IMPLEMENTATION
// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "src/stb_image.h"
#include "src/shader_s.h"

// Standard Headers
#include <iostream>
#include <cmath>

// Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float mix = 0.2f;
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
            // Pos                  // Color                // Texture
            0.5f,   0.5f,   0.0f,   1.0f,   0.0f,   0.0f,   0.25f,   0.25f,
            0.5f,   -0.5f,  0.0f,   0.0f,   1.0f,   0.0f,   0.25f,   0.0f,
            -0.5f,  -0.5f,  0.0f,   0.0f,   0.0f,   1.0f,   0.0f,   0.0f,
            -0.5f,  0.5f,   0.0f,   1.0f,   1.0f,   0.0f,   0.0f,   0.25f,
    };
    unsigned int indices[] = {
            0,  1,  2,
            0,  2,  3,
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // use one VBO, then change diff VAO to configure diff color shapes
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // in VAO[0]
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // Pos attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);                                               // enable
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Load & Create textures
    // ----------------------
    // texture1
    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load imgs
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);                     // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *data = stbi_load("../texture/container.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture1" << std::endl;
    }
    stbi_image_free(data);
    // texture2
    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // load imgs
    data = stbi_load("../texture/404.png", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture2" << std::endl;
    }
    stbi_image_free(data);

    shader1.use();                                                          // draw the triangles
    // tell opengl which texture to use
    shader1.setInt("tex1", 0);
    shader1.setInt("tex2", 1);

    // Polygon Mode
    // ------------
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        // -----------------------------------------------------------------------------
        processInput(window);                                                   // Input

        // -----------------------------------------------------------------------------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);                                   // Rendering
        glClear(GL_COLOR_BUFFER_BIT);

        // bind textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        shader1.setFloat("mixParam", mix);
        shader1.use();                                                          // draw the triangles

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

        // glDrawArrays(GL_TRIANGLES, 0, 3);


        // -----------------------------------------------------------------------------
        glfwSwapBuffers(window);                                                // Double-buufer
        glfwPollEvents();                                                       // check for IO
    }

    // Release resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;

}

// Process Input
void processInput (GLFWwindow* window) {
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE))
        glfwSetWindowShouldClose(window, true);

    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_UP)) {
        // ++;
        if (mix < 1.0f)
            mix += 0.01f;
        else
            mix = 1.0f;
    }

    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_DOWN)) {
        // --;
        if (mix > 0.0f)
            mix -= 0.01f;
        else
            mix = 0.0f;
    }


}

// To adjust yhe view when user change the window
void framebuffer_size_callback (GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}