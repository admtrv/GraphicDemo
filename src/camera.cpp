#include <glm/glm.hpp>

#include "camera.h"


Camera::Camera(float fov, float ratio, float near, float far) {
    float fowInRad = glm::radians(fov);

    projectionMatrix = glm::perspective(fowInRad, ratio, near, far);
    update();
}

void Camera::update() {
    viewMatrix = lookAt(position, position+direction, up);
}
