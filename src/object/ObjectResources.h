//
// Created by nortl on 06/12/2024.
//

#ifndef PPGSOHERNA_OBJECTRESOURCES_H
#define PPGSOHERNA_OBJECTRESOURCES_H
#include <ppgso.h>
#include <glm/glm.hpp>

class ObjectResources {
public:
    bool addTexture(const std::string key);
    bool addMesh(const std::string key);
    ppgso::Texture* getTexture(const std::string code);
    ppgso::Mesh* getMesh(const std::string code);
private:
    std::map< std::string, std::unique_ptr<ppgso::Texture> > textureFiles;
    std::map< std::string, std::unique_ptr<ppgso::Mesh> > meshFiles;
};

static ObjectResources objectResources;

#endif //PPGSOHERNA_OBJECTRESOURCES_H
