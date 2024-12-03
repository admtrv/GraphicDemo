/*
 * Arcade.cpp
 */

#include "Arcade.h"
#include "Glass.h"

std::vector<std::string> arcadeTextures = {
        "arcade1.bmp",
        "arcade2.bmp",
        "arcade3.bmp"
};

Arcade::Arcade(const std::string& textureFile, const std::string& modelFile)
        : StaticObject(textureFile, modelFile) {
    scale = glm::vec3(ARCADE_FACTOR, ARCADE_FACTOR, ARCADE_FACTOR);

    // adding screen
    auto screen = std::make_unique<Glass>();
    screen->position = glm::vec3(-1.45f,0.4f,-0.35f);
    screen->rotation = glm::vec3(glm::radians(-15.0f), 0.0f, 0.0f);
    addChild(std::move(screen));
}

void Arcade::render(Scene& scene) {
    StaticObject::renderInternal(scene);

    for (auto& child : children) {
        child->render(scene);
    }
}
