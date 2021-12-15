#include <directionAngle.h>

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

std::string DirectionAngle::toString()
{
    return "DirectionAngle(" + std::to_string(alpha) + ", " + std::to_string(beta) + ")";
}