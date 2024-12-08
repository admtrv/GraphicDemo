//
// Created by nortl on 08/12/2024.
//

#include "Outside.h"
#include "StaticObject.h"
#include "Tree.h"
#define WIDTH 256
#define DEPTH 256
Outside::Outside() {
    auto floor = std::make_unique<StaticObject>("grass.bmp");
    floor->scale = glm::vec3(WIDTH,0.01,DEPTH);
    addChild(std::move(floor));

    for(int i = 0; i < 1000; i++){
        auto tree = std::make_unique<Tree>();
        glm::vec2 pos;
        do {
            pos = glm::linearRand(glm::vec2(-WIDTH/2,-DEPTH/2),glm::vec2(WIDTH/2,DEPTH/2));
        } while (pos.x > -20 && pos.y > -20 && pos.x < 20 && pos.y < 20);
        glm::vec2 scale = glm::linearRand(glm::vec2(1,2),glm::vec2(2,4));
        tree->position = glm::vec3(pos[0],0,pos[1]);
        tree->scale = glm::vec3(scale[0],scale[1],scale[0]);
        addChild(std::move(tree));
    }
}
