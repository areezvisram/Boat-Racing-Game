#ifndef DIRECTION_ANGLE_H
#define DIRECTION_ANGLE_H

#include <string>

class DirectionAngle
{
public:
    DirectionAngle();
    DirectionAngle(float alpha, float beta);
    float alpha;
    float beta;

    DirectionAngle add(DirectionAngle angle);
    DirectionAngle inverse();
    std::string toString();
};

#endif