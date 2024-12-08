/*
 * main_dmitriev.cpp
 */

#pragma once

#include <iostream>
#include <list>
#include <ppgso/ppgso.h>

#include "scene/Scene.h"
#include "src/camera/PersCamera.h"
#include "src/camera/OrthoCamera.h"
#include "camera/DebugCamera.h"
#include "object/Room.h"
#include "object/Outside.h"
#include "src/object/ObjectResources.h"

#define WINDOW_WIDTH    1920
#define WINDOW_HEIGHT   1080

class SceneWindow : public ppgso::Window {
private:
    Scene scene;
    std::unique_ptr<DebugCamera> debugCamera;

    void initScene() {
        if (!scene.objects.empty()) return;
        scene.windowWidth = WINDOW_WIDTH;
        scene.windowHeight = WINDOW_HEIGHT;
        // create camera
        auto camera = std::make_unique<PersCamera>(60.0f, (float)width / (float)height, 0.1f, 100.0f);
        camera->position = glm::vec3(-3.56922, 6.68757, 13.4624);
        camera->direction = glm::vec3(-0.0315005, -0.144863, -0.98895);
        debugCamera = std::make_unique<DebugCamera>(camera.get());
        scene.camera = std::move(camera);


        scene.cameraControlPositions = std::vector<glm::vec3> {
                {-3.56922, 6.68757, 13.4624},
                {-4.16982, 5.25242, 3.5708},
                {-3.68975, 5.14495, -6.85578},
                {0.890161, 5.72851, -6.12124},
                {5.16384, 7.09585, -0.0985885},
                {-3.09901, 3.92229, -2.63013},
                {-5.96342, 3.66262, -3.94304},
                {-10.1974, 7.02118, -12.24521},
                {-9.77796, 6.66118, -18.1408},
                {3.98492, 6.60588, -12.1927},
                {7.54784, 5.9844, -7.83749},
                {5.92129, 5.9844, -2.71433},
                {2.26743, 5.9844, -0.374837},
                {-3.77929, 5.9844, -2.53799},
                {-3.82258, 4.65919, -7.56654},
                {-0.142, 4.18387, -11.5401}
        };
        scene.cameraControlTimes = std::vector<float> {
                0.0f,
                8.0f,
                16.0f,
                24.0f,
                32.0f,
                40.0f
        };
        scene.cameraControlDirections = std::vector<glm::vec3> {
                {-0.0315005, -0.144863, -0.98895},
                {0.00904859, -0.0229696, -0.999695},
                {-0.486955, -0.0229696, -0.873125},
                {-0.491105, -0.0640709, -0.868741},
                {-0.491105, -0.0640709, -0.868741},
                {0.41874, 0.0870477, -0.903925},
                {0.41874, 0.0870477, -0.903925},
                {0.724334, -0.687123, -0.0565858},
                {0.857525, -0.359755, 0.367732},
                {-0.779924, -0.413356, 0.469953},
                {-0.94405, -0.316957, -0.091148},
                {-0.76542, -0.316957, -0.560062},
                {-0.0122832, -0.445799, -0.895049},
                {0.546035, -0.445955, -0.709204},
                {0.955717, -0.28917, 0.0546511},
                {0.457523, -0.100155, 0.883539}

        };
        //scene.bezierShape(100);

        // create directional Light
        auto directionalLight = std::make_unique<OrthoCamera>(-80.0f, 80.0f, -60.0f, 20.0f, 0.1f, 5000.0f);
        directionalLight->position = glm::vec3(512.0f, 256.0f, -128.0f);
        directionalLight->direction = glm::normalize(-directionalLight->position);
        scene.directionalLight = std::move(directionalLight);

        // create room
        auto room = std::make_unique<Room>(scene);
        auto* roomPtr = room.get();
        scene.objects.push_back(std::move(room));

        // create arcade automates
        roomPtr->addArcades(scene);

        // create billiard tables
        scene.activeBall = roomPtr->addBilliards();

        // create chandeliers above billiard tables
        roomPtr->addChandeliers(scene);

        // create dartboards
        scene.activeDartboard = roomPtr->addDartboards();

        auto outside = std::make_unique<Outside>();
        roomPtr->addChild(std::move(outside));

        auto dart = std::make_unique<ThrowingDart>();
    }


public:
    SceneWindow() : Window{"Playroom", WINDOW_WIDTH, WINDOW_HEIGHT} {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        glDisable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);

        initScene();
        scene.setupPostProcessing();
        scene.loadPostProcessShaders();

        scene.setupBloom();
        scene.loadBloomShaders();
    }

    void onKey(int key, int scanCode, int action, int mods) override {
        debugCamera->keyboard[key] = action;
        if(key == GLFW_KEY_X && action){
            //scene.activeDartboard->throwDart();
            if(!scene.moving){
                scene.moving = true;
                scene.time = 0;
            }
        }

        // key M switch: none -> grayscale -> blur -> bloom -> none -> ...
        if (key == GLFW_KEY_M && action == GLFW_PRESS) {
            switch(scene.currentFilter) {
                case PostProcessMode::NONE:
                    scene.currentFilter = PostProcessMode::GRAYSCALE;
                    break;
                case PostProcessMode::GRAYSCALE:
                    scene.currentFilter = PostProcessMode::BLUR;
                    break;
                case PostProcessMode::BLUR:
                    scene.currentFilter = PostProcessMode::BLOOM;
                    break;
                case PostProcessMode::BLOOM:
                    scene.currentFilter = PostProcessMode::NONE;
                    break;
                default:
                    return;
            }
        }
    }

    void onIdle() override {
        // track time
        static auto time = (float) glfwGetTime();

        // compute time delta
        float dt = (float) glfwGetTime() - time;

        time = (float) glfwGetTime();

        // update and render all objects
        debugCamera->update(dt);
        scene.update(dt);
        scene.render();
    }
};

int main() {
    // initialize window
    SceneWindow window;

    // main loop
    while (window.pollEvents()) {}

    return EXIT_SUCCESS;
}
