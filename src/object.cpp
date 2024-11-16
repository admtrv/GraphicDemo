#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

#include "object.h"
void Object::addChild(std::unique_ptr<Object> child) {
    child->parent = this;
    this->children.push_back(std::move(child));
}

bool Object::update(Scene &scene, float dt) {
    bool o = this->updateInternal(scene, dt);
    if(parent != NULL) modelMatrix = parent->modelMatrix*modelMatrix;

    // Use iterator to update all objects so we can remove while iterating
    auto i = std::begin(children);

    while (i != std::end(children)) {
        // Update and remove from list if needed
        auto child = i->get();
        if (!child->update(scene, dt))
            i = children.erase(i); // NOTE: no need to call destructors as we store shared pointers in the scene
        else
            ++i;
    }
    return o;
}
void Object::render(Scene &scene) {
    this->renderInternal(scene);
    for ( auto& child : children )
        child->render(scene);
}

void Object::generateModelMatrix() {
    modelMatrix =
        glm::translate(glm::mat4(1.0f), position)
        * glm::orientate4(rotation)
        * glm::scale(glm::mat4(1.0f), scale);
}
