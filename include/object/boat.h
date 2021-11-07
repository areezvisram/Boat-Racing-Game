#ifndef BOAT_H
#define BOAT_H

#include <object/object.h>
#include <mathLib3D.h>
#include <mesh/mesh.h>
#include <camera.h>

class Boat : public Object
{
    public:
        Boat();
        Boat(Point3D position, Mesh mesh, Vec3D rotation, Vec3D direction, Camera camera);
        Vec3D rot;
        Vec3D dir;
        Camera camera;

        void update();
        void forward(float distance);
};

#endif