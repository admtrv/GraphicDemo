#include "Scene.h"

void Scene::update(float time) {
    // update the camera if it exists
    if (camera) {
        camera->update();
    }

    // update all objects and remove those that not alive
    objects.erase(
            std::remove_if(objects.begin(), objects.end(),
                           [this, time](const std::unique_ptr<Object>& obj) {
                               return !obj->update(*this, time);
                           }),
            objects.end()
    );
}

void Scene::render() {
    // render all objects in the scene
    for (auto& obj : objects) {
        obj->render(*this);
    }
}