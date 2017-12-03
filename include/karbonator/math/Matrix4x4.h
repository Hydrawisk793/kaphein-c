#ifndef KARBONATOR_MATH_MATRIX4X4_H
#define KARBONATOR_MATH_MATRIX4X4_H

#include "karbonator/math/Vector4.h"

typedef struct Matrix4x4_ {
    union {
        float elements_ [16];
        Vector4 rowVectors_ [4];
    } m;
} Matrix4x4;

#endif
