#ifndef PPGSO_STATICOBJECT_H
#define PPGSO_STATICOBJECT_H
#include <memory>

#include <ppgso/ppgso.h>

#include "scene.h"
#include "object.h"

/*!
 * Simple Object that doesn't change
 * This object represents an instance of mesh geometry
 * It initializes and loads all resources only once
 */
class StaticObject final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

public:
    glm::vec3 color;
    /*!
     * Create new Static Object
     */
    StaticObject();

    /*!
     * Update Static Object
     * @param scene Scene to interact with
     * @param dt Time delta for animation purposes
     * @return
     */
    bool update(Scene &scene, float dt) override;

    /*!
     * Render Static Object
     * @param scene Scene to render in
     */
    void render(Scene &scene) override;

private:
};
#endif //PPGSO_STATICOBJECT_H
