#include <object/boundingBox.h>

BoundingBox::BoundingBox()
{
    pos = Point3D();
    size = Vec3D(1,1,1);
    rot = DirectionAngle();
}

BoundingBox::BoundingBox(Point3D pos, Vec3D size, DirectionAngle rot)
{
    this->pos = pos;
    this->size = size;
    this->rot = rot;
}