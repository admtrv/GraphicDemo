#pragma once

#include "StaticObject.h"

class Glass : public StaticObject {
public:
    Glass();

    void renderInternal(ppgso::Shader *shader) override;
};
