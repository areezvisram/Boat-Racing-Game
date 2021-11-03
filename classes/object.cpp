#include <object.h>

Object::Object()
{
    pos = Point3D();
    rot = Vec3D();
    mesh = Mesh();
}

Object::Object(Point3D pos, Vec3D rot, Mesh mesh)
{
    this->pos = pos;
    this->rot = rot;
    this->mesh = mesh;
}

void Object::draw()
{
    mesh.draw();
}