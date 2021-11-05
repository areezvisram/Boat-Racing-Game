#ifndef FACE_H
#define FACE_H

#include <vector>
#include <mesh/meshPoint.h>

class Face
{
public:
    Face(std::vector<MeshPoint> meshPoints);
    std::vector<MeshPoint> meshPoints;
};

#endif