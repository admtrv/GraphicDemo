#include "Scene.h"
#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>
Scene::Scene(){
    shadowMap = std::make_unique<ShadowMap>();
    shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    shader->use();
    auto texture_attrib = shader->getUniformLocation("Texture");
    glUniform1i(texture_attrib, 0);
    texture_attrib = shader->getUniformLocation("ShadowMap");
    glUniform1i(texture_attrib, 1);
}

void Scene::update(float time) {
    // update the camera if it exists
    if (camera) {
        camera->update();
    }
    if (directionalLight) {
        directionalLight->update();
    }

    // update all objects and remove those that not alive
    objects.erase(
            std::remove_if(objects.begin(), objects.end(),
                           [this, time](const std::unique_ptr<Object>& obj) {
                               return !obj->update(time);
                           }),
            objects.end()
    );
}

void Scene::render() {
    // set gray background
    glClearColor(.5f, .5f, .5f, 0);
    // clear depth and color buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //render shadowMap
    glCullFace(GL_FRONT);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowMap->depthMapFBO);
    glViewport(0, 0, shadowMap->SHADOW_WIDTH, shadowMap->SHADOW_HEIGHT);
    glClear(GL_DEPTH_BUFFER_BIT);

    shader->use();
    shader->setUniform("ProjectionMatrix", directionalLight->projectionMatrix);
    shader->setUniform("ViewMatrix", directionalLight->viewMatrix);
    for (auto& obj : objects) {
        obj->render(shader.get());
    }

    // reset viewport
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glCullFace(GL_BACK);
    glViewport(0, 0, windowWidth, windowHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->setUniform("LightSpaceMatrix", directionalLight->projectionMatrix*directionalLight->viewMatrix);
    shader->setUniform("LightDirection", light);
    shader->setUniform("ProjectionMatrix", camera->projectionMatrix);
    shader->setUniform("ViewMatrix", camera->viewMatrix);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, shadowMap->depthMap);
    // render all objects in the scene
    for (auto& obj : objects) {
        obj->render(shader.get());
    }
}