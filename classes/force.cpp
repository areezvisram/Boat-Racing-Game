#include <force.h>

Force::Force()
{
    std::vector<Point3D> a;
    points = a;
    dir = Vec3D();
}

Force::Force(std::vector<Point3D> points, Vec3D dir)
{
    this->points = points;
    this->dir = dir;
}