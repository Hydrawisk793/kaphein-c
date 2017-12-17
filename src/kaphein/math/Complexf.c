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

#include <stdlib.h>
#include <math.h>
#include "kaphein/mem/func.h"
#include "kaphein/math/Complexf.h"

static int ComplexfCopyAssign(
    void* pLhs
    , const void* pRhs
)
{
    struct kphnMathComplexf* pL = (struct kphnMathComplexf*)pLhs;
    const struct kphnMathComplexf* pR = (const struct kphnMathComplexf*)pRhs;

    pL->imaginary = pR->imaginary;
    pL->real = pR->real;

    return krbntrEcNoError;
}

int kphnMathComplexfFromReal(
    const float* pRealValues
    , size_t realCount
    , struct kphnMathComplexf* pOut
    , size_t outSize
)
{
    const float* pRealValuesEnd;

    if(pRealValues == NULL || pOut == NULL) {
        return krbntrEcNullArgument;
    }

    if(outSize < realCount) {
        return krbntrEcNotEnoughBufferSize;
    }

    for(
        pRealValuesEnd = pRealValues + realCount;
        pRealValues < pRealValuesEnd;
        ++pRealValues, ++pOut
    ) {
        pOut->imaginary = 0.0f;
        pOut->real = *pRealValues;
    }

    return krbntrEcNoError;
}

int kphnMathComplexfFromImaginary(
    const float* pImaginaryValues
    , size_t imaginaryCount
    , struct kphnMathComplexf* pOut
    , size_t outSize
)
{
    const float* pImaginaryValuesEnd;

    if(pImaginaryValues == NULL || pOut == NULL) {
        return krbntrEcNullArgument;
    }

    if(outSize < imaginaryCount) {
        return krbntrEcNotEnoughBufferSize;
    }

    for(
        pImaginaryValuesEnd = pImaginaryValues + imaginaryCount;
        pImaginaryValues < pImaginaryValuesEnd;
        ++pImaginaryValues, ++pOut
    ) {
        pOut->imaginary = *pImaginaryValues;
        pOut->real = 0.0f;
    }

    return krbntrEcNoError;
}

int kphnMathComplexfConjugate(
    const struct kphnMathComplexf* pIn
    , struct kphnMathComplexf* pOut
)
{
    if(pIn == NULL || pOut == NULL) {
        return krbntrEcNullArgument;
    }
    
    pOut->imaginary = -pIn->imaginary;
    pOut->real = pIn->real;

    return krbntrEcNoError;
}

int kphnMathComplexfAdd(
    struct kphnMathComplexf* pLhs
    , const struct kphnMathComplexf* pRhs
)
{
    if(pLhs == NULL || pRhs == NULL) {
        return krbntrEcNullArgument;
    }
    
    pLhs->imaginary += pRhs->imaginary;
    pLhs->real += pRhs->real;

    return krbntrEcNoError;
}

int kphnMathComplexfSubtract(
    struct kphnMathComplexf* pLhs
    , const struct kphnMathComplexf* pRhs
)
{
    if(pLhs == NULL || pRhs == NULL) {
        return krbntrEcNullArgument;
    }
    
    pLhs->imaginary -= pRhs->imaginary;
    pLhs->real -= pRhs->real;

    return krbntrEcNoError;
}

int kphnMathComplexfMultiply(
    const struct kphnMathComplexf* pLhs
    , const struct kphnMathComplexf* pRhs
    , struct kphnMathComplexf* pOut
)
{
    float a, b, c, d;

    if(pLhs == NULL || pRhs == NULL || pOut == NULL) {
        return krbntrEcNullArgument;
    }

    a = pLhs->real;
    b = pLhs->imaginary;
    c = pRhs->real;
    d = pRhs->imaginary;
    
    pOut->imaginary = a * d + b * c;
    pOut->real = a * c - b * d;
    
    return krbntrEcNoError;
}

int kphnMathComplexfScale(
    struct kphnMathComplexf* pInOut
    , float factor
)
{
    if(pInOut == NULL) {
        return krbntrEcNullArgument;
    }

    pInOut->imaginary *= factor;
    pInOut->real *= factor;

    return krbntrEcNoError;
}

bool kphnMathComplexfEquals(
    const struct kphnMathComplexf* pLhs
    , const struct kphnMathComplexf* pRhs
    , float epsilon
)
{
    return (fabsf(pLhs->imaginary - pRhs->imaginary) < epsilon)
        && (fabsf(pLhs->real - pRhs->real) < epsilon)
    ;
}

