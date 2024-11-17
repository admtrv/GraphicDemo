/*
 * StaticObject.h
 */

#pragma once

#include <memory>
#include <ppgso/ppgso.h>
#include <glm/gtc/random.hpp>
#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

#include "src/scene/Scene.h"
#include "src/object/Object.h"

class StaticObject : public Object {
public:
    explicit StaticObject(const std::string& textureFile = "", const std::string& modelFile = "");

    bool updateInternal(Scene& scene, float dt) override;   // update object
    void renderInternal(Scene& scene) override;             // render object

private:
    // shared resources between instances
    static std::unique_ptr<ppgso::Mesh> defaultMesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> defaultTexture;

    std::unique_ptr<ppgso::Mesh> mesh;       // uniq mesh
    std::unique_ptr<ppgso::Texture> texture; // uniq texture
};