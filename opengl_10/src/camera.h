//
// Created by 二狗子 on 2020-02-13.
//

#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Define options for camera movement
// ---------------------------------------------------------------------------------------------------------------------
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
};

// Default camera values
// ---------------------------------------------------------------------------------------------------------------------
const float YAW         = -90.0f;
const float PITCH       = 0.0f;
const float SPEED       = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM        = 45.0f;

// Abstract camera class
// ---------------------------------------------------------------------------------------------------------------------
class Camera
{
public:
    // Camera attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // Euler Angles
    float Yaw;
    float Pitch;
    // Camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // Constructor with vectors
    explicit Camera (glm::vec3 position = glm::vec3(0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) :
        Front (glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position    = position;
        WorldUp     = up;
        Yaw         = yaw;
        Pitch       = pitch;
        updateCameraVectors();
    }
    // Constructor with scalar values
    Camera (float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) :
        Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position    = glm::vec3(posX, posY, posZ);
        WorldUp     = glm::vec3(upX, upY, upZ);
        Yaw         = yaw;
        Pitch       = pitch;
        updateCameraVectors();
    }

    // Returns the view matrix calculated using Euler Angles & LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    // Keyboard I/O stream
    void ProcessKeyboard (Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (FORWARD == direction)
            Position += Front * velocity;
        if (BACKWARD == direction)
            Position -= Front * velocity;
        if (LEFT == direction)
            Position -= Right * velocity;
        if (RIGHT == direction)
            Position += Right * velocity;

        // Keep on the ground
        // Position.y = 0.0f;
    }

    // Mouse movement I/O stream
    void ProcessMouseMovement (float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw     += xoffset;
        Pitch   += yoffset;

        // Make sure when pitch is out of bounds
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = 89.0f;
        }

        // Update Front
        updateCameraVectors();
    }

    // Mouse scroll I/O stream
    void ProcessMouseScroll (float yoffset)
    {
        if (Zoom >= 1.0f && Zoom <= 45.0f)
            Zoom -= yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom >= 45.0f)
            Zoom = 45.0f;
    }

private:
    // Calculates the front vector and other vectors
    void updateCameraVectors()
    {
        // front
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front   = glm::normalize(front);
        // Right & Up
        Right   = glm::normalize(glm::cross(Front, WorldUp));
        Up      = glm::normalize((glm::cross(Right, Front)));

    }

};


#endif //OPENGL_06_CAMERA_H
