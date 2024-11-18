/*
 * Chandelier.cpp
 */

#include "Chandelier.h"

Chandelier::Chandelier(const std::string& textureFile, const std::string& modelFile)
        : StaticObject(textureFile, modelFile) {
    scale = glm::vec3(CHANDELIER_SCALE, CHANDELIER_SCALE, CHANDELIER_SCALE);

    rotation = glm::vec3(glm::radians(-90.0f), 0.0f, glm::radians(-90.0f));
}
