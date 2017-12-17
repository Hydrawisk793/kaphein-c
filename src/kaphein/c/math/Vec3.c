/*
 *  Copyright (c) Hydrawisk793
 *  All rights reserved.
 *
 *  This code is licensed under the MIT License.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files(the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions :
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 */

#include <string.h>
#include <math.h>
#include "kaphein/c/math/Vec3f.h"

void kphnMathVec3fFill(
    struct kphnMathVec3f* pInOut
    , float value
)
{
    pInOut->m.values[0] = value;
    pInOut->m.values[1] = value;
    pInOut->m.values[2] = value;
}

void kphnMathVec3fFillZero(
    struct kphnMathVec3f* pInOut
)
{
    memset(pInOut, 0, sizeof(*pInOut));
}

void kphnMathVec3fAssign(
    struct kphnMathVec3f* pLhs
    , const struct kphnMathVec3f* pRhs
) {
    pLhs->m.values[0] = pRhs->m.values[0];
    pLhs->m.values[1] = pRhs->m.values[1];
    pLhs->m.values[2] = pRhs->m.values[2];
}

float kphnMathVec3fSquaredMagnitude(
    const struct kphnMathVec3f* pIn
)
{
    return kphnMathVec3fDot(pIn, pIn);
}

float kphnMathVec3fMagnitude(
    const struct kphnMathVec3f* pIn
)
{
    return sqrtf(kphnMathVec3fSquaredMagnitude(pIn));
}

int kphnMathVec3fNormalize(
    struct kphnMathVec3f* pInOut
    , float epsilon
) {
    const float sqrMag = kphnMathVec3fSquaredMagnitude(pInOut);

    if(fabsf(sqrMag) < epsilon * epsilon) {
        return krbntrEcInvalidValue;
    }
    else {
        kphnMathVec3fScale(pInOut, 1.0f / sqrtf(sqrMag));

        return krbntrEcNoError;
    }
}

bool kphnMathVec3fIsZero(
    const struct kphnMathVec3f* pIn
    , float epsilon
)
{
    return fabsf(kphnMathVec3fSquaredMagnitude(pIn)) < epsilon * epsilon;
}

void kphnMathVec3fAdd(
    struct kphnMathVec3f* pLhs
    , const struct kphnMathVec3f* pRhs
) {
    pLhs->m.values[0] += pRhs->m.values[0];
    pLhs->m.values[1] += pRhs->m.values[1];
    pLhs->m.values[2] += pRhs->m.values[2];
}

void kphnMathVec3fSubtract(
    struct kphnMathVec3f* pLhs
    , const struct kphnMathVec3f* pRhs
) {
    pLhs->m.values[0] -= pRhs->m.values[0];
    pLhs->m.values[1] -= pRhs->m.values[1];
    pLhs->m.values[2] -= pRhs->m.values[2];
}

void kphnMathVec3fScale(
    struct kphnMathVec3f* pVector
    , float factor
) {
    pVector->m.values[0] *= factor;
    pVector->m.values[1] *= factor;
    pVector->m.values[2] *= factor;
}

float kphnMathVec3fDot(
    const struct kphnMathVec3f* pLhs
    , const struct kphnMathVec3f* pRhs
)
{
    float sum = 0;
    sum += pLhs->m.values[0] * pRhs->m.values[0];
    sum += pLhs->m.values[1] * pRhs->m.values[1];
    sum += pLhs->m.values[2] * pRhs->m.values[2];

    return sum;
}

int kphnMathVec3fProject(
    const struct kphnMathVec3f* pVector
    , const struct kphnMathVec3f* pAxis
    , struct kphnMathVec3f* pOut
    , float epsilon
) {
    const float sqrMag = kphnMathVec3fSquaredMagnitude(pAxis);
    if(fabs(sqrMag) < epsilon) {
        kphnMathVec3fScale(pOut, kphnMathVec3fDot(pVector, pAxis) / sqrtf(sqrMag));

        return krbntrEcNoError;
    }
    else {
        kphnMathVec3fFill(pOut, 0);

        return krbntrEcInvalidValue;
    }
}

int kphnMathVec3fProjectOnPlane(
    const struct kphnMathVec3f* pVector
    , const struct kphnMathVec3f* pNormal
    , struct kphnMathVec3f* pOut
    , float epsilon
)
{
    struct kphnMathVec3f projected;
    int result;
    
    result = kphnMathVec3fProject(pVector, pNormal, &projected, epsilon);
    if(result != krbntrEcNoError) {
        return result;
    }

    kphnMathVec3fAssign(pOut, pVector);
    kphnMathVec3fSubtract(pOut, &projected);

    return result;
}

void kphnMathVec3fCross(
    const struct kphnMathVec3f* pLhs
    , const struct kphnMathVec3f* pRhs
    , struct kphnMathVec3f* pOut
) {
    pOut->m.axes.x = pLhs->m.axes.y * pRhs->m.axes.z - pLhs->m.axes.z * pRhs->m.axes.y;
    pOut->m.axes.y = pLhs->m.axes.z * pRhs->m.axes.x - pLhs->m.axes.x * pRhs->m.axes.z;
    pOut->m.axes.z = pLhs->m.axes.x * pRhs->m.axes.y - pLhs->m.axes.y * pRhs->m.axes.x;
}

bool kphnMathVec3fEquals(
    const struct kphnMathVec3f* pLhs
    , const struct kphnMathVec3f* pRhs
    , float epsilon
)
{
    return (fabsf(pLhs->m.values[0] - pRhs->m.values[0]) < epsilon)
        && (fabsf(pLhs->m.values[1] - pRhs->m.values[1]) < epsilon)
        && (fabsf(pLhs->m.values[2] - pRhs->m.values[2]) < epsilon)
    ;
}
