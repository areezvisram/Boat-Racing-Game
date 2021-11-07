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
        Boat(Point3D pos, Mesh mesh, Vec3D rotSpeed, Vec3D direction, float speed, Camera camera);
        Vec3D rot;
        Vec3D rotSpeed;
        Vec3D dir;
        float speed;
        Camera camera;

        void update();
};

#endif