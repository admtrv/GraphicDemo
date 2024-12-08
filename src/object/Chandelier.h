/*
 * Chandelier.h
 */

#pragma once

#include "StaticObject.h"
#include "src/light/Light.h"

#define CHANDELIER_SCALE 0.05f

class Chandelier : public StaticObject {
public:
    explicit Chandelier(const std::string& textureFile = "chandelier.bmp", const std::string& modelFile = "chandelier.obj");
    std::unique_ptr<Light> light;
};
