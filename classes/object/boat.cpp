#include <object/boat.h>

Boat::Boat() : Object(Point3D(), Mesh())
{
    rot = Vec3D();
    rotSpeed = Vec3D();
    dir = Vec3D();
    speed = 0.0;
    camera = Camera();
}

Boat::Boat(Point3D pos, Mesh mesh, Vec3D rotSpeed, Vec3D direction, float speed, Camera camera) : Object(pos, mesh)
{
    this->rot = Vec3D(0, -90, 0);
    this->dir = direction;
    this->rotSpeed = rotSpeed;
    this->speed = speed;
    this->camera = camera;
}

void Boat::update()
{
    Vec3D velocity = dir.multiply(speed);
    pos = velocity.movePoint(pos);

    dir = velocity.normalize();
    speed = velocity.length();
}