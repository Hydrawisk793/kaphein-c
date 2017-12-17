#include <math.h>
#include "kaphein/c/math/func.h"

float kphnMathRoundf(
    float v
)
{
    const float i = ceilf(v);

    return (v - i >= 0.5f ? i + 1 : i);
}

double kphnMathRoundd(
    double v
)
{
    const double i = ceil(v);

    return (v - i >= 0.5f ? i + 1 : i);
}
