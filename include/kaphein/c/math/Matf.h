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

#ifndef KAPHEIN_C_MATH_MATF_H
#define KAPHEIN_C_MATH_MATF_H

#include "../def.h"
#include "../mem/fwd.h"

KAPHEIN_PP_C_LINKAGE_BEGIN()

KAPHEIN_PP_DLL_API
int kphnMathVecfDot(
    const float* pLhs
    , kphnSizeType size
    , const float* pRhs
    , float* pOut
);

KAPHEIN_PP_DLL_API
int kphnMathMatfMakeZero(
    float* pMatInOut
    , kphnSizeType size
);

KAPHEIN_PP_DLL_API
int kphnMathMatfMakeIdentity(
	float* pSqMatInOut
	, kphnSizeType n
    , float diagonalValue
);

KAPHEIN_PP_DLL_API
int kphnMathMatfScale(
    float* pLhs
    , kphnSizeType size
    , float factor
);

KAPHEIN_PP_DLL_API
int kphnMathMatfAdd(
	float* pLhs
	, kphnSizeType size
	, const float* pRhs
);

KAPHEIN_PP_DLL_API
int kphnMathMatfSub(
	float* pLhs
    , kphnSizeType size
	, const float* pRhs
);

KAPHEIN_PP_DLL_API
int kphnMathMatfPointScale(
    float* pLhs
    , kphnSizeType size
    , const float* pRhs
);

KAPHEIN_PP_DLL_API
int kphnMathMatfMul(
    const float* pLhs
	, kphnSizeType nLhsRow
	, kphnSizeType nLhsCol
	, const float* pRhs
    , kphnSizeType nRhsCol
    , float* pOut
);

KAPHEIN_PP_DLL_API
int kphnMathMatfTranspose(
	const float* pMat
	, kphnSizeType nRow
	, kphnSizeType nCol
	, float* pOut
);

KAPHEIN_PP_DLL_API
int kphnMathMatSqfDecomposeIntoLu(
	const float* pSqMat
	, kphnSizeType n
	, float* pLMatOut
	, float* pUMatOut
    , float epsilon
);

int kphnMathMatSqfDecomposeIntoLup(
    float* pSqMat
    , kphnSizeType n
    , float* pLMatOut
    , float* pUMatOut
    , float* pPMatOut
    , float epsilon
);

int kphnMathMatSqfGetInverse(
	const float* pSqMat
	, kphnSizeType nSize
	, float* pInverseOut
    , float* pDeterminantOut
    , float epsilon
);

KAPHEIN_PP_C_LINKAGE_END()

#endif
