/*
 * Dartboard.h
 */

#pragma once

#include "StaticObject.h"

#define DARTBOARD_FACTOR 0.08f

class Dartboard : public StaticObject {
public:
    explicit Dartboard(const std::string& textureFile = "dartboard.bmp", const std::string& modelFile = "dartboard.obj");

    void throwDart();
};