int kphnMathComplexfToPolar(
    struct kphnMathComplexf* pInOut
    , size_t count
)
{
    struct kphnMathComplexf* pEnd;
    float imaginary;
    float real;

    if(pInOut == NULL) {
        return krbntrEcNullArgument;
    }
    
    for(pEnd = pInOut + count; pInOut < pEnd; ++pInOut) {
        imaginary = pInOut->imaginary;
        real = pInOut->real;

        /*TODO : Test is it right.*/
        pInOut->imaginary = atan2f(imaginary, real);
        pInOut->real = sqrtf(real * real + imaginary * imaginary);
    }

    return krbntrEcNoError;
}

int kphnMathComplexfDft(
    const struct kphnMathComplexf* pSignalsIn
    , size_t signalCount
    , struct kphnMathComplexf* pOut
    , size_t outSize
    , bool inverse
)
{
    size_t x, u, xTimesU;
    struct kphnMathComplexf* pSum;
    struct kphnMathComplexf complExp;
    struct kphnMathComplexf mulResult;
    float angle;
    float twoPiFreq;
    float signFactor;
    float scaleFactor;

    if(signalCount < 1) {
        return krbntrEcNoError;
    }

    if(pSignalsIn == NULL || pOut == NULL) {
        return krbntrEcNullArgument;
    }

    if(outSize < signalCount) {
        return krbntrEcNotEnoughBufferSize;
    }

    if(inverse) {
        signFactor = 1.0f;
        scaleFactor = 1.0f / signalCount;
    }
    else {
        signFactor = -1.0f;
        scaleFactor = 1.0f;
    }
    
    twoPiFreq = (2.0f * 3.14159265358979323846f) / signalCount;
    pSum = pOut;
    for(x = 0; x < signalCount; ++x, ++pSum) {
        pSum->real = 0;
        pSum->imaginary = 0;

        for(u = 0, xTimesU = 0; u < signalCount; ++u, xTimesU += x) {
            angle = xTimesU * twoPiFreq;

            complExp.real = cosf(angle);
            complExp.imaginary = sinf(angle) * signFactor;

            kphnMathComplexfMultiply(
                pSignalsIn + u
                , &complExp
                , &mulResult
            );

            pSum->real += mulResult.real;
            pSum->imaginary += mulResult.imaginary;
        }

        pSum->real *= scaleFactor;
        pSum->imaginary *= scaleFactor;
    }

    return krbntrEcNoError;
}

int kphnMathComplexfDft2D(
    const struct kphnMathComplexf* pMatrixIn
    , size_t inRowCount
    , size_t inColumnCount
    , struct kphnMathComplexf* pMatrixOut
    , size_t outSize
    , bool inverse
)
{
    struct kphnMathComplexf* temp1;
    size_t inSize;
    size_t inY;
    int result;

    if(pMatrixIn == NULL || pMatrixOut == NULL) {
        return krbntrEcNullArgument;
    }

    inSize = inRowCount * inColumnCount;
    if(outSize < inSize) {
        return krbntrEcNotEnoughBufferSize;
    }

    temp1 = (struct kphnMathComplexf*)malloc(sizeof(*temp1) * inSize);
    
    for(inY = 0; inY < inRowCount; ++inY) {
        result = kphnMathComplexfDft(
            pMatrixIn + inY * inColumnCount,
            inColumnCount,
            temp1 + inY * inColumnCount,
            inColumnCount,
            inverse
        );
        if(result != krbntrEcNoError) {
            free(temp1);
            
            return result;
        }
    }
    
    result = kphnMemTranspose2D(
        temp1
        , inRowCount
        , inColumnCount
        , pMatrixOut
        , inSize
        , ComplexfCopyAssign
        , sizeof(struct kphnMathComplexf)
    );
    if(result != krbntrEcNoError) {
        free(temp1);

        return result;
    }

    for(inY = 0; inY < inRowCount; ++inY) {
        result = kphnMathComplexfDft(
            pMatrixOut + inY * inColumnCount,
            inColumnCount,
            temp1 + inY * inColumnCount,
            inColumnCount,
            inverse
        );
        if(result != krbntrEcNoError) {
            free(temp1);

            return result;
        }
    }

    result = kphnMemTranspose2D(
        temp1
        , inRowCount
        , inColumnCount
        , pMatrixOut
        , inSize
        , ComplexfCopyAssign
        , sizeof(struct kphnMathComplexf)
    );
    if(result != krbntrEcNoError) {
        free(temp1);

        return result;
    }

    free(temp1);

    return krbntrEcNoError;
}
