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
 
#ifndef KAPHEIN_MEM_FSCPOOL_H
#define KAPHEIN_MEM_FSCPOOL_H

#include "kaphein/def.h"

KAPHEIN_PP_C_LINKAGE_BEGIN()
 
struct kphnMemFscPool
{
    void* pCurrent_;

    void* pBlock_;

    size_t blockSize_;

    size_t chunkSize_;
};

KAPHEIN_PP_DLL_API
int kphnMemFscPoolConstruct(
    struct kphnMemFscPool* pPool
    , size_t chunkSize
    , size_t chunkCount
);

KAPHEIN_PP_DLL_API
void kphnMemFscPoolDestruct(
    struct kphnMemFscPool* pPool
);

void* kphnMemFscPoolAllocate(
    struct kphnMemFscPool* pPool
    , size_t byteCount
);

int kphnMemFscPoolFree(
    struct kphnMemFscPool* pPool
    , void* pChunk
);

KAPHEIN_PP_C_LINKAGE_END()

#endif
