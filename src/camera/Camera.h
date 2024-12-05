/*
 * Camera.h
 */

#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <ppgso.h>

class Camera {
public:
    glm::vec3 up{0.0f, 1.0f, 0.0f};
    glm::vec3 position{-10.0f, 0.0f, 0.0f};
    glm::vec3 direction{1.0f, 0.0f, 0.0f};

    glm::mat4 viewMatrix{1.0f};
    glm::mat4 projectionMatrix{1.0f};


    void update();

};

