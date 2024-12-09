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
#include "src/object/Dartboard.h"
#include "src/object/BilliardBall.h"
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

    void update(float dt);    // update all objects in the scene
    void render();              // render all objects in the scene

    std::vector<Light*> lights;

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

    std::vector<glm::vec3> cameraControlPositions;
    std::vector<glm::vec3> cameraControlDirections;
    std::vector<float> cameraControlTimes;
    float time = 0.0f;

    Dartboard* activeDartboard;
    BilliardBall* activeBall;
    float lastDartTime = 0;
    bool billiardActive = false;
    bool moving = false;

    glm::vec3 bezierPoint(const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3, const float t);
    void bezierShape(int count);
    void bezierMove(float dt);

    float shininess =  32.0f;
    float specularIntensity = 1.0f;

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
