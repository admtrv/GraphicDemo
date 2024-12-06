/*
 * Dartboard.cpp
 */

#include "Dartboard.h"
#include "ThrowingDart.h"

Dartboard::Dartboard(const std::string& textureFile, const std::string& modelFile)
        : StaticObject(textureFile, modelFile) {
    scale = glm::vec3(DARTBOARD_FACTOR, DARTBOARD_FACTOR, DARTBOARD_FACTOR);
}

void Dartboard::throwDart() {
    auto dart = std::make_unique<ThrowingDart>();
    float xoffset = ((float)(rand() %800) / 100.0f)-4.0f;
    float yoffset = ((float)(rand() %400) / 100.0f)-2.0f;
    float zoffset = ((float)(rand() %400) / 100.0f)-2.0f;
    dart->speed = glm::vec3(xoffset,4.5f+yoffset,-20+zoffset)/scale;
    dart->height = (position.y-0.1f)/scale.y;
    dart->position = position + glm::vec3(0,0,200);
    dart->targetCoordinates = glm::vec3(0);//position + glm::vec3(0.8,-0.8,1.2f)/scale;
    dart->targetRadius = 32.0f/scale.x;
    dart->state = 0;

    addChild(std::move(dart));
}
