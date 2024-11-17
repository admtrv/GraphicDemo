/*
 * DebugCamera.h
 */

#pragma once

#include <unordered_map>

#include "Camera.h"

#define CAMERA_MOVEMENT_SPEED 15.0f
#define CAMERA_ROTATION_SPEED 20.0f

class DebugCamera {
public:
    std::map< int, int > keyboard;
    explicit DebugCamera(Camera *c) {
        camera = c;
    }

    void update(float dt);
    void handleMovement(float dt);
    void handleRotation(float dt);

private:
    Camera *camera;

};
