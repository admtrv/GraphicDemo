/*
 * Camera.cpp
 */

#include "Camera.h"


void Camera::update()
{
    viewMatrix = glm::lookAt(position, position + direction, up);
}
