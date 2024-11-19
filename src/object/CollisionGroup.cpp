#include "CollisionGroup.h"
#include "BilliardWall.h"
#include "BilliardBall.h"

bool CollisionGroup::CheckCollision(BilliardBall* object, BilliardWall* &boxOut, BilliardBall* &sphereOut) {
    sphereOut = nullptr;
    boxOut = nullptr;
    for (auto c : boxColliders){
        if(object->CheckCollision(c)){
            boxOut = c;
            return true;
        }
    }

    for (auto c : sphereColliders){
        if(object != c && object->CheckCollision(c)){
            sphereOut = c;
            return true;
        }
    }
    return false;
}

void CollisionGroup::AddCollider(BilliardBall* collider) {
    sphereColliders.push_back(collider);
}

void CollisionGroup::AddCollider(BilliardWall* collider) {
    boxColliders.push_back(collider);
}