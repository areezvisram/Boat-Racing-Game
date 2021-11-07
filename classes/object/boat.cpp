#include <object/boat.h>

Boat::Boat() : Object(Point3D(), Mesh())
{
    rot = Vec3D();
    dir = Vec3D(0,0,1);
    camera = Camera();
}

Boat::Boat(Point3D pos, Mesh mesh, Vec3D rot, Vec3D dir, Camera camera) : Object(pos, mesh)
{
    this->rot = rot;
    this->dir = dir;
    this->camera = camera;
}

void Boat::update()
{
    // Vec3D velocity = dir.multiply(speed);
    // pos = velocity.movePoint(pos);

    // dir = velocity.normalize();
    // speed = velocity.length();
}

void Boat::forward(float distance)
{

}