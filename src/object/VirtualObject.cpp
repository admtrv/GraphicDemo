#include "VirtualObject.h"

VirtualObject::VirtualObject() : Object(){

}

bool VirtualObject::updateInternal(float dt) {
    return true;
}

void VirtualObject::renderInternal(ppgso::Shader *shader) {

}
