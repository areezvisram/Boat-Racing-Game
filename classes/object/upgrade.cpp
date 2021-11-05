#include <object/upgrade.h>

Upgrade::Upgrade() : Object(Point3D(), Mesh())
{
    rot = Vec3D();
    rotSpeed = Vec3D();
}

Upgrade::Upgrade(Point3D pos, Mesh mesh, Vec3D rotSpeed) : Object(pos, mesh)
{
    this->rot = Vec3D();
    this-> rotSpeed = rotSpeed;
}

void Upgrade::update()
{
    rot = rot.add(rotSpeed);

    rot.x = (int)rot.x % 360;
    rot.y = (int)rot.y % 360;
    rot.z = (int)rot.z % 360;
}