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
#include <string.h>
#include "kaphein/c/ncod/cp949.h"

int kphnNcodCp949ToUcs4(
    const uint16_t* strIn
    , size_t strLength
    , uint32_t* pCharOut
    , size_t charOutSize
    , size_t* pCharCountOut
)
{
    static const uint32_t lut[94 * 94] = {0};

    const uint16_t* pSrcEnd;
    uint32_t* pDestEnd;
    size_t charCount;

    if(strIn == NULL || pCharOut == NULL) {
        return krbntrEcNullArgument;
    }

    charCount = 0;
    pSrcEnd = strIn + strLength;
    pDestEnd = pCharOut + charOutSize;
    while(strIn < pSrcEnd) {
        if(pCharOut >= pDestEnd) {
            return krbntrEcNotEnoughBufferSize;
        }
        *pCharOut++ = lut[*strIn++];
        ++charCount;
    }

    if(pCharCountOut != NULL) {
        *pCharCountOut = charCount;
    }

    return krbntrEcGenericError;
}
