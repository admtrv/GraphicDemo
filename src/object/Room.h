/*
 * Room.cpp
 */

#pragma once

#include "src/object/StaticObject.h"
#include "src/object/Arcade.h"
#include "src/object/Billiard.h"

#include <vector>
#include <memory>
#include <random>

#define ROOM_WIDTH              23.0f
#define ROOM_DEPTH              30.0f
#define ROOM_HEIGHT             10.0f
#define ROOM_WALL_THICKNESS     0.2f


class Room : public StaticObject {
public:
    Room();
    bool updateInternal(Scene& scene, float dt) override;
    void renderInternal(Scene& scene) override;

    void addArcades();
    void addBilliards();
private:
    std::vector<std::unique_ptr<StaticObject>> components;

    void generateArcade();
    void generateBilliards();
};
