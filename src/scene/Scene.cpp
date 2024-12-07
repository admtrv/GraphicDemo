#include "Scene.h"
#include "glm/gtx/string_cast.hpp"
#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>
#include <shaders/postprocess_vert_glsl.h>
#include <shaders/grayscale_frag_glsl.h>
#include <shaders/passthrough_frag_glsl.h>
#include <shaders/blur_frag_glsl.h>

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
    // 1) shadow rendering
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

    // back to rendering to the post-processing framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, fbo); // switching to FBO to render the scene
    glViewport(0, 0, windowWidth, windowHeight);
    glCullFace(GL_BACK);
    glClearColor(.5f, .5f, .5f, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 2) rendering scene with shadows and additional sources in FBO
    shader->use();
    shader->setUniform("LightSpaceMatrix", directionalLight->projectionMatrix * directionalLight->viewMatrix);
    shader->setUniform("LightDirection", light);
    shader->setUniform("ProjectionMatrix", camera->projectionMatrix);
    shader->setUniform("ViewMatrix", camera->viewMatrix);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, shadowMap->depthMap);

    shader->setUniform("LightCount", static_cast<int>(lights.size()));

    glm::vec3 AdditionalDirectionalLightPositions[8];
    glm::vec3 AdditionalDirectionalLightDirections[8];
    glm::vec3 AdditionalDirectionalLightColors[8];
    float AdditionalDirectionalLightIntensities[8];
    int AdditionalDirectionalLightCount = 0;

    glm::vec3 AdditionalLightPositions[8];
    glm::vec3 AdditionalLightColors[8];
    float AdditionalLightIntensities[8];
    int AdditionalLightCount = 0;

    for (auto light : lights) {
        if(light.isDirectionalLight()) {
            AdditionalDirectionalLightPositions[AdditionalDirectionalLightCount] = light.getPosition();
            AdditionalDirectionalLightDirections[AdditionalDirectionalLightCount] = light.getDirection();
            AdditionalDirectionalLightColors[AdditionalDirectionalLightCount] = light.getColor();
            AdditionalDirectionalLightIntensities[AdditionalDirectionalLightCount] = light.getIntensity();
            AdditionalDirectionalLightCount++;
        } else {
            AdditionalLightPositions[AdditionalLightCount] = light.getPosition();
            AdditionalLightColors[AdditionalLightCount] = light.getColor();
            AdditionalLightIntensities[AdditionalLightCount] = light.getIntensity();
            AdditionalLightCount++;
        }
    }

    for (int i = 0; i < AdditionalDirectionalLightCount; i++){
        shader->setUniform("AdditionalDirectionalLightPositions[" + std::to_string(i) + "]", AdditionalDirectionalLightPositions[i]);
        shader->setUniform("AdditionalDirectionalLightDirections[" + std::to_string(i) + "]", AdditionalDirectionalLightDirections[i]);
        shader->setUniform("AdditionalDirectionalLightColors[" + std::to_string(i) + "]", AdditionalDirectionalLightColors[i]);
        shader->setUniform("AdditionalDirectionalLightIntensities[" + std::to_string(i) + "]", AdditionalDirectionalLightIntensities[i]);
    }
    shader->setUniform("AdditionalDirectionalLightCount", (float)AdditionalDirectionalLightCount);

    for (int i = 0; i < AdditionalLightCount; i++){
        shader->setUniform("AdditionalLightPositions[" + std::to_string(i) + "]", AdditionalLightPositions[i]);
        shader->setUniform("AdditionalLightColors[" + std::to_string(i) + "]", AdditionalLightColors[i]);
        shader->setUniform("AdditionalLightIntensities[" + std::to_string(i) + "]", AdditionalLightIntensities[i]);
    }
    shader->setUniform("AdditionalLightCount", (float)AdditionalLightCount);

    std::cout << "LightCount: " << lights.size() << std::endl;
    for (size_t i = 0; i < lights.size(); ++i) {
        std::cout << "Light " << i << ": Position = " << glm::to_string(lights[i].getPosition())
                  << ", Direction = " << glm::to_string(lights[i].getDirection())
                  << ", Color = " << glm::to_string(lights[i].getColor())
                  << ", Intensity = " << lights[i].getIntensity()
                  << ", Type = " << (lights[i].isDirectionalLight() ? "Directional" : "Point") << std::endl;
    }

    // drawing objects in FBO
    for (auto& obj : objects) {
        obj->render(shader.get());
    }

    // 3) apply post-processing:
    // now render a full screen quad with the scene in colorTexture, using postProcessShader.
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // switch to the default framebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ppgso::Shader* chosenShader = nullptr;
    switch(currentFilter) {
        case PostProcessMode::NONE:
            chosenShader = passthroughShader.get();
            break;
        case PostProcessMode::GRAYSCALE:
            chosenShader = grayscaleShader.get();
            break;
        case PostProcessMode::BLUR:
            chosenShader = blurShader.get();
            break;
        //case PostProcessMode::BLOOM:
            //chosenShader = bloomShader.get();
            break;
        default:
            return;
    }

    chosenShader->use();
    chosenShader->setUniform("screenTexture", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, colorTexture);
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}


void Scene::setupPostProcessing() {
    // making framebuf
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // creating texture
    glGenTextures(1, &colorTexture);
    glBindTexture(GL_TEXTURE_2D, colorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth, windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);

    // creating renderbuf
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, windowWidth, windowHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "FrameBuffer is not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // full-screen quad
    float quadVertices[] = {
            // positions    // texCoords
            -1.0f,  1.0f,   0.0f, 1.0f,
            -1.0f, -1.0f,   0.0f, 0.0f,
            1.0f,  1.0f,   1.0f, 1.0f,

            -1.0f, -1.0f,   0.0f, 0.0f,
            1.0f, -1.0f,   1.0f, 0.0f,
            1.0f,  1.0f,   1.0f, 1.0f
    };
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));
    glBindVertexArray(0);
}

void Scene::loadPostProcessShaders() {
    passthroughShader = std::make_unique<ppgso::Shader>(postprocess_vert_glsl, passthrough_frag_glsl);
    grayscaleShader   = std::make_unique<ppgso::Shader>(postprocess_vert_glsl, grayscale_frag_glsl);
    blurShader        = std::make_unique<ppgso::Shader>(postprocess_vert_glsl, blur_frag_glsl);
    //bloomShader       = std::make_unique<ppgso::Shader>(postprocess_vert_glsl, bloom_frag_glsl);
}

