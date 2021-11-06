#include <object/boat.h>

Boat::Boat() : Object(Point3D(), Mesh())
{
    rot = Vec3D();
    rotSpeed = Vec3D();
    dir = Vec3D();
    speed = 0.0;
}

Boat::Boat(Point3D pos, Mesh mesh, Vec3D rotSpeed, Vec3D direction, float speed) : Object(pos, mesh)
{
    this->rot = Vec3D(0, -90, 0);
    this->dir = direction;
    this->rotSpeed = rotSpeed;
    this->speed = speed;
}

void Boat::update()
{
    // rot = rot.add(rotSpeed);

    // rot.x = (int)rot.x % 360;
    // rot.y = (int)rot.y % 360;
    // rot.z = (int)rot.z % 360;

    Vec3D velocity = dir.multiply(speed);
    pos = velocity.movePoint(pos);

    dir = velocity.normalize();
    speed = velocity.length();
}