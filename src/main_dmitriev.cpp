/*
 * main_dmitriev.cpp
 */

#pragma once

#include <iostream>
#include <list>
#include <ppgso/ppgso.h>

#include "scene/Scene.h"
#include "camera/Camera.h"
#include "camera/DebugCamera.h"
#include "object/Room.h"

#define WINDOW_WIDTH    1024
#define WINDOW_HEIGHT   512

class SceneWindow : public ppgso::Window {
private:
    Scene scene;
    std::unique_ptr<DebugCamera> debugCamera;

    void initScene() {
        if (!scene.objects.empty()) return;

        // create camera
        auto camera = std::make_unique<Camera>(60.0f, (float)width / (float)height, 0.1f, 100.0f);
        camera->position = glm::vec3(0.0f, 5.0f, 20.0f);
        camera->direction = glm::vec3(0.0f, 0.0f, -1.0f);
        debugCamera = std::make_unique<DebugCamera>(camera.get());
        scene.camera = std::move(camera);

        // create room
        auto room = std::make_unique<Room>();
        auto* roomPtr = room.get();
        scene.objects.push_back(std::move(room));

        // create arcade automates
        roomPtr->addArcades();

        // create billiard tables
        roomPtr->addBilliards();
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
    }

    void onIdle() override {
        // track time
        static auto time = (float) glfwGetTime();

        // compute time delta
        float dt = (float) glfwGetTime() - time;

        time = (float) glfwGetTime();

        // set gray background
        glClearColor(.5f, .5f, .5f, 0);
        // clear depth and color buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
