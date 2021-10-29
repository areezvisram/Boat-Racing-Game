#ifndef MESH_H
#define MESH_H

#include <vector>
#include "mathLib3D.h"
#include "face.h"

// struct Face
// {
//     int vertexIndex;
//     int normalIndex;
// };

class Mesh
{
public:
    Mesh();
    std::vector<Point3D> VBO;   //vertex buffer object
    std::vector<Point3D> NBO;   //normal buffer object
    std::vector<Face> faces;    //faces
};

#endif