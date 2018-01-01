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
#include "kaphein/c/mem/VscPool.h"

static INLINE char* getBeginOfBound(
    struct kphnMemVscPool* pPool
)
{
    return (char*)pPool->pHead_;
}

static INLINE char* getEndOfBound(
    struct kphnMemVscPool* pPool
)
{
    return getBeginOfBound(pPool) + pPool->blockSize_;
}

static INLINE char* getChunkPointer(
    struct kphnMemVscHeader* pHeader
)
{
    return (char*)pHeader + sizeof(struct kphnMemVscHeader);
}

int kphnMemVscPoolConstruct(
    struct kphnMemVscPool* pPool
    , kphnSizeType blockSize
)
{
    void* pBlock;

    if(pPool == NULL) {
        return kphnEcNullArgument;
    }
    
    if(blockSize < sizeof(struct kphnMemVscHeader)) {
        return kphnEcInvalidArgument;
    }
    
    pPool->blockSize_ = blockSize;
    pBlock = malloc(blockSize);
    if(pBlock == NULL) {
        return kphnEcNotEnoughMemory;
    }

#ifdef _DEBUG
    memset(pBlock, 0xDD, blockSize);
#endif
    
    pPool->pHead_ = (struct kphnMemVscHeader*)pBlock;
    pPool->pHead_->m_.f_.byteCount_ = blockSize - sizeof(struct kphnMemVscHeader);
    pPool->pHead_->m_.f_.pPrev_ = NULL;
    pPool->pHead_->m_.f_.pNext_ = NULL;
    pPool->pHead_->m_.f_.inUse_ = 0;

    pPool->pCurrent_ = pPool->pHead_;

    return kphnEcNoError;
}

void kphnMemVscPoolDestruct(
    struct kphnMemVscPool* pPool
)
{
    free(pPool->pHead_);
}

static struct kphnMemVscHeader* findAvailable(
    struct kphnMemVscHeader* pStart
    , struct kphnMemVscHeader* pEnd
    , kphnSizeType byteCount
)
{
    struct kphnMemVscHeader* pAvailable = NULL;
    struct kphnMemVscHeader* pCurrent = pStart;

    do {
        if(!pCurrent->m_.f_.inUse_ && pCurrent->m_.f_.byteCount_ >= byteCount) {
            pAvailable = pCurrent;
            break;
        }

        pCurrent = pCurrent->m_.f_.pNext_;
    }
    while(pCurrent != pEnd);

    return pAvailable;
}

void* kphnMemVscPoolAllocate(
    void* pPool
    , kphnSizeType byteCount
)
{
    struct kphnMemVscPool* const pPoolPtr = (struct kphnMemVscPool*)pPool;
    char* pChunk = NULL;
    struct kphnMemVscHeader* pAvailable;
    struct kphnMemVscHeader* pOldNext;

    pAvailable = findAvailable(pPoolPtr->pCurrent_, NULL, byteCount);
    if(pAvailable == NULL) {
        pAvailable = findAvailable(pPoolPtr->pHead_, pPoolPtr->pCurrent_, byteCount);
    }

    if(pAvailable != NULL) {
        pAvailable->m_.f_.inUse_ = 1;
        pChunk = getChunkPointer(pAvailable);
        
        pOldNext = pAvailable->m_.f_.pNext_;
        if(
            pOldNext == NULL
            || (uintptr_t)((char*)pOldNext - pChunk) > byteCount + sizeof(struct kphnMemVscHeader)
        ) {
            char* const pNextBytes = pChunk + byteCount;
            kphnSizeType nextByteCount = 0;
            struct kphnMemVscHeader* pNewNext = NULL;
            if(pNextBytes + sizeof(struct kphnMemVscHeader) < getEndOfBound(pPoolPtr)) {
                pNewNext = (struct kphnMemVscHeader*)((void*)pNextBytes);
                nextByteCount = pAvailable->m_.f_.byteCount_ - (sizeof(struct kphnMemVscHeader) + byteCount);
                pAvailable->m_.f_.byteCount_ = byteCount;
            }

            if(pNewNext != NULL) {
                pNewNext->m_.f_.byteCount_ = nextByteCount;
                pNewNext->m_.f_.inUse_ = 0;
                pNewNext->m_.f_.pNext_ = pOldNext;
                pNewNext->m_.f_.pPrev_ = pAvailable;
                pAvailable->m_.f_.pNext_ = pNewNext;

                if(pOldNext != NULL) {
                    pOldNext->m_.f_.pPrev_ = pNewNext;
                }
            }
        }

        pPoolPtr->pCurrent_ = (
            pAvailable->m_.f_.pNext_ != NULL
            ? pAvailable->m_.f_.pNext_
            : pPoolPtr->pHead_
        );
        
    #ifdef _DEBUG
        memset(pChunk, 0xAA, pAvailable->m_.f_.byteCount_);
    #endif
    }

    return pChunk;
}

static struct kphnMemVscHeader* mergeHeaders(
    struct kphnMemVscHeader* pStart
)
{
    struct kphnMemVscHeader* pMerger;
    struct kphnMemVscHeader* pNext;
    struct kphnMemVscHeader* pNextOfNext;

    for(
        pMerger = pStart;
        pMerger->m_.f_.pPrev_ != NULL && !pMerger->m_.f_.pPrev_->m_.f_.inUse_;
        pMerger = pMerger->m_.f_.pPrev_
    );
    
    for(
        pNext = pMerger->m_.f_.pNext_;
        pNext != NULL && !pNext->m_.f_.inUse_;
    ) {
        pMerger->m_.f_.byteCount_ += sizeof(struct kphnMemVscHeader) + pNext->m_.f_.byteCount_;

        pNextOfNext = pNext->m_.f_.pNext_;
        pMerger->m_.f_.pNext_ = pNextOfNext;
        if(pNextOfNext != NULL) {
            pNextOfNext->m_.f_.pPrev_ = pMerger;
        }

        pNext = pNextOfNext;
    }

    return pMerger;
}

int kphnMemVscPoolDeallocate(
    void* pPool
    , void* pChunk
    , kphnSizeType byteCount
)
{
    struct kphnMemVscPool* const pPoolPtr = (struct kphnMemVscPool*)pPool;
    struct kphnMemVscHeader* pHeader;
    char* pHeaderBytes;

    byteCount;

    pHeaderBytes = (char*)pChunk - sizeof(struct kphnMemVscHeader);
    if(
        pHeaderBytes < getBeginOfBound(pPoolPtr)
        || pHeaderBytes >= getEndOfBound(pPoolPtr)
    ) {
        return -1;
    }

    pHeader = (struct kphnMemVscHeader*)pHeaderBytes;
    if(!pHeader->m_.f_.inUse_) {
        return 0;
    }

    pHeader->m_.f_.inUse_ = 0;

    pPoolPtr->pCurrent_ = mergeHeaders(pHeader);
    
#ifdef _DEBUG
    memset(getChunkPointer(pPoolPtr->pCurrent_), 0xDD, pPoolPtr->pCurrent_->m_.f_.byteCount_);
#endif

    return 0;
}
