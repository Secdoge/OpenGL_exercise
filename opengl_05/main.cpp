#define STB_IMAGE_IMPLEMENTATION
// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "src/stb_image.h"
#include "src/shader_s.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Standard Headers
#include <iostream>
#include <cmath>

// Settings
const unsigned int SCR_WIDTH    = 800;
const unsigned int SCR_HEIGHT   = 600;
float mix = 0.2f;                                                               // texture
float deltaTime = 0.0f;                                                         // timer
float lastFrame = 0.0f;
float lastX = 400, lastY = 300;                                                 // cursor
float pitch = 0.0f;
float yaw   = -90.0f;
float fov   = 45.0f;
bool firstMouse = true;
glm::vec3 cameraPos     = glm::vec3(0.0f, 0.0f, 3.0f);                          // camera
glm::vec3 cameraFront   = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp      = glm::vec3(0.0f, 1.0f, 0.0f);

void framebuffer_size_callback (GLFWwindow* window, int width, int height);     // Call-back function statement
void processInput (GLFWwindow* window);
void mouse_callback (GLFWwindow* window, double xpos, double ypos);
void scroll_callback (GLFWwindow* window, double xoffset, double yoffset);

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
    // Cursor Control
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);                // disable cursor
    glfwSetCursorPosCallback(window, mouse_callback);                           // set function able
    glfwSetScrollCallback(window, scroll_callback);


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
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    };
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
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // use one VBO, then change diff VAO to configure diff color shapes
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // in VAO[0]
    glBindVertexArray(VAO);
    // Pos attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Texture attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

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

    // Matrix text board
    // -----------------



    // Polygon Mode
    // ------------
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        // -----------------------------------------------------------------------------
        float currentFrame = glfwGetTime();                                      // Time settings
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // -----------------------------------------------------------------------------
        processInput(window);                                                   // Input

        // -----------------------------------------------------------------------------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);                                   // Rendering
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        // bind textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // first texture
        shader1.setFloat("mixParam", mix);
        shader1.use();                                                          // draw the triangles


        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));                      // camera moves backward
        projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 100.0f);
        shader1.setMat4("view", view);
        shader1.setMat4("projection", projection);
        glBindVertexArray(VAO);
        // camera settings
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        shader1.setMat4("view", view);

        // draw models
        for (unsigned int i = 0; i < 10; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            float angle = 20.0f * i;
            if (0 == i % 3)
                angle = glfwGetTime() * 25.0f;
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shader1.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }


        glBindVertexArray(0);


        // glDrawArrays(GL_TRIANGLES, 0, 3);


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

    // Up&Down to control the texture mixture
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

    // WASD to control camera
    float cameraSpeed = 2.5f * deltaTime;
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_W))
        cameraPos += cameraSpeed * cameraFront;
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_S))
        cameraPos -= cameraSpeed * cameraFront;
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_A))
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_D))
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;


}

// To adjust yhe view when user change the window
void framebuffer_size_callback (GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// mouse control
void mouse_callback (GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = ypos - lastY;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.05f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch -= yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);

}

// scroll control
void scroll_callback (GLFWwindow* window, double xoffset, double yoffset) {
    if (fov >= 1.0f && fov <= 45.0f)
        fov -= yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}