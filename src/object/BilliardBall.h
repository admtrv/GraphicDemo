#ifndef PPGSOHERNA_BILLIARDBALL_H
#define PPGSOHERNA_BILLIARDBALL_H

#include "Object.h"
#include "ppgso.h"
#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>
#include "CollisionCommon.h"

#define BILLIARD_BALL_FACTOR 6.0f

class BilliardBall : public Object {
public:
    explicit BilliardBall(const std::string& textureFile = "billiardBalls.bmp", const std::string& modelFile = "sphere.obj");
    void setTextureSize(glm::vec2 textureSize);
    void setTextureOffset(glm::vec2 textureOffset);
    glm::vec3 GetClosestPoint(BilliardWall* object);
    bool CheckCollision(BilliardBall* object);
    bool CheckCollision(BilliardWall* object);
    bool moving = false;

    float radius;
    glm::vec3 speed = {0,0,0};

    CollisionGroup* collisionGroup;

    bool updateInternal(float dt) override;   // update object
    void renderInternal(ppgso::Shader *shader) override;
private:
    std::string meshCode;
    std::string textureCode;

    std::unique_ptr<ppgso::Mesh> mesh;       // uniq mesh
    std::unique_ptr<ppgso::Texture> texture; // uniq texture
};


#endif //PPGSOHERNA_BILLIARDBALL_H
