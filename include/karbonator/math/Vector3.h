#ifndef KARBONATOR_MATH_VECTOR3_H
#define KARBONATOR_MATH_VECTOR3_H

#include "karbonator/pp/basic.h"

KARBONATOR_PP_C_LINKAGE_BEGIN()

typedef struct Vector3_ {
    union {
        float values [3];
        struct {
            float x, y, z;
        } axes;
    } m;
} Vector3;

KARBONATOR_PP_DLL_API
void Vector3_setToZero(
    Vector3* pVector
);

KARBONATOR_PP_DLL_API
void Vector3_fill(
    Vector3* pVector,
    float value
);

KARBONATOR_PP_DLL_API
void Vector3_assign(
    Vector3* pLhs,
    const Vector3* pRhs
);

KARBONATOR_PP_DLL_API
float Vector3_getSquaredMagnitude(
    const Vector3* pVector
);

KARBONATOR_PP_DLL_API
float Vector3_getMagnitude(
    const Vector3* pVector
);

int Vector3_normalize(
    Vector3* pVector,
    float epsilon
);

KARBONATOR_PP_DLL_API
float Vector3_dot(
    const Vector3* pLhs,
    const Vector3* pRhs
);

int Vector3_project(
    Vector3* pOut,
    const Vector3* pVector,
    const Vector3* pAxis,
    float epsilon
);

int Vector3_projectOnPlane(
    Vector3* pOut,
    const Vector3* pVector,
    const Vector3* pNormal,
    float epsilon
);

KARBONATOR_PP_DLL_API
void Vector3_cross(
    Vector3* pOut,
    const Vector3* pLhs,
    const Vector3* pRhs
);

int Vector3_equals(
    const Vector3* pLhs,
    const Vector3* pRhs,
    float epsilon
);

int Vector3_isZero(
    const Vector3* pVector,
    float epsilon
);

KARBONATOR_PP_DLL_API
void Vector3_add(
    Vector3* pLhs,
    const Vector3* pRhs
);

KARBONATOR_PP_DLL_API
void Vector3_subtract(
    Vector3* pLhs,
    const Vector3* pRhs
);

KARBONATOR_PP_DLL_API
void Vector3_scale(
    Vector3* pVector,
    float factor
);

KARBONATOR_PP_C_LINKAGE_END()

#endif
