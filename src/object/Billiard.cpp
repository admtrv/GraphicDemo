/*
 * Billiard.cpp
 */

#include "Billiard.h"

Billiard::Billiard(const std::string& textureFile, const std::string& modelFile)
        : StaticObject(textureFile, modelFile) {
    scale = glm::vec3(BILLIARD_FACTOR, BILLIARD_FACTOR, BILLIARD_FACTOR);
    rotation = glm::vec3(glm::radians(-90.0f), 0.0f, glm::radians(-90.0f));
}
