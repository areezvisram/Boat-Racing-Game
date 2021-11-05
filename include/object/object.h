#ifndef OBJECT_H
#define OBJECT_H

#include <mathLib3D.h>
#include <mesh/mesh.h>

class Object
{
public:
    Object();
    Object(Point3D position, Mesh mesh);
    Point3D pos;
    Mesh mesh;
    void draw();
};

#endif