#ifndef WALL_H
#define WALL_H

#include <mathLib3D.h>
#include <vector>
#include <material.h>
#include <plane.h>

class Wall {
    public:
        Wall();
        Wall(std::vector<Point3D> vertices);
        Wall(std::vector<Point3D> vertices, Material material);

        std::vector<Point3D> vertices;
        Material material;
        Vec3D normal;

        void draw();
        float calcWidth();
        float calcHeight();
        Plane toPlane();
};

#endif