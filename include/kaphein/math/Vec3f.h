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

#ifndef KAPHEIN_MATH_VEC3_H
#define KAPHEIN_MATH_VEC3_H

#include "kaphein/def.h"

KAPHEIN_PP_C_LINKAGE_BEGIN()

/**
 *  @brief A struct for representing 3-dimensional vector.
 *  @since 2016-06-18
 */
struct kphnMathVec3f
{
    union
    {
        float values[3];

        struct
        {
            float x, y, z;
        } axes;
    } m;
};

/**
 *  @brief Fills all values of a vector to specified value.
 *  @param pInOut A target vector.
 *  @param value A value to be filled.
 *  @since 2016-06-18
 */
KAPHEIN_PP_DLL_API
void kphnMathVec3fFill(
    struct kphnMathVec3f* pInOut
    , float value
);

/**
 *  @brief Fills all values of a vector to zero.
 *  @param pInOut A target vector.
 *  @since 2016-06-18
 */
KAPHEIN_PP_DLL_API
void kphnMathVec3fFillZero(
    struct kphnMathVec3f* pInOut
);

/**
 *  @brief Assigns rhs to lhs.
 *  @param pLhs 
 *  @param pRhs 
 *  @since 2016-06-18
 */
KAPHEIN_PP_DLL_API
void kphnMathVec3fAssign(
    struct kphnMathVec3f* pLhs
    , const struct kphnMathVec3f* pRhs
);

/**
 *  @brief Calculates the squared magnitude of a vector.
 *  @param pIn A target vector.
 *  @since 2016-06-18
 */
KAPHEIN_PP_DLL_API
float kphnMathVec3fSquaredMagnitude(
    const struct kphnMathVec3f* pIn
);

/**
 *  @brief Calculates the magnitude of a vector.
 *  @param pIn A target vector.
 *  @since 2016-06-18
 */
KAPHEIN_PP_DLL_API
float kphnMathVec3fMagnitude(
    const struct kphnMathVec3f* pIn
);

/**
 *  @brief Scale the vector to 1, but preserves the direction.
 *  @param pInOut A target vector to be normalized.
 *  @param epsilon 
 *  @return An error code returned by this function.
 *      <br/> krbntrEcNoError No error has been occured.
 *      <br/> krbntrEcDivisionByZero the maginitude is zero.
 *  @since 2016-06-18
 */
int kphnMathVec3fNormalize(
    struct kphnMathVec3f* pInOut
    , float epsilon
);

/**
 *  @brief Tests if a vector is zero vector.
 *  @param pIn A target vector. 
 *  @return true if the vector is zero vector, false otherwise.
 *  @since 2016-06-18
 */
bool kphnMathVec3fIsZero(
    const struct kphnMathVec3f* pIn
    , float epsilon
);

/**
 *  @brief Adds rhs to lhs.
 *  @param pLhs 
 *  @param pRhs 
 *  @since 2016-06-18
 */
KAPHEIN_PP_DLL_API
void kphnMathVec3fAdd(
    struct kphnMathVec3f* pLhs
    , const struct kphnMathVec3f* pRhs
);

/**
 *  @brief Subtracts rhs from lhs.
 *  @param pLhs 
 *  @param pRhs 
 *  @since 2016-06-18
 */
KAPHEIN_PP_DLL_API
void kphnMathVec3fSubtract(
    struct kphnMathVec3f* pLhs
    , const struct kphnMathVec3f* pRhs
);

/**
 *  @brief Scales a vector by specified factor
 *  @param pVector 
 *  @param factor 
 *  @since 2016-06-18
 */
KAPHEIN_PP_DLL_API
void kphnMathVec3fScale(
    struct kphnMathVec3f* pVector
    , float factor
);

/**
 *  @brief Performs vector dot product.
 *  @param pVector 
 *  @param factor 
 *  @since 2016-06-18
 */
KAPHEIN_PP_DLL_API
float kphnMathVec3fDot(
    const struct kphnMathVec3f* pLhs
    , const struct kphnMathVec3f* pRhs
);

/**
 *  @brief Projects a vector to specified axis.
 *  @param pVector A vector to project.
 *  @param pAxis A target axis.
 *  @since 2016-06-18
 */
KAPHEIN_PP_DLL_API
int kphnMathVec3fProject(
    const struct kphnMathVec3f* pVector
    , const struct kphnMathVec3f* pAxis
    , struct kphnMathVec3f* pOut
    , float epsilon
);

/**
 *  @brief Projects a vector on a plane.
 *  @param pVector A target vector to project.
 *  @param pNormal The normal of a plane.
 *  @param pOut The result of projection.
 *  @param epsilon 
 *  @return An error code returned by this function.
 *  @since 2016-06-18
 */
int kphnMathVec3fProjectOnPlane(
    const struct kphnMathVec3f* pVector
    , const struct kphnMathVec3f* pNormal
    , struct kphnMathVec3f* pOut
    , float epsilon
);

/**
 *  @brief Performs vector cross product.
 *  @param pLhs 
 *  @param pRhs 
 *  @param pOut The result of cross product.
 *  @since 2016-06-18
 */
KAPHEIN_PP_DLL_API
void kphnMathVec3fCross(
    const struct kphnMathVec3f* pLhs
    , const struct kphnMathVec3f* pRhs
    , struct kphnMathVec3f* pOut
);

/**
 *  @brief Tests if two vectors are equal.
 *  @param pLhs 
 *  @param pRhs 
 *  @return true if two vectors are equal, false otherwise.
 *  @since 2016-06-18
 */
bool kphnMathVec3fEquals(
    const struct kphnMathVec3f* pLhs
    , const struct kphnMathVec3f* pRhs
    , float epsilon
);

KAPHEIN_PP_C_LINKAGE_END()

#endif
