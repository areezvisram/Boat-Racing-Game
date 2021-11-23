#ifndef FLOOR_H
#define FLOOR_H

#include <mathLib3D.h>
#include <vector>

class Floor {
    public:
        Floor();
        Floor(std::vector<Vec3D> vertices);

        std::vector<Vec3D> vertices;

        void draw();
};

#endif