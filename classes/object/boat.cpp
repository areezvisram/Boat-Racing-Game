#include <object/boat.h>
#include <cmath>

Boat::Boat() : Object(Point3D(), Mesh())
{
    rot = Vec3D();
    camera = Camera();
    dir = Vec3D(1,0,0);
    mass = 1;
}

Boat::Boat(Point3D pos, Mesh mesh, Vec3D rot, float mass, float max_speed, float thrust_force_mag, Camera camera) : Object(pos, mesh)
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

void Boat::update(bool forward, bool back, bool left, bool right)
{       
    thrusting = forward;
    breaking = back;
    float rotInc = 0.8;    
    if (left)
    {
        rot.y += rotInc;
    }
    if (right)
    {
        rot.y -= rotInc;
    }

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
        total = total.add(forwardVector().multiply(thrust_force_mag));
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