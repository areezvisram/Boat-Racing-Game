#include <object/boat.h>
#include <cmath>
#include <iostream>

Boat::Boat() : Object(Point3D(), Mesh())
{
    rot = DirectionAngle();
    camera = Camera();
    dir = Vec3D(1,0,0);
    mass = 1;
}

Boat::Boat(Point3D pos, Mesh mesh, DirectionAngle rot, float mass, float max_speed, float thrust_force_mag, Camera camera) : Object(pos, mesh)
{
    this->rot = rot;    
    this->mass = mass;
    this->max_speed = max_speed;
    this->thrust_force_mag = thrust_force_mag;
    this->camera = camera;
    this->dir = Vec3D(1,0,0);
}

//const float Boat::THRUST_FORCE_MAG = 0.7;
const float Boat::BREAK_FORCE_MAG = 0.5;
const float Boat::FRICTION_FORCE_MAG = 0.1;
// const float Boat::MIN_SPEED = 0.0001;
//const float Boat::MAX_SPEED = 0.3;
const float Boat::MAX_ANGULAR_SPEED = 1.0;

void Boat::update(bool forward, bool back, bool left, bool right)
{       
    thrusting = forward;
    breaking = back;
    // float rotInc = 0.8;    
    angularAcc = Vec3D();
    // angularVel = Vec3D();
    if (left)
    {
        // rot.y += rotInc;
        angularAcc = Vec3D(0,-0.25,0);
    }
    if (right)
    {
        // rot.y -= rotInc;
        angularAcc = Vec3D(0,0.25,0);
    }
    angularVel = angularVel.add(angularAcc);
    if (std::abs(angularVel.y) > MAX_ANGULAR_SPEED)
    {
        angularVel.y = (angularVel.y / std::abs(angularVel.y)) * MAX_ANGULAR_SPEED;
    }
    //angular friction
    if (std::abs(angularVel.y) > 0.025)
    {
        if (angularVel.y > 0)
        {
            angularVel = angularVel.add(Vec3D(0,-0.025,0));
        }
        else if (angularVel.y < 0)
        {
            angularVel = angularVel.add(Vec3D(0,0.025,0));
        }
    }
    else
    {
        angularVel.y = 0;
    }
    // std::cout << angularAcc.toString() << std::endl;
    rot = rot.add(DirectionAngle(angularVel.y, 0));

    // std::cout << rot.toString() << std::endl;

    Vec3D force = sumForces();
    acc = force.multiply(1.0 / mass);
    Vec3D vel = dir.multiply(speed);
    vel = vel.add(acc);
    speed = vel.length();
    if (speed >= max_speed)
    {
        speed = max_speed;
    }
    dir = vel.normalize();
    pos = vel.movePoint(pos);
}

Vec3D Boat::forwardVector()
{
    Vec3D straight = Vec3D(1,0,0);
    Vec3D rotVec = Vec3D(
        straight.x * cosf(-Vec3D::toRadians(rot.alpha)) - straight.z * sinf(-Vec3D::toRadians(rot.alpha)),
        0,
        -(straight.x * sinf(-Vec3D::toRadians(rot.alpha)) + straight.z * cosf(-Vec3D::toRadians(rot.alpha)))
    );
    return rotVec;
}

Vec3D Boat::sumForces()
{
    Vec3D total = Vec3D();

    if (thrusting)
    {
        total = total.add(forwardVector().multiply(thrust_force_mag));
    }

    if (breaking)
    {
        total = total.add(forwardVector().multiply(-1 * BREAK_FORCE_MAG));
    }

    total = total.add(dir.multiply(-1 * FRICTION_FORCE_MAG));

    return total;
}