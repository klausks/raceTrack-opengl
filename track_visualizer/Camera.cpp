#include "Camera.h"

// returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera::getViewMatrix()
{
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(position, position + forward, up);
    return view;
}

// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::processKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = movementSpeed * deltaTime;
    if (direction == FORWARD)
        position += forward * velocity;
    if (direction == BACKWARD)
        position -= forward * velocity;
    if (direction == LEFT)
        position -= right * velocity;
    if (direction == RIGHT)
        position += right * velocity;
}

// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::processMouseScroll(float yoffset)
{
    zoom -= (float)yoffset;
    if (zoom < 1.0f)
        zoom = 1.0f;
    if (zoom > 45.0f)
        zoom = 45.0f;
}

void Camera::updateCameraVectors()
{
    {
        glm::vec3 _front;
        _front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        _front.y = sin(glm::radians(pitch));
        _front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        this->forward = glm::normalize(_front);

        // Also re-calculate the Right and Up vector
        right = glm::normalize(glm::cross(forward, worldUp)); //Normalize the vectors, because their length gets closer to 0 
        up = glm::normalize(glm::cross(right, forward)); //     the more you look up or down which results in slower movement.
    }
}