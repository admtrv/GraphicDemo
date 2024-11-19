#ifndef PPGSOHERNA_COLLISIONGROUP_H
#define PPGSOHERNA_COLLISIONGROUP_H

#include <list>
#include "ppgso.h"
#include "CollisionCommon.h"

class CollisionGroup {
    std::vector<BilliardWall*> boxColliders;
    std::vector<BilliardBall*> sphereColliders;
public:
    bool CheckCollision(BilliardBall* object, BilliardWall* &boxOut, BilliardBall* &sphereOut);
    void AddCollider(BilliardBall* collider);
    void AddCollider(BilliardWall* collider);
};


#endif //PPGSOHERNA_COLLISIONGROUP_H
