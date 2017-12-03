#ifndef KARBONATOR_MATH_MATRIX3X3_H
#define KARBONATOR_MATH_MATRIX3X3_H

#include "karbonator/math/Vector3.h"

typedef struct Matrix3x3_ {
    union {
        float elements_ [9];
        Vector3 rowVectors_ [3];
    } m;
} Matrix3x3;

#endif
