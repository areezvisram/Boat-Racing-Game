#include <camera.h>

Camera::Camera()
{
    pos = Point3D();
    dir = Vec3D();
    fov = 45;
}

Camera::Camera(Point3D pos, Vec3D dir, float fov)
{
    this->pos = pos;
    this->dir = dir;
    this->fov = fov;
}

Vec3D Camera::directionTo(Point3D lookAt)
{
    return Vec3D::createVector(pos, lookAt).normalize();
}