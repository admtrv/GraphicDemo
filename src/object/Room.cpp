#include <unordered_map>
#include "Room.h"

Room::Room() {
    // floor
    auto floor = std::make_unique<StaticObject>("floor.bmp");
    floor->position = glm::vec3(0.0f, 0.0f, 0.0f);
    floor->scale = glm::vec3(ROOM_WIDTH, ROOM_WALL_THICKNESS, ROOM_DEPTH);
    addChild(std::move(floor));

    // ceiling
    auto ceiling = std::make_unique<StaticObject>("ceiling.bmp");
    ceiling->position = glm::vec3(0.0f, ROOM_HEIGHT, 0.0f);
    ceiling->scale = glm::vec3(ROOM_WIDTH, ROOM_WALL_THICKNESS, ROOM_DEPTH);
    addChild(std::move(ceiling));

    // back wall
    auto backWall = std::make_unique<StaticObject>("wall.bmp");
    backWall->position = glm::vec3(0.0f, ROOM_HEIGHT / 2.0f, -ROOM_DEPTH / 2.0f);
    backWall->scale = glm::vec3(ROOM_WIDTH, ROOM_HEIGHT, ROOM_WALL_THICKNESS);
    addChild(std::move(backWall));

    // left wall
    auto leftWall = std::make_unique<StaticObject>("wall.bmp");
    leftWall->position = glm::vec3(-ROOM_WIDTH / 2.0f, ROOM_HEIGHT / 2.0f, 0.0f);
    leftWall->scale = glm::vec3(ROOM_DEPTH, ROOM_HEIGHT, ROOM_WALL_THICKNESS);
    leftWall->rotation = glm::vec3(0.0f, 0.0f, glm::radians(90.0f));
    addChild(std::move(leftWall));

    // right wall
    auto rightWall = std::make_unique<StaticObject>("wall.bmp");
    rightWall->position = glm::vec3(ROOM_WIDTH / 2.0f, ROOM_HEIGHT / 2.0f, 0.0f);
    rightWall->scale = glm::vec3(ROOM_DEPTH, ROOM_HEIGHT, ROOM_WALL_THICKNESS);
    rightWall->rotation = glm::vec3(0.0f, 0.0f, glm::radians(90.0f));
    addChild(std::move(rightWall));
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
    addChild(std::move(leftDartboard));

    auto rightDartboard = std::make_unique<Dartboard>();
    rightDartboard->position = glm::vec3(offsetX, dartboardY, dartboardZ);
    addChild(std::move(rightDartboard));
}

// random arcade generation
void Room::generateArcade() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> textureDist(0, arcadeTextures.size() - 1);

    float startZ = -(ROOM_DEPTH / 2.0f) + ARCADE_WIDTH * 0.5f;
    float stepZ = ARCADE_WIDTH;

    std::unordered_map<std::string, int> textureUsage;

    for (const auto& texture : arcadeTextures) {
        textureUsage[texture] = 0;
    }

    std::vector<std::string> arcadeSequence;

    for (int i = 0; i < ARCADE_NUM; i++) {
        std::string selectedTexture;

        auto selectTexture = [&]() {
            std::vector<std::string> candidates;
            int minUsage = std::numeric_limits<int>::max();

            // minimal usage
            for (const auto& texture : arcadeTextures) {
                if (textureUsage[texture] < minUsage) {
                    candidates.clear();
                    candidates.push_back(texture);
                    minUsage = textureUsage[texture];
                } else if (textureUsage[texture] == minUsage) {
                    candidates.push_back(texture);
                }
            }

            std::uniform_int_distribution<> candidateDist(0, candidates.size() - 1);
            return candidates[candidateDist(gen)];
        };

        // not like neighbours
        do {
            selectedTexture = selectTexture();
        } while (!arcadeSequence.empty() && selectedTexture == arcadeSequence.back());

        arcadeSequence.push_back(selectedTexture);

        textureUsage[selectedTexture]++;

        auto arcadeMachine = std::make_unique<Arcade>(selectedTexture);

        arcadeMachine->position = glm::vec3(
                -(ROOM_WIDTH / 2.0f) + (ROOM_WALL_THICKNESS / 2.0f) + (ARCADE_WIDTH / 2.0f),
                ARCADE_HEIGHT / 2.0f,
                startZ + i * stepZ
        );

        arcadeMachine->rotation = glm::vec3(0.0f, 0.0f, glm::radians(90.0f));

        addChild(std::move(arcadeMachine));
    }
}

