/*
 * Arcade.cpp
 */

#include "Arcade.h"

std::vector<std::string> arcadeTextures = {
        "arcade1.bmp",
        "arcade2.bmp",
        "arcade3.bmp"
};

Arcade::Arcade(const std::string& textureFile, const std::string& modelFile)
        : StaticObject(textureFile, modelFile) {
    scale = glm::vec3(ARCADE_FACTOR, ARCADE_FACTOR, ARCADE_FACTOR);
}