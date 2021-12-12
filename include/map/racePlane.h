#ifndef RACE_PLANE_H
#define RACE_PLANE_H

#include <mathLib3D.h>
#include <vector>
#include <material.h>

class RacePlane {
    public:
        RacePlane();
        RacePlane(std::vector<Point3D> vertices);
        RacePlane(std::vector<Point3D> vertices, Material material);

        std::vector<Point3D> vertices;
        Material material;

        void draw();
        void loadTextures();
};

#endif