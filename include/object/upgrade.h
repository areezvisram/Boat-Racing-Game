#ifndef UPGRADE_H
#define UPGRADE_H

#include <object/object.h>
#include <mathLib3D.h>
#include <mesh/mesh.h>

class Upgrade : public Object
{
public:
    Upgrade();
    Upgrade(Point3D pos, Mesh mesh, Vec3D rotSpeed);
    Vec3D rot;
    Vec3D rotSpeed;

    void update();
};

#endif