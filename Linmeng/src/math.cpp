#include "math.h"
#include <cmath>

constexpr float PI = 3.141592654f ;

float math::Todegree(float rad)
{
    return rad/PI * 180.f;
}