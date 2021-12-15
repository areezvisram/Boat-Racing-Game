#include <object/boundingBox.h>

BoundingBox::BoundingBox(Point3D pos, Vec3D size, DirectionAngle rot)
{
    this->pos = pos;
    this->size = size;
    this->rot = rot;
}