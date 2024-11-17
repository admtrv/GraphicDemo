/*
 * Scene.h
 */

#pragma once

#include <memory>
#include <map>
#include <list>
#include <algorithm>

#include "src/object/Object.h"
#include "src/object/StaticObject.h"
#include "src/camera/Camera.h"

// scene aggregates all scene-related data, including objects and camera
class Scene {
public:

    void update(float time);    // update all objects in the scene
    void render();              // render all objects in the scene

    std::unique_ptr<Camera> camera;                 // camera object
    std::list< std::unique_ptr<Object> > objects;   // all objects to be rendered in scene
    glm::vec3 light{0.5f, 1.0f, 1.0f};
};
