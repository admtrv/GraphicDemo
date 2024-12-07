#include "Glass.h"

Glass::Glass() : StaticObject("glass.bmp", "cube.obj") {
    scale = glm::vec3(1.5f, 1.5f, 0.01f);
    transparency = 0.5f;
}

void Glass::renderInternal(ppgso::Shader *shader) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDepthMask(GL_FALSE);

    StaticObject::renderInternal(shader);

    glDepthMask(GL_TRUE);

    glDisable(GL_BLEND);
}
