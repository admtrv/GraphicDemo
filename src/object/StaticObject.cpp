/*
 * StaticObject.cpp
 */

#include "StaticObject.h"
#include "ObjectResources.h"


StaticObject::StaticObject(const std::string& textureFile, const std::string& modelFile) {
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

void StaticObject::setTextureSize(glm::vec2 textureSize) {
    textureCoordMod.x = textureSize.x / (float)objectResources.getTexture(textureCode)->image.width;
    textureCoordMod.y = textureSize.y / (float)objectResources.getTexture(textureCode)->image.height;
}
void StaticObject::setTextureOffset(glm::vec2 textureOffset) {
    this->textureOffset.x = textureOffset.x / (float)objectResources.getTexture(textureCode)->image.width;
    this->textureOffset.y = textureOffset.y / (float)objectResources.getTexture(textureCode)->image.height;
}

bool StaticObject::updateInternal(float dt) {
    //rotationMatrix = glm::rotate(glm::mat4(1), glm::radians(1.0f), glm::vec3(1,1,0)) * rotationMatrix;
    generateModelMatrix();  // update transformation matrix
    return true;            // static object is always active
}

void StaticObject::renderInternal(ppgso::Shader *shader) {
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Transparency", transparency);
    shader->setUniform("TextureCoordMod", textureCoordMod);
    shader->setUniform("TextureOffset", textureOffset);

    objectResources.getTexture(textureCode)->bind();

    objectResources.getMesh(meshCode)->render();
}