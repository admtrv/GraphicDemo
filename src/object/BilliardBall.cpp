#include "BilliardBall.h"
#include "BilliardWall.h"
#include "CollisionGroup.h"

std::unique_ptr<ppgso::Mesh> BilliardBall::defaultMesh;
std::unique_ptr<ppgso::Shader> BilliardBall::shader;
std::unique_ptr<ppgso::Texture> BilliardBall::defaultTexture;

BilliardBall::BilliardBall(const std::string& textureFile, const std::string& modelFile)
        : Object() {// initialize shared resources
    if (!shader) {
        shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    }

    if (!modelFile.empty()) {
        try {
            mesh = std::make_unique<ppgso::Mesh>(modelFile);
            std::cout << "Loaded model: " << modelFile << std::endl;
        } catch (const std::runtime_error& e) {
            std::cerr << "Failed to load model: " << modelFile << ". Error: " << e.what() << std::endl;
        }
    } else if (!defaultMesh) {
        defaultMesh = std::make_unique<ppgso::Mesh>("cube.obj");
        mesh = std::make_unique<ppgso::Mesh>(*defaultMesh);
    } else {
        mesh = std::make_unique<ppgso::Mesh>(*defaultMesh);
    }


    // load texture only if textureFile is not empty
    if (!textureFile.empty()) {
        try {
            texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP(textureFile));
            std::cout << "Loaded texture: " << textureFile << std::endl;
        } catch (const std::runtime_error& e) {
            std::cerr << "Failed to load texture: " << textureFile << ". Error: " << e.what() << std::endl;
        }
    } else if (!defaultTexture) {
        defaultTexture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("lena.bmp"));
        texture = std::make_unique<ppgso::Texture>(*defaultTexture);
    } else {
        texture = std::make_unique<ppgso::Texture>(*defaultTexture);
    }
    scale = glm::vec3(BILLIARD_BALL_FACTOR, BILLIARD_BALL_FACTOR, BILLIARD_BALL_FACTOR);
    rotation = glm::vec3(glm::radians(-90.0f), 0.0f, glm::radians(-90.0f));
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

bool BilliardBall::updateInternal(Scene &scene, float dt) {
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

void BilliardBall::renderInternal(Scene& scene) {
    shader->use();

    // set light direction
    shader->setUniform("LightDirection", scene.light);

    // set matrix
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
    shader->setUniform("ModelMatrix", modelMatrix);

    // bind texture
    glActiveTexture(GL_TEXTURE0);
    texture->bind();
    shader->setUniform("Texture", 0);

    mesh->render();
}