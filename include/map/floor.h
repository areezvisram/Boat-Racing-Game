#ifndef FLOOR_H
#define FLOOR_H

#include <mathLib3D.h>
#include <vector>
#include <material.h>

class Floor {
    public:
        Floor();
        Floor(std::vector<Vec3D> vertices);
        Floor(std::vector<Vec3D> vertices, Material material);

        std::vector<Vec3D> vertices;
        Material material;

        void draw();
};

#endif