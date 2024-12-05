#include "OrthoCamera.h"

OrthoCamera::OrthoCamera(float left, float right, float bottom, float top, float near, float far)  {
    projectionMatrix = glm::ortho(left, right, bottom, top, near, far);
    update();
}
