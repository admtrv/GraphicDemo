#include "Light.h"

Light::Light(const glm::vec3& position, const glm::vec3& color, float intensity)
        : position(position), direction(glm::vec3(0.0f)), color(color), intensity(intensity), isDirectional(false) {}

Light::Light(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& color, float intensity)
        : position(position),
          direction(glm::normalize(direction)),
          color(color),
          intensity(intensity),
          isDirectional(true) {}


glm::vec3 Light::getPosition() const {
    return position;
}

glm::vec3 Light::getDirection() const {
    return direction;
}

void Light::setDirection(const glm::vec3& direction) {
    this->direction = glm::normalize(direction);
}

glm::vec3 Light::getColor() const {
    return color;
}

float Light::getIntensity() const {
    return intensity;
}

bool Light::isDirectionalLight() const {
    return isDirectional;
}
