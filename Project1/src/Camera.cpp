#include <iostream>
#include "Camera.h"




// returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera::GetViewMatrix()
{
    glm::mat4 viewMat = glm::mat4(1.0f);
    viewMat = glm::lookAt(transform.position, transform.position + transform.GetForward(), transform.GetUp());
    return viewMat;
}

// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
        transform.position += transform.GetForward() * velocity;
    if (direction == BACKWARD)
        transform.position -= transform.GetForward() * velocity;
    if (direction == LEFT)
        transform.position -= transform.GetRight() * velocity;
    if (direction == RIGHT)
        transform.position += transform.GetRight() * velocity;
}

// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    transform.rotation.y -= xoffset;
    transform.rotation.x += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (transform.rotation.z == 180 || transform.rotation.z == -180)
        {
            if (transform.rotation.x > 89.0f + 180)	transform.rotation.x = 89.0f + 180;
            if (transform.rotation.x < -89.0f - 180)	transform.rotation.x = -89.0f - 180;
        }
        else
        {
            if (transform.rotation.x > 89.0f)	transform.rotation.x = 89.0f;
            if (transform.rotation.x < -89.0f)	transform.rotation.x = -89.0f;
        }
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    transform.SetRotation(transform.rotation);
}

// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::ProcessMouseScroll(float yoffset)
{
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}


void Camera::updateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(transform.rotation.y)) * cos(glm::radians(transform.rotation.x));
    front.y = sin(glm::radians(transform.rotation.x));
    front.z = sin(glm::radians(transform.rotation.y)) * cos(glm::radians(transform.rotation.x));
    transform.SetForward(glm::normalize(front));

    

    
}

Transform* Camera::GetTransform()
{
    return &transform;
}
