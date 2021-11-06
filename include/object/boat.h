#ifndef BOAT_H
#define BOAT_H

#include <object/object.h>
#include <mathLib3D.h>
#include <mesh/mesh.h>

class Boat : public Object
{
    public:
        Boat();
        Boat(Point3D pos, Mesh mesh, Vec3D rotSpeed, Vec3D direction, float speed);
        Vec3D rot;
        Vec3D rotSpeed;
        Vec3D dir;
        float speed;

        void update();
};

#endif