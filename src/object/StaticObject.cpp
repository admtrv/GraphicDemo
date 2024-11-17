/*
 * StaticObject.cpp
 */

#include "StaticObject.h"

// shared resources
std::unique_ptr<ppgso::Mesh> StaticObject::mesh;
std::unique_ptr<ppgso::Texture> StaticObject::texture;
std::unique_ptr<ppgso::Shader> StaticObject::shader;

StaticObject::StaticObject() {
    // initialize shared resources if needed
    if (!shader) {
        shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
    }
    if (!mesh) {
        mesh = std::make_unique<ppgso::Mesh>("cube.obj");
    }
    color = glm::vec3(1.0f, 0.0f, 0.0f); // default color: red
}

bool StaticObject::updateInternal(Scene& scene, float dt) {
    generateModelMatrix();  // update transformation matrix
    return true;            // static object is always active
}

void StaticObject::renderInternal(Scene& scene) {
    shader->use();

    // set camera and transformation matrices
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
    shader->setUniform("ModelMatrix", modelMatrix);

    // set object color
    shader->setUniform("OverallColor", color);

    // render the mesh
    mesh->render();
}