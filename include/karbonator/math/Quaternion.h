#ifndef KARBONATOR_MATH_QUATERNION_H
#define KARBONATOR_MATH_QUATERNION_H

#include "karbonator/math/Vector3.h"

typedef struct Quaternion_ {
    union {
        float values [4];
        struct {
            float x, y, z, w;
        } axes;
        struct {
            Vector3 imaginary;
            float real;
        } pair;
    } m;
} Quaternion;

#endif
