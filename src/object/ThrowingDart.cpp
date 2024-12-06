#include "ThrowingDart.h"
#include "ObjectResources.h"

ThrowingDart::ThrowingDart(const std::string &textureFile, const std::string &modelFile) {
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
}

bool ThrowingDart::updateInternal(float dt) {
    switch(state){
        case 0:
            speed += speed * -0.01f;
            speed.y -= 98.0f*dt;
            position += speed*dt;

            if(position.z < targetCoordinates.z){
                position.z = targetCoordinates.z;
                speed.x *= 0.8;
                speed.z *= -0.3;

                glm::vec3 v = targetCoordinates-position;
                if(abs(glm::dot(v,v)) < targetRadius){
                    state = 1;
                } else {
                    state = 2;
                }
            }
            break;
        case 1:
            break;
        case 2:
            speed += speed * -0.01f;
            speed.y -= 98.0f*dt;
            position += speed*dt;
            if (position.y < -height){
                position.y = -height;
                state = 1;
            }
            break;
    }
    generateModelMatrix();  // update transformation matrix
    return true;            // static object is always active
}

void ThrowingDart::renderInternal(ppgso::Shader *shader) {
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("TextureCoordMod", textureCoordMod);
    shader->setUniform("TextureOffset", textureOffset);

    objectResources.getTexture(textureCode)->bind();

    objectResources.getMesh(meshCode)->render();
}
