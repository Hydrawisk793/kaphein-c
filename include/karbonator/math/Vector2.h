#ifndef KARBONATOR_MATH_VECTOR2_H
#define KARBONATOR_MATH_VECTOR2_H

typedef struct Vector2_ {
    union {
        float values [2];
        struct {
            float x, y;
        } axes;
    } m;
} Vector2;

#endif
