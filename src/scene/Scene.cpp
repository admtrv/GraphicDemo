#include "Scene.h"
#include "glm/gtx/string_cast.hpp"
#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>
#include <shaders/postprocess_vert_glsl.h>
#include <shaders/grayscale_frag_glsl.h>
#include <shaders/passthrough_frag_glsl.h>
#include <shaders/blur_frag_glsl.h>

#include <shaders/threshold_frag_glsl.h>
#include <shaders/gauss_frag_glsl.h>
#include <shaders/combine_frag_glsl.h>

Scene::Scene(){
    shadowMap = std::make_unique<ShadowMap>();
    shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    shader->use();
    auto texture_attrib = shader->getUniformLocation("Texture");
    glUniform1i(texture_attrib, 0);
    texture_attrib = shader->getUniformLocation("ShadowMap");
    glUniform1i(texture_attrib, 1);
}

void Scene::update(float dt) {
    // update the camera if it exists
    if (camera) {
        if(moving) bezierMove(dt);
        camera->update();
    }
    if (directionalLight) {
        directionalLight->update();
    }
    if(time > cameraControlTimes[1] && time < cameraControlTimes[3]) {
        if(time-lastDartTime > 1.0f){
            lastDartTime = time;
            activeDartboard->throwDart();
        }
    }
    if(time > cameraControlTimes[3] && !billiardActive) {
        billiardActive = true;

        activeBall->speed = glm::vec3(glm::linearRand(glm::vec2(-5,-100),glm::vec2(5,-160)),0);
    }

    // update all objects and remove those that not alive
    objects.erase(
            std::remove_if(objects.begin(), objects.end(),
                           [this, dt](const std::unique_ptr<Object>& obj) {
                               return !obj->update(dt);
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
    glClearColor(.5f, .5f, .8f, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 2) rendering scene with shadows and additional sources in FBO
    shader->use();
    shader->setUniform("LightSpaceMatrix", directionalLight->projectionMatrix * directionalLight->viewMatrix);
    shader->setUniform("LightDirection", light);
    shader->setUniform("ProjectionMatrix", camera->projectionMatrix);
    shader->setUniform("ViewMatrix", camera->viewMatrix);
    shader->setUniform("ViewPos", camera->position);
    shader->setUniform("shininess", shininess);
    shader->setUniform("specularIntensity", specularIntensity);

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
        if(light->isDirectionalLight()) {
            AdditionalDirectionalLightPositions[AdditionalDirectionalLightCount] = light->getPosition();
            AdditionalDirectionalLightDirections[AdditionalDirectionalLightCount] = light->getDirection();
            AdditionalDirectionalLightColors[AdditionalDirectionalLightCount] = light->getColor();
            AdditionalDirectionalLightIntensities[AdditionalDirectionalLightCount] = light->getIntensity();
            AdditionalDirectionalLightCount++;
        } else {
            AdditionalLightPositions[AdditionalLightCount] = light->getPosition();
            AdditionalLightColors[AdditionalLightCount] = light->getColor();
            AdditionalLightIntensities[AdditionalLightCount] = light->getIntensity();
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

    /*std::cout << "LightCount: " << lights.size() << std::endl;
    for (size_t i = 0; i < lights.size(); ++i) {
        std::cout << "Light " << i << ": Position = " << glm::to_string(lights[i].getPosition())
                  << ", Direction = " << glm::to_string(lights[i].getDirection())
                  << ", Color = " << glm::to_string(lights[i].getColor())
                  << ", Intensity = " << lights[i].getIntensity()
                  << ", Type = " << (lights[i].isDirectionalLight() ? "Directional" : "Point") << std::endl;
    }*/

    // drawing objects in FBO
    for (auto& obj : objects) {
        obj->render(shader.get());
    }

    // 3) apply post-processing:
    // now render a full screen quad with the scene in colorTexture, using postProcessShader.
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // switch to the default framebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (currentFilter == PostProcessMode::BLOOM) {
        // 1. Threshold pass
        glBindFramebuffer(GL_FRAMEBUFFER, brightFBO);
        glClear(GL_COLOR_BUFFER_BIT);
        thresholdShader->use();
        thresholdShader->setUniform("sceneTexture", 0);
        thresholdShader->setUniform("threshold", thresholdValue);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, colorTexture);
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // 2. blur pass
        bool horizontal = true, first_iteration = true;
        gaussShader->use();
        for (int i = 0; i < blurIterations; i++) {
            glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
            gaussShader->setUniform("horizontal", horizontal);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, first_iteration ? brightTexture : pingpongColorbuffers[!horizontal]);
            glBindVertexArray(quadVAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            horizontal = !horizontal;
            if (first_iteration)
                first_iteration = false;
        }

        // 3. Combine pass
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        combineShader->use();
        combineShader->setUniform("sceneTexture", 0);
        combineShader->setUniform("bloomTexture", 1);
        combineShader->setUniform("bloomIntensity", bloomIntensity);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, colorTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[!horizontal]);
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

    } else {
        // usual post-processing
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

void Scene::setupBloom()
{
    // FBO for bright pixels
    glGenFramebuffers(1, &brightFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, brightFBO);

    glGenTextures(1, &brightTexture);
    glBindTexture(GL_TEXTURE_2D, brightTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, windowWidth, windowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brightTexture, 0);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "Bright FBO not complete!" << std::endl;

    // ping-pong framebuffers for blur
    glGenFramebuffers(2, pingpongFBO);
    glGenTextures(2, pingpongColorbuffers);
    for (int i = 0; i < 2; i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
        glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, windowWidth, windowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cerr << "Pingpong FBO " << i << " not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Scene::loadBloomShaders() {
    thresholdShader = std::make_unique<ppgso::Shader>(postprocess_vert_glsl, threshold_frag_glsl);
    gaussShader = std::make_unique<ppgso::Shader>(postprocess_vert_glsl, gauss_frag_glsl);
    combineShader = std::make_unique<ppgso::Shader>(postprocess_vert_glsl, combine_frag_glsl);
}

// Compute points for Bezier curve using 4 control points
glm::vec3 Scene::bezierPoint(const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3, const float t) {
    glm::vec3 p11 = glm::lerp(p0,p1,t);
    glm::vec3 p21 = glm::lerp(p1,p2,t);
    glm::vec3 p31 = glm::lerp(p2,p3,t);

    glm::vec3 p22 = glm::lerp(p11,p21,t);
    glm::vec3 p32 = glm::lerp(p21,p31,t);

    glm::vec3 p33 = glm::lerp(p22,p32,t);
    return p33;
}

// Compute points for a sequence of Bezier curves defined by a vector of control points
// Each bezier curve will reuse the end point of the previous curve
// count - Number of points to generate on each curve
void Scene::bezierShape(int count) {
    for(int i = 1; i < (int) cameraControlPositions.size(); i+=3) {
        for (int j = 0; j <= count; j++) {
            glm::vec3 point = bezierPoint(cameraControlPositions[i - 1], cameraControlPositions[i], cameraControlPositions[i + 1], cameraControlPositions[i + 2], (float)(j) / (float)(count));
            auto ball = std::make_unique<StaticObject>("sphere.bmp", "sphere.obj");
            ball->position = point;
            ball->scale = glm::vec3(0.1,0.1,0.1);
            objects.push_back(std::move(ball));
        }
    }

    for (int i = 0; i < (int) cameraControlPositions.size(); i++){
        auto s = std::make_unique<StaticObject>("sphere.bmp","sphere.obj");
        s->position = cameraControlPositions[i];
        objects.push_back(std::move(s));s = std::make_unique<StaticObject>("sphere.bmp","sphere.obj");
    }
}


void Scene::bezierMove(float dt) {
    time += dt;
    int i = 1;
    while (i < cameraControlTimes.size()){
        if (cameraControlTimes[i] > time) break;
        i++;
    }
    int j = i*3;
    if(j < cameraControlPositions.size()){
        float ratio = (time - cameraControlTimes[i-1])/(cameraControlTimes[i] - cameraControlTimes[i-1]);
        //std::cout <<"i:" << i << " time: " << time << " ratio: " << ratio << "\n";
        camera->position = bezierPoint(cameraControlPositions[j-3], cameraControlPositions[j - 2], cameraControlPositions[j - 1], cameraControlPositions[j], ratio);
        camera->direction = bezierPoint(cameraControlDirections[j-3], cameraControlDirections[j - 2], cameraControlDirections[j - 1], cameraControlDirections[j], ratio);;
    }
}
