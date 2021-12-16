#ifndef BOAT_H
#define BOAT_H

#include <object/object.h>
#include <object/boundingBox.h>
#include <mathLib3D.h>
#include <mesh/mesh.h>
#include <camera.h>
#include <directionAngle.h>
#include <plane.h>
#include <map/wall.h>
#include <map/racePlane.h>
#include <force.h>
#include <vector>

class Boat : public Object
{
    public:

        enum BoatType {SPEED, FISHING, PIRATE, SMALLPIRATE};

        Boat();
        Boat(Point3D position, BoatType type, DirectionAngle rotation, Camera camera);
        
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
        BoundingBox boundingBox;
        Force colForce;
        std::vector<int> checkpoints;

        void update(bool forward, bool back, bool left, bool right, std::vector<Wall> walls, std::vector<RacePlane> racePlanes);
        Vec3D forwardVector();
        Vec3D sumForces();
        void drawBoundingBoxes();
        std::vector<Plane> calculatePlanes();
        Force intersects(std::vector<Plane> planes, std::vector<Wall> walls);
        int intersectsCheckpoint(std::vector<Plane> planes, std::vector<RacePlane> walls);
};

#endif