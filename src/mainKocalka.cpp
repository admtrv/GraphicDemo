// Example gl_scene
// - Introduces the concept of a dynamic scene of objects
// - Uses abstract object interface for Update and Render steps
// - Creates a simple game scene with Player, Asteroid and Space objects
// - Contains a generator object that does not render but adds Asteroids to the scene
// - Some objects use shared resources and all object deallocations are handled automatically
// - Controls: LEFT, RIGHT, "R" to reset, SPACE to fire

#include <iostream>
#include <map>
#include <list>

#include <ppgso/ppgso.h>

#include "camera.h"
#include "scene.h"
#include "DebugCameraControl.h"

const unsigned int WIDTH = 1024;
const unsigned int HEIGHT = 512;

/*!
 * Custom windows for our simple game
 */
class SceneWindow : public ppgso::Window {
private:
    Scene scene;
    std::unique_ptr<DebugCameraControl> debugCamera;

    /*!
    * Reset and initialize the game scene
    * Creating unique smart pointers to objects that are stored in the scene object list
    */
    void initScene() {
        scene.objects.clear();

        // Create a camera
        auto camera = std::make_unique<Camera>(120.0f, (float)width/(float)height, 0.1f, 100.0f);
        camera->position = glm::vec3(-10,1,0);
        camera->direction = glm::vec3(1,-0.25,0);
        debugCamera = std::make_unique<DebugCameraControl>(camera.get());
        scene.camera = std::move(camera);


            // Add floor
            auto FloorObject = std::make_unique<StaticObject>();
            FloorObject->position = glm::vec3(0,0,0);
            FloorObject->scale = glm::vec3(50,0.5f,50);
            scene.objects.push_back(std::move(FloorObject));
            auto Table = std::make_unique<StaticObject>();
            Table->color = glm::vec3(0.65f,0.16f,0.16f);
            Table->position = glm::vec3(-5,0.5,0);
            Table->scale = glm::vec3(1,0.5,2);
            scene.objects.push_back(std::move(Table));

    }

public:
    /*!
    * Construct custom game window
    */
    SceneWindow() : Window{"gl9_scene", WIDTH, HEIGHT} {

        // Initialize OpenGL state
        // Enable Z-buffer
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        // Enable polygon culling
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);

        initScene();
    }

    /*!
     * Handles pressed key when the window is focused
     * @param key Key code of the key being pressed/released
     * @param scanCode Scan code of the key being pressed/released
     * @param action Action indicating the key state change
     * @param mods Additional modifiers to consider
     */
    void onKey(int key, int scanCode, int action, int mods) override {
        debugCamera->keyboard[key] = action;
    }

    /*!
    * Window update implementation that will be called automatically from pollEvents
    */
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
        debugCamera->Update(dt);
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
