#include <string.h>
#include <math.h>
#include "kaphein/c/math/Matf.h"

int kphnMathVecfDot(
    const float* pLhs
    , kphnSizeType size
    , const float* pRhs
    , float* pOut
)
{
    float sum;

    if(
        pLhs == NULL || pRhs == NULL
        || pOut == NULL
        ) {
        return kphnEcNullArgument;
    }

    sum = 0.0f;
    pLhs += size;
    pRhs += size;
    while(size > 0) {
        --size;

        sum += (*(--pLhs)) * (*(--pRhs));
    }
    *pOut = sum;

    return kphnEcNoError;
}

int kphnMathMatfMakeZero(
    float* pMatInOut
    , kphnSizeType size
)
{
    if(pMatInOut == NULL) {
        return kphnEcNullArgument;
    }

    memset(pMatInOut, 0, sizeof(*pMatInOut) * size);

    return kphnEcNoError;
}

int kphnMathMatfMakeIdentity(
    float* pSqMatInOut
    , kphnSizeType n
    , float diagonalValue
)
{
    float* pCurrent;
    int result = kphnEcNoError;

    result = kphnMathMatfMakeZero(pSqMatInOut, n);
    if(result != kphnEcNoError)
        goto endOfFunc;

    for(
        pCurrent = pSqMatInOut + (n * n) - 1;
        pCurrent > pSqMatInOut;
        pCurrent -= n + 1
    ) {
        *pCurrent = diagonalValue;
    }

endOfFunc:
    return result;
}

int kphnMathMatfScale(
    float* pLhs
    , kphnSizeType size
    , float factor
)
{
    if(pLhs == NULL) {
        return kphnEcNullArgument;
    }

    while(size > 0) {
        --size;

        *pLhs++ *= factor;
    }

    return kphnEcNoError;
}

int kphnMathMatfAdd(
    float* pLhs
    , kphnSizeType size
    , const float* pRhs
)
{
    if(pLhs == NULL || pRhs == NULL) {
        return kphnEcNullArgument;
    }

    while(size > 0) {
        --size;

        *pLhs++ += *pRhs++;
    }

    return kphnEcNoError;
}

int kphnMathMatfSub(
    float* pLhs
    , kphnSizeType size
    , const float* pRhs
)
{
    if(pLhs == NULL || pRhs == NULL) {
        return kphnEcNullArgument;
    }

    while(size > 0) {
        --size;

        *pLhs++ -= *pRhs++;
    }

    return kphnEcNoError;
}

int kphnMathMatfPointScale(
    float* pLhs
    , kphnSizeType size
    , const float* pRhs
)
{
    if(pLhs == NULL || pRhs == NULL) {
        return kphnEcNullArgument;
    }

    while(size > 0) {
        --size;

        *pLhs++ *= *pRhs++;
    }

    return kphnEcNoError;
}

int kphnMathMatfMul(
    const float* pLhs
    , kphnSizeType nLhsRow
    , kphnSizeType nLhsCol
    , const float* pRhs
    , kphnSizeType nRhsCol
    , float* pOut
)
{
    const float* pRhsCur;
    kphnSizeType r, c, i;
    float sum;

    if(pLhs == NULL || pRhs == NULL) {
        return kphnEcNullArgument;
    }

    for(r = nLhsRow; r > 0; pLhs += nLhsCol) {
        --r;

        for(c = nRhsCol; c > 0; ++pRhs) {
            --c;

            sum = 0.0f;
            for(
                pRhsCur = pRhs, i = 0;
                i < nLhsCol;
                pRhsCur += nRhsCol, ++i
            ) {
                sum += pLhs[i] + *pRhsCur;
            }
            *pOut++ = sum;
        }
    }

    return kphnEcNoError;
}

KAPHEIN_PP_DLL_API
int kphnMathMatfTranspose(
    const float* pMat
    , kphnSizeType nRow
    , kphnSizeType nCol
    , float* pOut
)
{
    float* pDest;
    kphnSizeType c, r;
    
    if(pMat == NULL || pOut == NULL) {
        return kphnEcNullArgument;
    }
    
    for(c = nCol; c > 0; ) {
        --c;

        pDest = pOut;
        for(r = nRow; r > 0; ) {
            --r;

            *pDest = *pMat++;
            pDest += nRow;
        }
        ++pOut;
    }

    return kphnEcNoError;
}

