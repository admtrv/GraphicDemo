/*
 * DebugCamera.cpp
 */

#include "DebugCamera.h"

void DebugCamera::update(float dt) {
    handleMovement(dt);
    handleRotation(dt);
    camera->update();
    logState();
}

void DebugCamera::handleMovement(float dt) {
    // forward or backward
    if (keyboard[GLFW_KEY_W] && !keyboard[GLFW_KEY_S]) {
        camera->position += camera->direction * dt * CAMERA_MOVEMENT_SPEED;
    } else if (keyboard[GLFW_KEY_S] && !keyboard[GLFW_KEY_W]) {
        camera->position -= camera->direction * dt * CAMERA_MOVEMENT_SPEED;
    }

    // left or right
    glm::vec3 right = glm::normalize(glm::cross(camera->direction, camera->up));
    if (keyboard[GLFW_KEY_A] && !keyboard[GLFW_KEY_D]) {
        camera->position -= right * dt * CAMERA_MOVEMENT_SPEED;
    } else if (keyboard[GLFW_KEY_D] && !keyboard[GLFW_KEY_A]) {
        camera->position += right * dt * CAMERA_MOVEMENT_SPEED;
    }
}

void DebugCamera::handleRotation(float dt) {
    // up or down
    if (keyboard[GLFW_KEY_UP] && !keyboard[GLFW_KEY_DOWN]) {
        glm::vec3 right = glm::normalize(glm::cross(camera->direction, camera->up));
        camera->direction = glm::rotate(glm::mat4(1.0f), glm::radians(dt * CAMERA_ROTATION_SPEED), right) * glm::vec4(camera->direction, 0.0f);
    } else if (keyboard[GLFW_KEY_DOWN] && !keyboard[GLFW_KEY_UP]) {
        glm::vec3 right = glm::normalize(glm::cross(camera->direction, camera->up));
        camera->direction = glm::rotate(glm::mat4(1.0f), glm::radians(-dt * CAMERA_ROTATION_SPEED), right) * glm::vec4(camera->direction, 0.0f);
    }

    // left or right
    if (keyboard[GLFW_KEY_LEFT] && !keyboard[GLFW_KEY_RIGHT]) {
        camera->direction = glm::rotate(glm::mat4(1.0f), glm::radians(dt * CAMERA_ROTATION_SPEED), camera->up) * glm::vec4(camera->direction, 0.0f);
    } else if (keyboard[GLFW_KEY_RIGHT] && !keyboard[GLFW_KEY_LEFT]) {
        camera->direction = glm::rotate(glm::mat4(1.0f), glm::radians(-dt * CAMERA_ROTATION_SPEED), camera->up) * glm::vec4(camera->direction, 0.0f);
    }

    camera->direction = glm::normalize(camera->direction);
}

void DebugCamera::logState() {
    std::cout << "Position: (" << camera->position.x << ", " << camera->position.y << ", " << camera->position.z << "), "
              << "Direction: (" << camera->direction.x << ", " << camera->direction.y << ", " << camera->direction.z << ")\n";
}