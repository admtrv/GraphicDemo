//
// Created by nortl on 12/11/2024.
//
#pragma once
#include "camera.h"

#define CAMERA_MOVEMENT_SPEED 0.5f
#define CAMERA_ROTATION_SPEED 20.0f

class DebugCameraControl {
public:
    std::map< int, int > keyboard;
    DebugCameraControl(Camera *c){
        camera = c;
    }
    void Update(float dt);
private:
    Camera *camera;
};
