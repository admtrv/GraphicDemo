/*
 * Camera.cpp
 */

#include "Camera.h"

Camera::Camera(float fov, float ratio, float near, float far)
{
    projectionMatrix = glm::perspective(glm::radians(fov), ratio, near, far);
    update();
}

void Camera::update()
{
    viewMatrix = glm::lookAt(position, position + direction, up);
}
