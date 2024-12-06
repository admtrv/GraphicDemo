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

    void setTextureSize(glm::vec2 textureSize);
    void setTextureOffset(glm::vec2 textureOffset);

    bool updateInternal(float dt) override;   // update object
    void renderInternal(ppgso::Shader *shader) override;             // render object

    float transparency{1.0f};

private:
    std::string meshCode;
    std::string textureCode;
};