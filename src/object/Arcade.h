/*
 * Arcade.h
 */

#pragma once

#include "StaticObject.h"

#define ARCADE_FACTOR   2.0f
#define ARCADE_WIDTH    (2.5f * ARCADE_FACTOR)
#define ARCADE_HEIGHT   (3.0f * ARCADE_FACTOR)
#define ARCADE_NUM      5

class Arcade : public StaticObject {
public:
    explicit Arcade(const std::string& textureFile, const std::string& modelFile = "arcade.obj");
};