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

#define WINDOW_WIDTH    1024
#define WINDOW_HEIGHT   512

#define ROOM_WIDTH              25.0f
#define ROOM_DEPTH              30.0f
#define ROOM_HEIGHT             10.0f
#define ROOM_WALL_THICKNESS     0.2f


class SceneWindow : public ppgso::Window {
private:
    Scene scene;
    std::unique_ptr<DebugCamera> debugCamera;

    void initScene() {
        if (!scene.objects.empty()) return;

        // Create camera
        auto camera = std::make_unique<Camera>(60.0f, (float)width / (float)height, 0.1f, 100.0f);
        camera->position = glm::vec3(0.0f, 5.0f, 20.0f);
        camera->direction = glm::vec3(0.0f, 0.0f, -1.0f);
        debugCamera = std::make_unique<DebugCamera>(camera.get());
        scene.camera = std::move(camera);

        // Floor
        auto floor = std::make_unique<StaticObject>("floor.bmp");
        floor->position = glm::vec3(0.0f, 0.0f, 0.0f);
        floor->scale = glm::vec3(ROOM_WIDTH, ROOM_WALL_THICKNESS, ROOM_DEPTH);
        scene.objects.push_back(std::move(floor));

        // Ceiling
        auto ceiling = std::make_unique<StaticObject>("ceiling.bmp");
        ceiling->position = glm::vec3(0.0f, ROOM_HEIGHT, 0.0f);
        ceiling->scale = glm::vec3(ROOM_WIDTH, ROOM_WALL_THICKNESS, ROOM_DEPTH);
        scene.objects.push_back(std::move(ceiling));

        // Back wall
        auto backWall = std::make_unique<StaticObject>("wall.bmp");
        backWall->position = glm::vec3(0.0f, ROOM_HEIGHT / 2.0f, -ROOM_DEPTH / 2.0f);
        backWall->scale = glm::vec3(ROOM_WIDTH, ROOM_HEIGHT, ROOM_WALL_THICKNESS);
        scene.objects.push_back(std::move(backWall));

        // Left wall
        auto leftWall = std::make_unique<StaticObject>("wall.bmp");
        leftWall->position = glm::vec3(-ROOM_WIDTH / 2.0f, ROOM_HEIGHT / 2.0f, 0.0f);
        leftWall->scale = glm::vec3(ROOM_DEPTH, ROOM_HEIGHT, ROOM_WALL_THICKNESS);
        leftWall->rotation = glm::vec3(0.0f, 0.0f, glm::radians(90.0f));
        scene.objects.push_back(std::move(leftWall));

        // Right wall
        auto rightWall = std::make_unique<StaticObject>("wall.bmp");
        rightWall->position = glm::vec3(ROOM_WIDTH / 2.0f, ROOM_HEIGHT / 2.0f, 0.0f);
        rightWall->scale = glm::vec3(ROOM_DEPTH, ROOM_HEIGHT, ROOM_WALL_THICKNESS);
        rightWall->rotation = glm::vec3(0.0f, 0.0f, glm::radians(90.0f));
        scene.objects.push_back(std::move(rightWall));

    }

public:
    SceneWindow() : Window{"Playroom", WINDOW_WIDTH, WINDOW_HEIGHT} {
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
