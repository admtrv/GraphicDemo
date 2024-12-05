#ifndef PPGSOHERNA_PERSCAMERA_H
#define PPGSOHERNA_PERSCAMERA_H

#include "Camera.h"

class PersCamera : public Camera {
public:
    explicit PersCamera(float fov, float ratio, float near, float far) ;
};


#endif //PPGSOHERNA_PERSCAMERA_H
