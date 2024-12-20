/*
 * Arcade.h
 */

#pragma once

#include "StaticObject.h"
#include "Glass.h"
#include "src/light/Light.h"

#define ARCADE_FACTOR   2.0f
#define ARCADE_WIDTH    (2.5f * ARCADE_FACTOR)
#define ARCADE_HEIGHT   (3.0f * ARCADE_FACTOR)
#define ARCADE_NUM      5

extern std::vector<std::string> arcadeTextures;

class Arcade : public StaticObject {
public:
    explicit Arcade(const std::string& textureFile, const std::string& modelFile = "arcade.obj");

    bool updateInternal(float dt) override;   // update object
    std::unique_ptr<Light> light;
    glm::vec3 lightBaseColor;
private:
    float time = 0;
};