#ifndef MESH_H
#define MESH_H

#include "mathLib3D.h"
#include "vector"

struct Face
{
    int vertexIndex;
    int normalIndex;
};

class Mesh
{
private:
    std::vector<Point3D> VBO;   //vertex buffer object
    std::vector<Point3D> NBO;   //normal buffer object
    std::vector<Face> faces;    //faces
public:
    Mesh();
};

#endif