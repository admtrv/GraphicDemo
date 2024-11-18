/*
 * Room.cpp
 */

#include "Room.h"

Room::Room() {
    // floor
    auto floor = std::make_unique<StaticObject>("floor.bmp");
    floor->position = glm::vec3(0.0f, 0.0f, 0.0f);
    floor->scale = glm::vec3(ROOM_WIDTH, ROOM_WALL_THICKNESS, ROOM_DEPTH);
    components.push_back(std::move(floor));

    // ceiling
    auto ceiling = std::make_unique<StaticObject>("ceiling.bmp");
    ceiling->position = glm::vec3(0.0f, ROOM_HEIGHT, 0.0f);
    ceiling->scale = glm::vec3(ROOM_WIDTH, ROOM_WALL_THICKNESS, ROOM_DEPTH);
    components.push_back(std::move(ceiling));

    // back wall
    auto backWall = std::make_unique<StaticObject>("wall.bmp");
    backWall->position = glm::vec3(0.0f, ROOM_HEIGHT / 2.0f, -ROOM_DEPTH / 2.0f);
    backWall->scale = glm::vec3(ROOM_WIDTH, ROOM_HEIGHT, ROOM_WALL_THICKNESS);
    components.push_back(std::move(backWall));

    // left wall
    auto leftWall = std::make_unique<StaticObject>("wall.bmp");
    leftWall->position = glm::vec3(-ROOM_WIDTH / 2.0f, ROOM_HEIGHT / 2.0f, 0.0f);
    leftWall->scale = glm::vec3(ROOM_DEPTH, ROOM_HEIGHT, ROOM_WALL_THICKNESS);
    leftWall->rotation = glm::vec3(0.0f, 0.0f, glm::radians(90.0f));
    components.push_back(std::move(leftWall));

    // right wall
    auto rightWall = std::make_unique<StaticObject>("wall.bmp");
    rightWall->position = glm::vec3(ROOM_WIDTH / 2.0f, ROOM_HEIGHT / 2.0f, 0.0f);
    rightWall->scale = glm::vec3(ROOM_DEPTH, ROOM_HEIGHT, ROOM_WALL_THICKNESS);
    rightWall->rotation = glm::vec3(0.0f, 0.0f, glm::radians(90.0f));
    components.push_back(std::move(rightWall));
}

void Room::addArcades() {
    generateArcade();
}

void Room::addBilliards() {
    generateBilliards();
}

void Room::addDartboards() {
    float offsetX = ROOM_WIDTH / 5.0f;
    float dartboardY = ROOM_HEIGHT / 2.0f;
    float dartboardZ = -(ROOM_DEPTH / 2.0f) + 0.1f;

    auto leftDartboard = std::make_unique<Dartboard>();
    leftDartboard->position = glm::vec3(-offsetX, dartboardY, dartboardZ);
    components.push_back(std::move(leftDartboard));

    auto rightDartboard = std::make_unique<Dartboard>();
    rightDartboard->position = glm::vec3(offsetX, dartboardY, dartboardZ);
    components.push_back(std::move(rightDartboard));
}

// random arcade generation
void Room::generateArcade() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> textureDist(0, arcadeTextures.size() - 1);

    float startZ = - (ROOM_DEPTH / 2.0f) + ARCADE_WIDTH * 0.5f;
    float stepZ = ARCADE_WIDTH;

    for (int i = 0; i < ARCADE_NUM; i++) {
        std::string randomTexture = arcadeTextures[textureDist(gen)];
        auto arcadeMachine = std::make_unique<Arcade>(randomTexture);

        arcadeMachine->position = glm::vec3(
                -(ROOM_WIDTH / 2.0f) + (ROOM_WALL_THICKNESS / 2.0f) + (ARCADE_WIDTH / 2.0f),
                ARCADE_HEIGHT / 2.0f,
                startZ + i * stepZ
        );

        arcadeMachine->rotation = glm::vec3(0.0f, 0.0f, glm::radians(90.0f));

        components.push_back(std::move(arcadeMachine));
    }
}

void Room::generateBilliards() {
    float startZ = - (ROOM_DEPTH / 2.0f) + BILLIARD_WIDTH * 2.0f;
    float stepZ = BILLIARD_WIDTH * 2.0f;

    for (int i = 0; i < BILLIARD_NUM; i++) {
        auto billiard = std::make_unique<Billiard>();

        billiard->position = glm::vec3(
                (ROOM_WIDTH / 2.0f) - ((ROOM_WALL_THICKNESS / 2.0f) + BILLIARD_LENGTH),
                BILLIARD_HEIGHT / 2.0f,
                startZ + i * stepZ
        );

        components.push_back(std::move(billiard));
    }
}

void Room::addChandeliers() {
    float chandelierY = ROOM_HEIGHT - 4.0f;
    float startZ = - (ROOM_DEPTH / 2.0f) + BILLIARD_WIDTH * 2.0f;
    float stepZ = BILLIARD_WIDTH * 2.0f;

    for (int i = 0; i < BILLIARD_NUM; i++) {
        auto chandelier = std::make_unique<Chandelier>();

        chandelier->position = glm::vec3(
                (ROOM_WIDTH / 2.0f) - ((ROOM_WALL_THICKNESS / 2.0f) + BILLIARD_LENGTH),
                chandelierY,
                startZ + i * stepZ
        );

        components.push_back(std::move(chandelier));
    }
}


bool Room::updateInternal(Scene& scene, float dt) {
    for (auto& component : components) {
        component->updateInternal(scene, dt);
    }
    return true;
}

void Room::renderInternal(Scene& scene) {
    for (auto& component : components) {
        component->renderInternal(scene);
    }
}
