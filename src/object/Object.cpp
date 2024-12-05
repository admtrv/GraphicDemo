/*
 * Object.cpp
 */

#include "Object.h"

void Object::addChild(std::unique_ptr<Object> child) {
    // set the parent of the child and add it to the list of children
    child->parent = this;
    children.push_back(std::move(child));
}

bool Object::update(float dt) {
    // update current object
    bool isAlive = updateInternal(dt);

    // combine transformation matrix with the parent's matrix
    if (parent) {
        modelMatrix = parent->modelMatrix * modelMatrix;
    }

    // update children and remove not alive
    for (auto it = children.begin(); it != children.end();) {
        if (!(*it)->update(dt)) {
            it = children.erase(it);    // remove dead child
        } else {
            ++it;                               // move to the next
        }
    }

    return isAlive;
}

void Object::render(ppgso::Shader *shader) {
    renderInternal(shader);                                  // render the current object
    for (const auto& child : children) {
        child->render(shader);                               // render all children
    }
}

void Object::generateModelMatrix() {
    // compute transformation matrix from position, rotation, and scale
    modelMatrix = glm::translate(glm::mat4(1.0f), position) *
                  glm::orientate4(rotation) *
                  glm::scale(glm::mat4(1.0f), scale);
}
