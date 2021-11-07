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
        Boat(Point3D position, Mesh mesh, Vec3D rotation, Camera camera);
        Vec3D dir;
        float speed;
        Vec3D rot;
        Camera camera;

        void update(bool forward, bool back, bool left, bool right);
        Vec3D sumForces();
        void moveForward(float distance);
};

#endif