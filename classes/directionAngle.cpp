#include <directionAngle.h>
#include <mathLib3D.h>

DirectionAngle::DirectionAngle()
{
    alpha = 0;
    beta = 0;
}

DirectionAngle::DirectionAngle(float alpha, float beta)
{
    this->alpha = alpha;
    this->beta = beta;
}

DirectionAngle DirectionAngle::add(DirectionAngle angle)
{
    return DirectionAngle(alpha + angle.alpha, beta + angle.beta);
}

DirectionAngle DirectionAngle::inverse()
{
    return Vec3D::createVector(DirectionAngle(alpha, beta), 1).multiply(-1).calcRotation();
}

std::string DirectionAngle::toString()
{
    return "DirectionAngle(" + std::to_string(alpha) + ", " + std::to_string(beta) + ")";
}