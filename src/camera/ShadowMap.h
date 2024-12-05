#ifndef PPGSOHERNA_SHADOWMAP_H
#define PPGSOHERNA_SHADOWMAP_H


#include <ppgso/ppgso.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

class ShadowMap {
public:
    explicit ShadowMap();
//private:
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 512;
    unsigned int depthMapFBO;
    unsigned int depthMap;

    //int BufferID;
    //glm::vec3 LightCoords;
    //glm::vec3 LightDir;

};


#endif //PPGSOHERNA_SHADOWMAP_H
