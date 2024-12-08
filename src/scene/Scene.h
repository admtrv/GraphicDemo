/*
 * Scene.h
 */

#pragma once

#include <memory>
#include <map>
#include <list>
#include <algorithm>

#include "src/object/Object.h"
#include "src/object/StaticObject.h"
#include "src/camera/Camera.h"
#include "src/camera/ShadowMap.h"
#include "src/light/Light.h"

enum class PostProcessMode {
    NONE,
    GRAYSCALE,
    BLUR,
    BLOOM
};

// scene aggregates all scene-related data, including objects and camera
class Scene {
public:
    explicit Scene();

    void update(float time);    // update all objects in the scene
    void render();              // render all objects in the scene

    std::vector<Light> lights;

    int windowWidth, windowHeight;
    std::unique_ptr<Camera> camera;                 // camera object
    std::unique_ptr<Camera> directionalLight;
    std::unique_ptr<ppgso::Shader> shader;
    std::unique_ptr<ShadowMap> shadowMap;           // ShadowMap object
    std::list< std::unique_ptr<Object> > objects;   // all objects to be rendered in scene
    glm::vec3 light{0.5f, 1.0f, 1.0f};

    void setupPostProcessing();
    void setupBloom();
    void loadPostProcessShaders();
    void loadBloomShaders();
    PostProcessMode currentFilter = PostProcessMode::NONE;

private:
    unsigned int fbo = 0;
    unsigned int colorTexture = 0;
    unsigned int rbo = 0;
    unsigned int quadVAO = 0;
    unsigned int quadVBO = 0;
    std::unique_ptr<ppgso::Shader> grayscaleShader;
    std::unique_ptr<ppgso::Shader> passthroughShader;
    std::unique_ptr<ppgso::Shader> blurShader;

    unsigned int brightFBO, brightTexture;
    unsigned int pingpongFBO[2], pingpongColorbuffers[2];

    std::unique_ptr<ppgso::Shader> thresholdShader;
    std::unique_ptr<ppgso::Shader> gaussShader;
    std::unique_ptr<ppgso::Shader> combineShader;

    bool horizontal = true;
    bool first_iteration = true;
    int blurIterations = 10;
    float thresholdValue = 0.5f;
    float bloomIntensity = 1.0f;

};
