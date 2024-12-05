/*
 * Object.h
 */

#pragma once

#include <memory>
#include <list>
#include <ppgso/ppgso.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <algorithm>

class Scene;

class Object {
public:
    Object() = default;
    Object(const Object&) = default;
    Object(Object&&) = default;
    virtual ~Object() = default;

    // parent and child objects
    Object* parent = nullptr;
    std::list<std::unique_ptr<Object>> children;

    // add child object
    void addChild(std::unique_ptr<Object> child);

    // update  object and its children
    bool update(float dt);

    // render object and its children
    void render(ppgso::Shader *shader);

    virtual bool updateInternal(float dt) = 0;
    virtual void renderInternal(ppgso::Shader *shader) = 0;

    // object properties
    glm::vec3 position{0.0f, 0.0f, 0.0f}; // position of the object
    glm::vec3 rotation{0.0f, 0.0f, 0.0f}; // rotation angles
    glm::vec3 scale{1.0f, 1.0f, 1.0f};    // scaling factors
    glm::mat4 modelMatrix{1.0f};                // transformation matrix

protected:
    // generate model matrix
    void generateModelMatrix();
};
