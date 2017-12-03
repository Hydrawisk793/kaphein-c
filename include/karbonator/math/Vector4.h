#ifndef KARBONATOR_MATH_VECTOR4_H
#define KARBONATOR_MATH_VECTOR4_H

typedef struct Vector4_ {
    union {
        float values [4];
        struct {
            float x, y, z, w;
        } axes;
    } m;
} Vector4;

#endif
