#include <glm/gtc/random.hpp>
#include "StaticObject.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>
#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>


// Static resources
std::unique_ptr<ppgso::Mesh> StaticObject::mesh;
std::unique_ptr<ppgso::Texture> StaticObject::texture;
std::unique_ptr<ppgso::Shader> StaticObject::shader;

StaticObject::StaticObject() {

    // Initialize static resources if needed
    //if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!shader) shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
    //if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("asteroid.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("cube.obj");
    color = glm::vec3(1,0,0);
}

bool StaticObject::updateInternal(Scene &scene, float dt) {
    generateModelMatrix();
    return true;
}


void StaticObject::renderInternal(Scene &scene) {
    shader->use();

    // Set up light
    //shader->setUniform("LightDirection", scene.lightDirection);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("OverallColor", color);
    //shader->setUniform("Texture", *texture);
    mesh->render();
}