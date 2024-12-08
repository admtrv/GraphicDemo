#pragma once

#include "src/object/StaticObject.h"
#include "src/object/VirtualObject.h"
#include "src/object/Arcade.h"
#include "src/object/Billiard.h"
#include "src/object/Dartboard.h"
#include "src/object/Chandelier.h"
#include "src/object/CollisionGroup.h"
#include "src/object/BilliardBall.h"
#include "src/object/BilliardWall.h"
#include "src/object/ThrowingDart.h"

#include <vector>
#include <memory>
#include <random>

#define ROOM_WIDTH              23.0f
#define ROOM_DEPTH              30.0f
#define ROOM_HEIGHT             10.0f
#define ROOM_WALL_THICKNESS     0.2f

class Room : public VirtualObject {
public:
    explicit Room(Scene& scene);

    void addArcades(Scene& scene);
    void addChandeliers(Scene& scene);

    BilliardBall* addBilliards();
    Dartboard* addDartboards();

private:
    std::vector<std::unique_ptr<CollisionGroup>> collisionGroups;

    void generateArcade(Scene& scene);
    BilliardBall* generateBilliards();
};
