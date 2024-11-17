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

const unsigned int WIDTH = 1024;
const unsigned int HEIGHT = 512;

class SceneWindow : public ppgso::Window {
private:
    Scene scene;
    std::unique_ptr<DebugCamera> debugCamera;

    void initScene() {
        scene.objects.clear();

        // Create a camera
        auto camera = std::make_unique<Camera>(60.0f, (float)width/(float)height, 0.1f, 100.0f);
        camera->position = glm::vec3(-10,1,0);
        camera->direction = glm::vec3(1,-0.25,0);
        debugCamera = std::make_unique<DebugCamera>(camera.get());
        scene.camera = std::move(camera);

        // Add floor
        auto FloorObject = std::make_unique<StaticObject>();
        FloorObject->position = glm::vec3(0,0,0);
        FloorObject->scale = glm::vec3(50,0.5f,50);
        scene.objects.push_back(std::move(FloorObject));

        // Add table
        auto Table = std::make_unique<StaticObject>();
        Table->color = glm::vec3(0.65f,0.16f,0.16f);
        Table->position = glm::vec3(-5,0.5,0);
        Table->scale = glm::vec3(0.2,1,0.2);

        auto TableTop = std::make_unique<StaticObject>();
        TableTop->color = glm::vec3(0.65f,0.16f,0.16f);
        TableTop->position = glm::vec3(0,0.5,0);
        TableTop->scale = glm::vec3(3,0.1,3);
        Table->addChild(std::move(TableTop));

        scene.objects.push_back(std::move(Table));
    }

public:
    SceneWindow() : Window{"gl9_scene", WIDTH, HEIGHT} {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);

        initScene();
    }

    void onKey(int key, int scanCode, int action, int mods) override {
        debugCamera->keyboard[key] = action;
    }

    void onIdle() override {
        // Track time
        static auto time = (float) glfwGetTime();

        // Compute time delta
        float dt = (float) glfwGetTime() - time;

        time = (float) glfwGetTime();

        // Set gray background
        glClearColor(.5f, .5f, .5f, 0);
        // Clear depth and color buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update and render all objects
        debugCamera->update(dt);
        scene.update(dt);
        scene.render();
    }
};

int main() {
    // Initialize our window
    SceneWindow window;

    // Main execution loop
    while (window.pollEvents()) {}

    return EXIT_SUCCESS;
}
