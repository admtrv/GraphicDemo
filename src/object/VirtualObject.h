#ifndef PPGSOHERNA_VIRTUALOBJECT_H
#define PPGSOHERNA_VIRTUALOBJECT_H

#include "src/scene/Scene.h"
#include "src/object/Object.h"

class VirtualObject : public Object {
public:
    VirtualObject();
    bool updateInternal(float dt) override;   // update object
    void renderInternal(ppgso::Shader *shader) override;             // render object

};


#endif //PPGSOHERNA_VIRTUALOBJECT_H
