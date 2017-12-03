#ifndef KARBONATOR_MATH_MATRIX2X2_H
#define KARBONATOR_MATH_MATRIX2X2_H

#include "karbonator/math/Vector2.h"

typedef struct Matrix2x2_ {
    union {
        float elements_ [4];
        Vector2 rowVectors_ [2];
    } m;
} Matrix2x2;

#endif
