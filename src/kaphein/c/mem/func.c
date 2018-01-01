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

#include "kaphein/c/mem/func.h"

int kphnMemTranspose2D(
    const void* pSrc
    , kphnSizeType srcRowCount
    , kphnSizeType srcColumnCount
    , void* pDest
    , kphnSizeType destSize
    , kphnCopyAssignFuncPtr caFuncPtr
    , kphnSizeType elementSize
)
{
    const char* pCurrentSrc;
    char* pCurrentDest;
    kphnSizeType srcSize;
    kphnSizeType srcX, srcY;
    int result;

    if(pSrc == NULL || pDest == NULL || caFuncPtr == NULL) {
        return kphnEcNullArgument;
    }

    srcSize = srcRowCount * srcColumnCount;
    if(destSize < srcSize) {
        return kphnEcNotEnoughBufferSize;
    }

    pCurrentSrc = (const char*)pSrc;
    pCurrentDest = (char*)pDest;
    for(srcY = 0; srcY < srcRowCount; ++srcY) {
        for(srcX = 0; srcX < srcColumnCount; ++srcX) {
            result = (*caFuncPtr)(
                pCurrentDest + (srcX * srcRowCount + srcY) * elementSize,
                pCurrentSrc + (srcY * srcColumnCount + srcX) * elementSize
            );
            if(result != kphnEcNoError) {
                return result;
            }
        }
    }

    return kphnEcNoError;
}
