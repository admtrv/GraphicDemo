/*
 * Billiard.h
 */

#pragma once

#include "StaticObject.h"

#define BILLIARD_FACTOR 0.035f
#define BILLIARD_WIDTH  (120.0f * BILLIARD_FACTOR)
#define BILLIARD_LENGTH (240.0f * BILLIARD_FACTOR)
#define BILLIARD_HEIGHT (10.0f * BILLIARD_FACTOR)
#define BILLIARD_NUM    3

class Billiard : public StaticObject {
public:
    explicit Billiard(const std::string& textureFile = "billiard.bmp", const std::string& modelFile = "billiardTable.obj");
};
