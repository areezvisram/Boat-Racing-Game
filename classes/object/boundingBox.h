#ifndef BOUNDING_BOX_h
#define BOUNDING_BOX_h

#include <mathLib3D.h>
#include <directionAngle.h>

class BoundingBox
{
public:
    BoundingBox(Point3D pos, Vec3D size, DirectionAngle rot);

    Point3D pos;
    Vec3D size;
    DirectionAngle rot;
};

#endif