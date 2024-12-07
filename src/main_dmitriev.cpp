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
#include "object/Dartboard.h"

#define WINDOW_WIDTH    1024
#define WINDOW_HEIGHT   512

class SceneWindow : public ppgso::Window {
private:
    Scene scene;
    std::unique_ptr<DebugCamera> debugCamera;
    Dartboard* activeDartboard;

    void initScene() {
        if (!scene.objects.empty()) return;
        scene.windowWidth = WINDOW_WIDTH;
        scene.windowHeight = WINDOW_HEIGHT;
        // create camera
        auto camera = std::make_unique<PersCamera>(60.0f, (float)width / (float)height, 0.1f, 100.0f);
        camera->position = glm::vec3(0.0f, 5.0f, 20.0f);
        camera->direction = glm::vec3(0.0f, 0.0f, -1.0f);
        debugCamera = std::make_unique<DebugCamera>(camera.get());
        scene.camera = std::move(camera);

        // create directional Light
        auto directionalLight = std::make_unique<OrthoCamera>(-20.0f, 20.0f, -10.0f, 20.0f, 0.1f, 50.0f);
        directionalLight->position = glm::vec3(15.0f, 5.0f, -2.0f);
        directionalLight->direction = glm::normalize(-directionalLight->position);
        scene.directionalLight = std::move(directionalLight);

        // create room
        auto room = std::make_unique<Room>(scene);
        auto* roomPtr = room.get();
        scene.objects.push_back(std::move(room));

        // create arcade automates
        roomPtr->addArcades(scene);

        // create billiard tables
        roomPtr->addBilliards();

        // create chandeliers above billiard tables
        roomPtr->addChandeliers(scene);

        // create dartboards
        activeDartboard = roomPtr->addDartboards();
    }


public:
    SceneWindow() : Window{"Playroom", WINDOW_WIDTH, WINDOW_HEIGHT} {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        glDisable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);

        initScene();
    }

    void onKey(int key, int scanCode, int action, int mods) override {
        debugCamera->keyboard[key] = action;
        if(key == GLFW_KEY_X && action){
            activeDartboard->throwDart();
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
