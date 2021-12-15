#ifndef BOAT_H
#define BOAT_H

#include <object/object.h>
#include <mathLib3D.h>
#include <mesh/mesh.h>
#include <camera.h>
#include <directionAngle.h>

class Boat : public Object
{
    public:
        Boat();
        Boat(Point3D position, Mesh mesh, DirectionAngle rotation, float mass, float max_speed, float thrust_force_mag, Camera camera);
        
        //static const float THRUST_FORCE_MAG;
        static const float BREAK_FORCE_MAG;
        static const float FRICTION_FORCE_MAG;
        static const float MIN_SPEED;
        //static const float MAX_SPEED;
        static const float MAX_ANGULAR_SPEED;
        
        bool thrusting;
        bool breaking;
        float forwardMag;
        float mass;
        float max_speed;
        float thrust_force_mag;
        Vec3D dir;
        float speed;
        Vec3D acc;
        DirectionAngle rot;
        Vec3D angularVel;
        Vec3D angularAcc;
        Camera camera;

        void update(bool forward, bool back, bool left, bool right);
        Vec3D forwardVector();
        Vec3D sumForces();
};

#endif