void Room::generateBilliards() {
    float startZ = - (ROOM_DEPTH / 2.0f) + BILLIARD_WIDTH * 2.0f;
    float stepZ = BILLIARD_WIDTH * 2.0f;

    for (int i = 0; i < BILLIARD_NUM; i++) {
        auto billiard = std::make_unique<Billiard>();
        auto collisionGroup = std::make_unique<CollisionGroup>();

        billiard->position = glm::vec3(
                (ROOM_WIDTH / 2.0f) - ((ROOM_WALL_THICKNESS / 2.0f) + BILLIARD_LENGTH),
                BILLIARD_HEIGHT / 2.0f,
                startZ + i * stepZ
        );
        {
            auto wall = std::make_unique<BilliardWall>();
            wall->position = glm::vec3(57.2, 0, 75);
            wall->scale = glm::vec3(10, 240, 5);
            wall->collisionGroup = collisionGroup.get();
            wall->x = 5;
            wall->y = 120;
            wall->z = 2.5f;
            collisionGroup->AddCollider(wall.get());
            billiard->addChild(std::move(wall));

            wall = std::make_unique<BilliardWall>();
            wall->position = glm::vec3(-57.2, 0, 75);
            wall->scale = glm::vec3(10, 240, 5);
            wall->collisionGroup = collisionGroup.get();
            wall->x = 5;
            wall->y = 120;
            wall->z = 2.5f;
            collisionGroup->AddCollider(wall.get());
            billiard->addChild(std::move(wall));

            wall = std::make_unique<BilliardWall>();
            wall->position = glm::vec3(0, 120, 75);
            wall->scale = glm::vec3(120, 10, 5);
            wall->collisionGroup = collisionGroup.get();
            wall->x = 60;
            wall->y = 5;
            wall->z = 2.5f;
            collisionGroup->AddCollider(wall.get());
            billiard->addChild(std::move(wall));

            wall = std::make_unique<BilliardWall>();
            wall->position = glm::vec3(0, -120, 75);
            wall->scale = glm::vec3(120, 10, 5);
            wall->x = 60;
            wall->y = 5;
            wall->z = 2.5f;
            wall->collisionGroup = collisionGroup.get();
            collisionGroup->AddCollider(wall.get());
            billiard->addChild(std::move(wall));
        }

        auto ball = std::make_unique<BilliardBall>();
        ball->collisionGroup = collisionGroup.get();
        ball->position = glm::vec3{0,69,74};
        ball->speed = glm::vec3(0,-100,0);
        ball->radius = ball->scale.x/2;
        collisionGroup->AddCollider(ball.get());
        billiard->addChild(std::move(ball));

        for (int i = 0; i < 5; i++){
            for (int j = 0; j <= i; j++){
                ball = std::make_unique<BilliardBall>();
                ball->collisionGroup = collisionGroup.get();
                ball->position = glm::vec3{0-((6.5/2)*i)+6.5*j,-55 -(6.5*i),74};
                ball->speed = glm::vec3(0,0,0);
                ball->radius = ball->scale.x/2;
                collisionGroup->AddCollider(ball.get());
                billiard->addChild(std::move(ball));
            }
        }

        collisionGroups.push_back(std::move(collisionGroup));
        addChild(std::move(billiard));
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

        addChild(std::move(chandelier));
    }
}