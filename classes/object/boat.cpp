#include <object/boat.h>
#include <cmath>

Boat::Boat() : Object(Point3D(), Mesh())
{
    rot = Vec3D();
    camera = Camera();
    dir = Vec3D(1,0,0);
    mass = 1;
}

Boat::Boat(Point3D pos, Mesh mesh, Vec3D rot, float mass, Camera camera) : Object(pos, mesh)
{
    this->rot = rot;
    this->mass = mass;
    this->camera = camera;
    this->dir = Vec3D(1,0,0);
}

const float Boat::THRUST_FORCE_MAG = 0.7;
const float Boat::BREAK_FORCE_MAG = 0.5;
const float Boat::FRICTION_FORCE_MAG = 0.1;
// const float Boat::MIN_SPEED = 0.0001;
const float Boat::MAX_SPEED = 0.1;

void Boat::update(bool forward, bool back, bool left, bool right)
{   
    // forwardMag = 0;
    thrusting = forward;
    breaking = back;
    float rotInc = 0.8;
    // if (forward)
    // {
    //     // moveForward(0.2);
    // }
    // if (back)
    // {
    //     // moveForward(-0.2);
    // }
    if (left)
    {
        rot.y += rotInc;
    }
    if (right)
    {
        rot.y -= rotInc;
    }

    rot = rot.add(angularVel);

    Vec3D force = sumForces();
    acc = force.multiply(1.0 / mass);
    Vec3D vel = dir.multiply(speed);
    vel = vel.add(acc);
    speed = vel.length();
    // if (speed <= MIN_SPEED)
    // {
    //     speed = 0.0;
    // }
    if (speed >= MAX_SPEED)
    {
        speed = MAX_SPEED;
    }
    dir = vel.normalize();
    pos = vel.movePoint(pos);
}

Vec3D Boat::forwardVector()
{
    Vec3D straight = Vec3D(1,0,0);
    Vec3D rotVec = Vec3D(
        straight.x * cosf(-Vec3D::toRadians(rot.y)) - straight.z * sinf(-Vec3D::toRadians(rot.y)),
        0,
        straight.x * sinf(-Vec3D::toRadians(rot.y)) + straight.z * cosf(-Vec3D::toRadians(rot.y))
    );
    return rotVec;
}

Vec3D Boat::sumForces()
{
    Vec3D total = Vec3D();

    if (thrusting)
    {
        total = total.add(forwardVector().multiply(THRUST_FORCE_MAG));
    }

    if (breaking)
    {
        total = total.add(forwardVector().multiply(-1 * BREAK_FORCE_MAG));
    }

    total = total.add(dir.multiply(-1 * FRICTION_FORCE_MAG));

    return total;
}

void Boat::moveForward(float distance)
{
    Vec3D straight = Vec3D(distance,0,0);
    Vec3D rotVec = Vec3D(
        straight.x * cosf(-Vec3D::toRadians(rot.y)) - straight.z * sinf(-Vec3D::toRadians(rot.y)),
        0,
        straight.x * sinf(-Vec3D::toRadians(rot.y)) + straight.z * cosf(-Vec3D::toRadians(rot.y))
    );
    pos = rotVec.movePoint(pos);

}