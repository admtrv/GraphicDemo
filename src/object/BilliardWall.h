#ifndef PPGSOHERNA_BILLIARDWALL_H
#define PPGSOHERNA_BILLIARDWALL_H

#include "VirtualObject.h"
#include "CollisionCommon.h"

class BilliardWall : public VirtualObject{
public:
    CollisionGroup* collisionGroup;
    float x,y,z;
};


#endif //PPGSOHERNA_BILLIARDWALL_H
