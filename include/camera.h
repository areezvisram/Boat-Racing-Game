#ifndef CAMERA_H
#define CAMERA_H

#include <mathLib3D.h>

class Camera
{
public:
    Camera();
    Camera(Point3D position, Vec3D direction, float fov);
    Point3D pos;
    Vec3D dir;
    float fov;
    Vec3D directionTo(Point3D lookAt);
};

#endif