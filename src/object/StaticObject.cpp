/*
 * StaticObject.cpp
 */

#include "StaticObject.h"

// shared resources
std::unique_ptr<ppgso::Mesh> StaticObject::mesh;
std::unique_ptr<ppgso::Shader> StaticObject::shader;

StaticObject::StaticObject(const std::string& textureFile) {
    // initialize shared resources if needed
    if (!shader) {
        shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    }
    if (!mesh) {
        mesh = std::make_unique<ppgso::Mesh>("cube.obj");
    }

    // load texture only if textureFile is not empty
    if (!textureFile.empty()) {
        try {
            texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP(textureFile));
            std::cout << "Loaded texture: " << textureFile << std::endl;
        } catch (const std::runtime_error& e) {
            std::cerr << "Failed to load texture: " << textureFile << ". Error: " << e.what() << std::endl;
        }
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

    // set transparency
    shader->setUniform("Transparency", 1.0f);

    // set texture offset
    shader->setUniform("TextureOffset", glm::vec2(0.0f, 0.0f));

    // bind texture
    glActiveTexture(GL_TEXTURE0);
    texture->bind();
    shader->setUniform("Texture", 0);

    mesh->render();
}
