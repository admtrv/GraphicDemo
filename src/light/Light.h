#pragma once

#include <glm/vec3.hpp>
#include <glm/glm.hpp>

class Light {
public:
    Light(const glm::vec3& position, const glm::vec3& color, float intensity);
    Light(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& color, float intensity);

    glm::vec3 getPosition() const;
    glm::vec3 getDirection() const;
    void setDirection(const glm::vec3& direction);
    glm::vec3 getColor() const;
    void setColor(const glm::vec3& color);
    float getIntensity() const;
    bool isDirectionalLight() const;

private:
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 color;
    float intensity;
    bool isDirectional;
};
