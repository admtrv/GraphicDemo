#ifndef PPGSOHERNA_TREE_H
#define PPGSOHERNA_TREE_H

#include "StaticObject.h"
#include "VirtualObject.h"

class Tree : public StaticObject {
public:
    explicit Tree(const std::string& textureFile = "Tree.bmp", const std::string& modelFile = "Tree.obj");
};


#endif //PPGSOHERNA_TREE_H
