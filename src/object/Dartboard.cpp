/*
 * Dartboard.cpp
 */

#include "Dartboard.h"

Dartboard::Dartboard(const std::string& textureFile, const std::string& modelFile)
        : StaticObject(textureFile, modelFile) {
    scale = glm::vec3(DARTBOARD_FACTOR, DARTBOARD_FACTOR, DARTBOARD_FACTOR);
}
