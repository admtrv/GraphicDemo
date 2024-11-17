/*
 * StaticObject.h
 */

#pragma once

#include <memory>
#include <ppgso/ppgso.h>
#include <glm/gtc/random.hpp>
#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>

#include "src/scene/Scene.h"
#include "src/object/Object.h"

class StaticObject final : public Object {
public:
    glm::vec3 color;

    StaticObject();

    bool updateInternal(Scene& scene, float dt) override;   // update object
    void renderInternal(Scene& scene) override;             // render object

private:
    // shared resources between instances
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

};