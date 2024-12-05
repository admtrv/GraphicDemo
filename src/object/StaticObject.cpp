/*
 * StaticObject.cpp
 */

#include "StaticObject.h"

// shared resources
std::unique_ptr<ppgso::Mesh> StaticObject::defaultMesh;
std::unique_ptr<ppgso::Shader> StaticObject::shader;
std::unique_ptr<ppgso::Texture> StaticObject::defaultTexture;

StaticObject::StaticObject(const std::string& textureFile, const std::string& modelFile) {
    // initialize shared resources

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
}

bool StaticObject::updateInternal(float dt) {
    generateModelMatrix();  // update transformation matrix
    return true;            // static object is always active
}

void StaticObject::renderInternal(ppgso::Shader *shader) {
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Transparency", transparency);
    shader->setUniform("TextureOffset", glm::vec2(0.0f, 0.0f));

    texture->bind();

    mesh->render();
}