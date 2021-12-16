#include <plane.h>

Plane::Plane(Point3D pos, Vec3D normal, float width, float height)
{
    this->pos = pos;
    this->normal = normal;
    this->width = width;
    this->height = height;
}