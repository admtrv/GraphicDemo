#include "BilliardBall.h"
#include "BilliardWall.h"
#include "CollisionGroup.h"
#include "ObjectResources.h"


BilliardBall::BilliardBall(const std::string& textureFile, const std::string& modelFile)
        : Object() {
    // initialize shared resources
    if (!modelFile.empty()) {
        if(objectResources.addMesh(modelFile)){
            meshCode = modelFile;
        } else {
            if(objectResources.addMesh("cube.obj"))
                meshCode = "cube.obj";
            std::cout << "Load default mesh\n";
        }
    } else {
        if(objectResources.addMesh("cube.obj"))
            meshCode = "cube.obj";
        std::cout << "Load default mesh\n";
    }

    // load texture only if textureFile is not empty
    if (!textureFile.empty()) {
        if(objectResources.addTexture(textureFile)){
            textureCode = textureFile;
        } else {
            if(objectResources.addTexture("lena.bmp"))
                textureCode = "lena.bmp";
            std::cout << "Load default mesh\n";
        }
    } else {
        if(objectResources.addTexture("lena.bmp"))
            textureCode = "lena.bmp";
        std::cout << "Load default mesh\n";
    }

    scale = glm::vec3(BILLIARD_BALL_FACTOR, BILLIARD_BALL_FACTOR, BILLIARD_BALL_FACTOR);
    rotation = glm::vec3(glm::radians(-90.0f), 0.0f, glm::radians(-90.0f));
}


void BilliardBall::setTextureSize(glm::vec2 textureSize) {
    textureCoordMod.x = textureSize.x / (float)objectResources.getTexture(textureCode)->image.width;
    textureCoordMod.y = textureSize.y / (float)objectResources.getTexture(textureCode)->image.height;
}
void BilliardBall::setTextureOffset(glm::vec2 textureOffset) {
    this->textureOffset.x = textureOffset.x / (float)objectResources.getTexture(textureCode)->image.width;
    this->textureOffset.y = textureOffset.y / (float)objectResources.getTexture(textureCode)->image.height;
}

glm::vec3 BilliardBall::GetClosestPoint(BilliardWall* object) {
    glm::vec3 centreVector = position - object->position;
    float x = std::max(-object->x,std::min(object->x,centreVector.x));
    float y = std::max(-object->y,std::min(object->y,centreVector.y));
    float z = std::max(-object->z,std::min(object->z,centreVector.z));
    return glm::vec3 {object->position.x+x,object->position.y+y,object->position.z+z};
}
bool BilliardBall::CheckCollision(BilliardWall* object) {
    glm::vec3 closestPoint = GetClosestPoint(object);
    return glm::length(closestPoint - position) < radius;
}

bool BilliardBall::CheckCollision(BilliardBall* object) {
    return glm::length(object->position - position) < object->radius + radius;
}

bool BilliardBall::updateInternal(float dt) {
    moving = true;
    if(glm::length(speed) < 0.01){
        speed = glm::vec3(0,0,0);
        moving = false;
    }

    if (moving){
        glm::vec3 (speedDelta) = -speed * 0.05f;
        if (glm::length(speedDelta) > 0.05f){
            speedDelta = glm::normalize(speedDelta) * 0.05f;
        }
        speed += speedDelta;
        position += speed*dt;


        BilliardWall* boxCollisionObject;
        BilliardBall* sphereCollisionObject;
        int counter = 0;
        while(counter < 1 && collisionGroup->CheckCollision(this, boxCollisionObject, sphereCollisionObject)){
            if(boxCollisionObject != nullptr) {
                glm::vec3 closestPoint = GetClosestPoint(boxCollisionObject);
                glm::vec3 d = position - closestPoint;
                float frac = glm::length(d)/radius;
                position += (1 - frac) * d;

                if (position.x <= (boxCollisionObject->position.x) - boxCollisionObject->x || position.x >= (boxCollisionObject->position.x) + boxCollisionObject->x){
                    speed.x = -speed.x;
                }
                if (position.y <= (boxCollisionObject->position.y) - boxCollisionObject->y || position.y >= (boxCollisionObject->position.y) + boxCollisionObject->y){
                    speed.y = -speed.y;
                }
                if (position.z <= (boxCollisionObject->position.z) - boxCollisionObject->z || position.z >= (boxCollisionObject->position.z) + boxCollisionObject->z){
                    speed.y = -speed.y;
                }
            }
            else if (sphereCollisionObject != nullptr) {
                //Move object out of collision
                glm::vec3 distance = position - sphereCollisionObject->position;
                float overlap = radius + sphereCollisionObject->radius - glm::length(distance);
                position -= glm::normalize(speed)*overlap;

                //Calculate the angle of rotation
                glm::vec3 direction = sphereCollisionObject->position - position;
                double angle = -atan(direction.y/direction.x);
                auto u1 = glm::mat3({cos(angle),sin(angle),0,
                                    -sin(angle),cos(angle),0,
                                     0,0,1})*speed;

                auto u2 = glm::mat3({cos(angle),sin(angle),0,
                                     -sin(angle),cos(angle),0,
                                     0,0,1}) * sphereCollisionObject->speed;

                //Apply 1-Dimensional Elastic Collision Formulas
                speed = glm::mat3({cos(-angle),sin(-angle),0,
                                   -sin(-angle),cos(-angle),0,
                                   0,0,1})*glm::vec3(u2.x,u1.y,u1.z);

                sphereCollisionObject->speed = glm::mat3({cos(-angle),sin(-angle),0,
                                                          -sin(-angle),cos(-angle),0,
                                                          0,0,1}) * glm::vec3(u1.x,u2.y,u2.z);
            }
            counter++;
        }
    }

    generateModelMatrix();  // update transformation matrix
    return true;
}

void BilliardBall::renderInternal(ppgso::Shader *shader) {
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("TextureCoordMod", textureCoordMod);
    shader->setUniform("TextureOffset", textureOffset);

    // bind texture
    objectResources.getTexture(textureCode)->bind();

    objectResources.getMesh(meshCode)->render();
}