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
#include "kaphein/c/mem/FscPool.h"

int kphnMemFscPoolConstruct(
    struct kphnMemFscPool* pPool
    , kphnSizeType chunkSize
    , kphnSizeType chunkCount
)
{
    pPool->chunkSize_ = chunkSize;

    pPool->blockSize_ = chunkSize * chunkCount;    
    pPool->pBlock_ = malloc(pPool->blockSize_);
    if(NULL == pPool->pBlock_) {
        return kphnEcNotEnoughMemory;
    }

    pPool->pCurrent_ = pPool->pBlock_;

    return kphnEcNoError;
}

void kphnMemFscPoolDestruct(
    struct kphnMemFscPool* pPool
)
{
    free(pPool->pBlock_);
}

void* kphnMemFscPoolAllocate(
    void* pPool
    , kphnSizeType byteCount
)
{
    pPool;
    byteCount;
    
    return NULL;
}

int kphnMemFscPoolDeallocate(
    void* pPool
    , void* pChunk
    , kphnSizeType byteCount
)
{
    pPool;
    pChunk;
    byteCount;

    return -1;
}
