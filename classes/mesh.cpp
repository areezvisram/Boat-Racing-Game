#include "mesh.h"

Mesh::Mesh(vector<Point3D> VBO, vector<Point3D> NBO, vector<Face> faces)
{
    this->VBO = VBO;
    this->NBO = NBO;
    this->faces = faces;
}