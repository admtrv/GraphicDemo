/*
 * Arcade.cpp
 */

#include "Arcade.h"

Arcade::Arcade(const std::string& textureFile, const std::string& modelFile)
        : StaticObject(textureFile, modelFile) {
    scale = glm::vec3(ARCADE_FACTOR, ARCADE_FACTOR, ARCADE_FACTOR);
}