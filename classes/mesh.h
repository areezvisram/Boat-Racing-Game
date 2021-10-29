#ifndef MESH_H
#define MESH_H

#include <vector>
#include "mathLib3D.h"
#include "face.h"

using std::vector;

class Mesh
{
public:
    Mesh(vector<Point3D> VBO, vector<Point3D> NBO, vector<Face> faces);
    vector<Point3D> VBO;   //vertex buffer object
    vector<Point3D> NBO;   //normal buffer object
    vector<Face> faces;    //faces
};

#endif