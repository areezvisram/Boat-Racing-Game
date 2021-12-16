#ifndef FORCE_H
#define FORCE_H

#include <vector>
#include <mathLib3D.h>

class Force
{
public:
    Force();
    Force(std::vector<Point3D> points, Vec3D dir);

    std::vector<Point3D> points;
    Vec3D dir;
};

#endif