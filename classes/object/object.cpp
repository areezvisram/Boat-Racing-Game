#include <object/object.h>

Object::Object()
{
    pos = Point3D();
    mesh = Mesh();
}

Object::Object(Point3D pos, Mesh mesh)
{
    this->pos = pos;
    this->mesh = mesh;
}

void Object::draw()
{
    mesh.draw();
}