//
// Created by nortl on 12/11/2024.
//

#include "DebugCameraControl.h"
void DebugCameraControl::Update(float dt){
    // Keyboard controls
    if(keyboard[GLFW_KEY_W] && !keyboard[GLFW_KEY_S]) {
        camera->position += (camera->direction)*dt*CAMERA_MOVEMENT_SPEED;
    }
    else if(keyboard[GLFW_KEY_S] && !keyboard[GLFW_KEY_W] ) {
        camera->position -= (camera->direction)*dt*CAMERA_MOVEMENT_SPEED;
    }

    if(keyboard[GLFW_KEY_A] && !keyboard[GLFW_KEY_D]) {
        camera->position += glm::mat3{  1,0,0,
                                        0,0,0,
                                        0,0,1}
                            *(glm::mat3)(glm::rotate(glm::mat4(1),glm::radians(90.0f),glm::vec3(0,1,0)))*(camera->direction)*(dt)*CAMERA_MOVEMENT_SPEED;
    }
    else if(keyboard[GLFW_KEY_D] && !keyboard[GLFW_KEY_A] ) {
        camera->position += glm::mat3{  1,0,0,
                                        0,0,0,
                                        0,0,1}
                            *(glm::mat3)(glm::rotate(glm::mat4(1),glm::radians(90.0f),glm::vec3(0,1,0)))*(camera->direction)*(-dt)*CAMERA_MOVEMENT_SPEED;
    }

    if(keyboard[GLFW_KEY_UP] && !keyboard[GLFW_KEY_DOWN]) {
        camera->direction = (glm::mat3)(glm::rotate(glm::mat4(1),glm::radians(dt*CAMERA_ROTATION_SPEED),glm::vec3(0,0,1)))*camera->direction;
    }
    else if(keyboard[GLFW_KEY_DOWN] && !keyboard[GLFW_KEY_UP] ) {
        camera->direction = (glm::mat3)(glm::rotate(glm::mat4(1),glm::radians(-dt*CAMERA_ROTATION_SPEED),glm::vec3(0,0,1)))*camera->direction;
    }

    if(keyboard[GLFW_KEY_LEFT] && !keyboard[GLFW_KEY_RIGHT]) {
        camera->direction = (glm::mat3)(glm::rotate(glm::mat4(1),glm::radians(dt*CAMERA_ROTATION_SPEED),glm::vec3(0,1,0)))*camera->direction;
    }
    else if(keyboard[GLFW_KEY_RIGHT] && !keyboard[GLFW_KEY_LEFT] ) {
        camera->direction = (glm::mat3)(glm::rotate(glm::mat4(1),glm::radians(-dt*CAMERA_ROTATION_SPEED),glm::vec3(0,1,0)))*camera->direction;
    }
    camera->update();
}