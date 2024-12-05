#ifndef PPGSOHERNA_ORTHOCAMERA_H
#define PPGSOHERNA_ORTHOCAMERA_H

#include "Camera.h"

class OrthoCamera : public Camera {
public:
    explicit OrthoCamera(float left, float right, float bottom, float top, float near, float far) ;
};


#endif //PPGSOHERNA_ORTHOCAMERA_H
