#include "Glass.h"

Glass::Glass() : StaticObject("screen.bmp", "cube.obj") {
    scale = glm::vec3(1.5f, 1.5f, 0.01f);
    transparency = 0.3f;
}

void Glass::renderInternal(ppgso::Shader *shader) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    StaticObject::renderInternal(shader);

    glDisable(GL_BLEND);
}