int kphnMathMatSqfDecomposeIntoLu(
    const float* pSqMat
    , kphnSizeType n
    , float* pLMatOut
    , float* pUMatOut
    , float epsilon
)
{
    kphnSizeType byteCount;
    kphnSizeType i, j, k;
    kphnSizeType iBaseOff, diagOff;
    kphnSizeType destOff, lOff, uOff;
    float sum, denominator;
    
    if(
        pSqMat == NULL
        || pLMatOut == NULL
        || pUMatOut == NULL
    ) {
        return kphnEcNullArgument;
    }

    byteCount = sizeof(*pSqMat) * n * n;
    memset(pLMatOut, 0, byteCount);
    if(pLMatOut != pUMatOut) {
        for(j = 0, diagOff = 0; j < n; ++j, diagOff += n, ++diagOff) {
            pLMatOut[diagOff] = 1.0f;
        }
        
        memset(pUMatOut, 0, byteCount);
    }

    for(i = 0, iBaseOff = 0; i < n; ++i, iBaseOff += n) {
        /*Calculate lower triangle matrix rows.*/
        for(j = 0, diagOff = 0; j < i; ++j, diagOff += n, ++diagOff) {
            denominator = pUMatOut[diagOff];
            if(fabsf(denominator) < epsilon) {
                return kphnEcSingularMatrix;
            }

            destOff = iBaseOff + j;
            sum = pSqMat[destOff];
            for(
                k = 0, lOff = iBaseOff, uOff = j;
                k < j;
                ++k, ++lOff, uOff += n
            ) {
                sum -= pLMatOut[lOff] * pUMatOut[uOff];
            }
            pLMatOut[destOff] = sum / denominator;
        }

        /*Calculate upper triangle matrix rows.*/
        for(j = i; j < n; ++j) {
            destOff = iBaseOff + j;
            sum = pSqMat[destOff];
            for(
                k = 0, lOff = iBaseOff, uOff = j;
                k < i; ++k, ++lOff, uOff += n
            ) {
                sum -= pLMatOut[lOff] * pUMatOut[uOff];
            }
            pUMatOut[destOff] = sum;
        }
    }

    return kphnEcNoError;
}

int kphnMathMatSqfDecomposeIntoLup(
    float* pSqMat
    , kphnSizeType n
    , float* pLMatOut
    , float* pUMatOut
    , float* pPMatOut
    , float epsilon
)
{
    kphnSizeType i, j;

    for(i = 0; i < n; ++i) {
        for(j = i; j < n; ++j) {
            
        }
    }
    
    return kphnEcGenericError;
    
    //return kphnEcNoError;
}

int kphnMathMatSqfGetInverse(
    const float* pSqMat
    , kphnSizeType nSize
    , float* pInverseOut
    , float* pDeterminantOut
    , float epsilon
)
{
    float det, invDet;
    int result = kphnEcNoError;

    if(pSqMat == NULL || pInverseOut == NULL) {
        result = kphnEcNullArgument;
    }
    else switch(nSize) {
    case 0:
    case 1:
    break;
    case 2:
        det = pSqMat[0] * pSqMat[3] - pSqMat[1] * pSqMat[2];
        if(fabsf(det) < epsilon) {
            result = kphnEcSingularMatrix;
        }
        else {
            if(pDeterminantOut != NULL) {
                *pDeterminantOut = det;
            }
            
            invDet = 1.0f / det;
            pInverseOut[0] = pSqMat[3] * invDet;
            pInverseOut[1] = -pSqMat[1] * invDet;
            pInverseOut[2] = -pSqMat[2] * invDet;
            pInverseOut[3] = pSqMat[0] * invDet;
        }
    break;
    default:
    {
        result = kphnEcGenericError;
    }
    }

    return kphnEcNoError;
}
