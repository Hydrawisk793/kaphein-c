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

#ifndef KAPHEIN_MATH_COMPLEXF_H
#define KAPHEIN_MATH_COMPLEXF_H

#include "kaphein/def.h"

KAPHEIN_PP_C_LINKAGE_BEGIN()

/**
 *  @brief A struct for representing complex numbers.
 *  @since 2012-10-01
 */
struct kphnMathComplexf
{
    /**
     *  @breif the imaginary part factor.
     */
    float imaginary;

    /**
     *  @breif the real part factor.
     */
    float real;
};

/**
 *  @brief Initializes an array of compelx numbers by real values.
 *  @param pRealValues 
 *  @param realCount 
 *  @param pOut 
 *  @param outSize
 *  @return An error code returned by this function.
 *  @since 2012-10-01
 */
KAPHEIN_PP_DLL_API
int kphnMathComplexfFromReal(
    const float* pRealValues
    , size_t realCount
    , struct kphnMathComplexf* pOut
    , size_t outSize
);

/**
 *  @brief Initializes an array of compelx numbers by imaginary values.
 *  @param pImaginaryValues  
 *  @param imaginaryCount 
 *  @param pOut 
 *  @param outSize
 *  @return An error code returned by this function.
 *  @since 2012-10-01
 */
KAPHEIN_PP_DLL_API
int kphnMathComplexfFromImaginary(
    const float* pImaginaryValues
    , size_t imaginaryCount
    , struct kphnMathComplexf* pOut
    , size_t outSize
);

/**
 *  @brief Retrieves the conjugate of a complex number.
 *  @param pIn 
 *  @param pOut 
 *  @return An error code returned by this function.
 *  @since 2012-10-01
 */
KAPHEIN_PP_DLL_API
int kphnMathComplexfConjugate(
    const struct kphnMathComplexf* pIn
    , struct kphnMathComplexf* pOut
);

/**
 *  @brief Adds rhs to lhs.
 *  @param pLhs 
 *  @param pRhs 
 *  @return An error code returned by this function.
 *  @since 2012-10-01
 */
KAPHEIN_PP_DLL_API
int kphnMathComplexfAdd(
    struct kphnMathComplexf* pLhs
    , const struct kphnMathComplexf* pRhs
);

/**
 *  @brief Subtracts rhs from lhs.
 *  @param pLhs 
 *  @param pRhs 
 *  @return An error code returned by this function.
 *  @since 2012-10-01
 */
KAPHEIN_PP_DLL_API
int kphnMathComplexfSubtract(
    struct kphnMathComplexf* pLhs
    , const struct kphnMathComplexf* pRhs
);

/**
 *  @brief Multiplies lhs by rhs.
 *  @param pLhs 
 *  @param pRhs 
 *  @return An error code returned by this function.
 *  @since 2012-10-01
 */
KAPHEIN_PP_DLL_API
int kphnMathComplexfMultiply(
    const struct kphnMathComplexf* pLhs
    , const struct kphnMathComplexf* pRhs
    , struct kphnMathComplexf* pOut
);

/**
 *  @brief Divides lhs by rhs.
 *  @param pLhs 
 *  @param pRhs 
 *  @return An error code returned by this function.
 *  @since 2012-10-01
 */
int kphnMathComplexfDivide(
    const struct kphnMathComplexf* pLhs
    , const struct kphnMathComplexf* pRhs
    , struct  kphnMathComplexf* pOut
);

/**
 *  @brief Scales a complex number by specified factor.
 *  @param pInOut 
 *  @param factor 
 *  @return An error code returned by this function.
 *  @since 2012-10-01
 */
KAPHEIN_PP_DLL_API
int kphnMathComplexfScale(
    struct kphnMathComplexf* pInOut
    , float factor
);

/**
 *  @brief Tests if tow complex numbers are equal.
 *  @param pLhs 
 *  @param pRhs 
 *  @return true if two complex numbers are equal, false otherwise.
 *  @since 2012-10-01
 */
KAPHEIN_PP_DLL_API
bool kphnMathComplexfEquals(
    const struct kphnMathComplexf* pLhs
    , const struct kphnMathComplexf* pRhs
    , float epsilon
);

/**
 *  @brief 
 *  @param pInOut A pointer to the first complex number.
 *  @param count The number of complex number.
 *  @return An error code returned by this function.
 *  @since 2017-12-01
 */
KAPHEIN_PP_DLL_API
int kphnMathComplexfToPolar(
    struct kphnMathComplexf* pInOut
    , size_t count
);

/**
 *  @brief Performs Discrete Fourier Transform on a signal array.
 *  @param pSignalsIn A pointer to the first input signal.
 *  @param signalCount The number of input signals.
 *  @param pOut A pointer to the output array.
 *  @param outSize The size of the output array.
 *  @param inverse true if inverse transform, false otherwise.
 *  @return An error code returned by the function.
 *      <br/>
 *      <br/>
 *  @since 2017-12-01
 */
KAPHEIN_PP_DLL_API
int kphnMathComplexfDft(
    const struct kphnMathComplexf* pSignalsIn
    , size_t signalCount
    , struct kphnMathComplexf* pOut
    , size_t outSize
    , bool inverse
);

/**
 *  @brief Performs Discrete Fourier Transform on a signal matrix.
 *  @param pIn
 *  @param rowCount
 *  @param columnCount
 *  @param pOut A pointer to the output array.
 *  @param outSize The size of the output array.
 *  @param inverse true if inverse transform, false otherwise.
 *  @return An error code returned by the function.
 *      <br/>
 *      <br/>
 *  @since 2017-12-01
 */
KAPHEIN_PP_DLL_API
int kphnMathComplexfDft2D(
    const struct kphnMathComplexf* pIn
    , size_t rowCount
    , size_t columnCount
    , struct kphnMathComplexf* pOut
    , size_t outSize
    , bool inverse
);

KAPHEIN_PP_C_LINKAGE_END()

#endif
