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
}

bool StaticObject::updateInternal(Scene& scene, float dt) {
    generateModelMatrix();  // update transformation matrix
    return true;            // static object is always active
}

void StaticObject::renderInternal(Scene& scene) {
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
