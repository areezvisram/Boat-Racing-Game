#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>
#include "mathLib3D.h"
#include "face.h"

class Mesh
{
public:
    Mesh(std::vector<Point3D> VBO, std::vector<Point3D> NBO, std::vector<Face> faces);
    std::vector<Point3D> VBO;   //vertex buffer object
    std::vector<Point3D> NBO;   //normal buffer object
    std::vector<Face> faces;    //faces
    std::string name;           //object name usually taken from .obj file

    static Mesh createFromOBJ(std::string filePath);
};

#endif