#include "BilliardWall.h"
/*bool BilliardWall::updateInternal(Scene& scene, float dt) {
    generateModelMatrix();
    return true;
}   // update object

void BilliardWall::renderInternal(Scene& scene) {

}*/

BilliardWall::BilliardWall(const std::string &textureFile, const std::string &modelFile) : StaticObject(textureFile,
                                                                                                        modelFile) {

}
