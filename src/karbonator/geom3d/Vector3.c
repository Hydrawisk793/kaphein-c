#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "karbonator/math/Vector3.h"

void Vector3_setToZero(
    Vector3* pVector
) {
    Vector3_fill(pVector, 0);
}

void Vector3_fill(
    Vector3* pVector,
    float value
) {
    pVector->m.values[0] = value;
    pVector->m.values[1] = value;
    pVector->m.values[2] = value;
}

void Vector3_assign(
    Vector3* pLhs,
    const Vector3* pRhs
) {
    pLhs->m.values[0] = pRhs->m.values[0];
    pLhs->m.values[1] = pRhs->m.values[1];
    pLhs->m.values[2] = pRhs->m.values[2];
}

float Vector3_getSquaredMagnitude(
    const Vector3* pVector
) {
    return Vector3_dot(pVector, pVector);
}

float Vector3_getMagnitude(
    const Vector3* pVector
) {
    return sqrtf(Vector3_getSquaredMagnitude(pVector));
}

//int Vector3_normalize(
//    Vector3* pVector,
//    float epsilon
//) {
    //const float sqrMag = Vector3_getSquaredMagnitude(pVector);
    //const int result = 0;//!Vector3_isZero(sqrMag, epsilon * epsilon);
    //if(result) {
    //    Vector3_scale(pVector, 1 / sqrtf(sqrMag));
    //}

    //return result;
//}

float Vector3_dot(
    const Vector3* pLhs,
    const Vector3* pRhs
) {
    float sum = 0;
    sum += pLhs->m.values[0] * pRhs->m.values[0];
    sum += pLhs->m.values[1] * pRhs->m.values[1];
    sum += pLhs->m.values[2] * pRhs->m.values[2];

    return sum;
}

//int Vector3_project(
//    Vector3* pOut,
//    const Vector3* pVector,
//    const Vector3* pAxis,
//    float epsilon
//) {
//    const float sqrMag = Vector3_getSquaredMagnitude(pAxis, epsilon);
//    if(!IsZero<E>()(sqrMag, epsilon * epsilon)) {
//        return (dot(axis) / Sqrt<E>()(sqrMag)) * axis;
//    }
//    else {
//        Vector3_fill(pOut, 0);
//    }
//}
//
//int Vector3_projectOnPlane(
//    Vector3* pOut,
//    const Vector3* pVector,
//    const Vector3* pNormal,
//    float epsilon
//) {
//    return (*this) - Vector3_project(normal, epsilon);
//}

void Vector3_cross(
    Vector3* pOut,
    const Vector3* pLhs,
    const Vector3* pRhs
) {
    pOut->m.axes.x = pLhs->m.axes.y * pRhs->m.axes.z - pLhs->m.axes.z * pRhs->m.axes.y;
    pOut->m.axes.y = pLhs->m.axes.z * pRhs->m.axes.x - pLhs->m.axes.x * pRhs->m.axes.z;
    pOut->m.axes.z = pLhs->m.axes.x * pRhs->m.axes.y - pLhs->m.axes.y * pRhs->m.axes.x;
}

int Vector3_equals(
    const Vector3* pLhs,
    const Vector3* pRhs,
    float epsilon
);

int Vector3_isZero(
    const Vector3* pVector,
    float epsilon
);

void Vector3_add(
    Vector3* pLhs,
    const Vector3* pRhs
) {
    pLhs->m.values[0] += pRhs->m.values[0];
    pLhs->m.values[1] += pRhs->m.values[1];
    pLhs->m.values[2] += pRhs->m.values[2];
}

void Vector3_subtract(
    Vector3* pLhs,
    const Vector3* pRhs
) {
    pLhs->m.values[0] -= pRhs->m.values[0];
    pLhs->m.values[1] -= pRhs->m.values[1];
    pLhs->m.values[2] -= pRhs->m.values[2];
}

void Vector3_scale(
    Vector3* pVector,
    float factor
) {
    pVector->m.values[0] *= factor;
    pVector->m.values[1] *= factor;
    pVector->m.values[2] *= factor;
}
