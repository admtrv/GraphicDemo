#ifndef PPGSOHERNA_BILLIARDWALL_H
#define PPGSOHERNA_BILLIARDWALL_H

#include "StaticObject.h"
#include "CollisionCommon.h"

class BilliardWall : public StaticObject{
public:
    explicit BilliardWall(const std::string& textureFile = "lena.bmp", const std::string& modelFile = "cube.obj");
    CollisionGroup* collisionGroup;
    float x,y,z;
/*
    bool updateInternal(Scene& scene, float dt) override;   // update object
    void renderInternal(Scene& scene) override;*/
};


#endif //PPGSOHERNA_BILLIARDWALL_H
