#include "PersCamera.h"

PersCamera::PersCamera(float fov, float ratio, float near, float far)  {
    projectionMatrix = glm::perspective(glm::radians(fov), ratio, near, far);
    update();
}
