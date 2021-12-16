#ifndef PLANE_H
#define PLANE_H

#include <mathLib3D.h>

class Plane
{
public:
    Plane(Point3D pos, Vec3D normal, float width, float height);

    Point3D pos;
    Vec3D normal;
    float width;
    float height;

};

#endif