/*
 * Arcade.cpp
 */

#include "Arcade.h"
#include <algorithm>

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

bool Arcade::updateInternal(float dt) {
    time+=dt;
    glm::vec3 newColor = lightBaseColor;
    newColor.b = lightBaseColor.b * ((sin(time) + 1) / 2.0f);

    light->setColor(newColor);
    //std::cout << "color: " << light->getColor().r << "\n";
    return StaticObject::updateInternal(dt);
}
