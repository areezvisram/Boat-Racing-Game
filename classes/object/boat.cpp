#include <object/boat.h>
#include <cmath>

Boat::Boat() : Object(Point3D(), Mesh())
{
    rot = Vec3D();
    camera = Camera();
    dir = Vec3D(1,0,0);
}

Boat::Boat(Point3D pos, Mesh mesh, Vec3D rot, Camera camera) : Object(pos, mesh)
{
    this->rot = rot;
    this->camera = camera;
    this->dir = Vec3D(1,0,0);
}

void Boat::update(bool forward, bool back, bool left, bool right)
{   
    float rotInc = 1;
    if (forward)
    {
        moveForward(0.2);
    }
    if (back)
    {
        moveForward(-0.2);
    }
    if (left)
    {
        rot.y += rotInc;
    }
    if (right)
    {
        rot.y -= rotInc;
    }
}

Vec3D Boat::sumForces()
{
    Vec3D total = Vec3D();
}

void Boat::moveForward(float distance)
{
    Vec3D straight = Vec3D(distance,0,0);
    Vec3D rotVec = Vec3D(
        straight.x * cosf(-Vec3D::toRadians(rot.y)) - straight.z * sinf(-Vec3D::toRadians(rot.y)),
        0,
        straight.x * sinf(-Vec3D::toRadians(rot.y)) + straight.z * cosf(-Vec3D::toRadians(rot.y))
    );
    pos = rotVec.movePoint(pos);

}