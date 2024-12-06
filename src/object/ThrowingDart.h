#ifndef PPGSOHERNA_THROWINGDART_H
#define PPGSOHERNA_THROWINGDART_H

#include "src/object/Object.h"

class ThrowingDart : public Object{
public:
    explicit ThrowingDart(const std::string& textureFile = "throwingDart.bmp", const std::string& modelFile = "throwingDart.obj");

    glm::vec3 speed;
    glm::vec3 targetCoordinates;
    float targetRadius;
    float height;
    int state;

private:
    bool updateInternal(float dt) override;                  // update object
    void renderInternal(ppgso::Shader *shader) override;     // render object
    std::string meshCode;
    std::string textureCode;
};


#endif //PPGSOHERNA_THROWINGDART_H
