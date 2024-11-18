/*
 * Chandelier.h
 */

#pragma once

#include "StaticObject.h"

#define CHANDELIER_SCALE 0.05f

class Chandelier : public StaticObject {
public:
    explicit Chandelier(const std::string& textureFile = "chandelier.bmp", const std::string& modelFile = "chandelier.obj");
};
