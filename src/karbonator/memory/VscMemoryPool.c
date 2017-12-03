#include <stdlib.h>
#include <string.h>
#include "karbonator/memory/VscMemoryPool.h"

//static inline size_t getBaseTwoPaddingSize(
//    size_t size,
//    size_t baseTwoExp
//) {
//    const size_t align = ((size_t)1 << baseTwoExp);
//    const size_t alignMinusOne = (align - 1);
//    
//    return (align - (size & alignMinusOne)) & alignMinusOne;
//};

static INLINE char* getBeginOfBound(
    VscMemoryPool* pPool
) {
    return (char*)pPool->pHead_;
}

static INLINE char* getEndOfBound(
    VscMemoryPool* pPool
) {
    return getBeginOfBound(pPool) + pPool->blockSize_;
}

static INLINE char* getChunkPointer(
    VscHeader* pHeader
) {
    return (char*)pHeader + sizeof(VscHeader);
}

karbonator_ErrorCode VscMemoryPool_construct(
    VscMemoryPool* pPool,
    size_t blockSize
) {
    void* pBlock;

    if(pPool == NULL) {
        return krbntrEcNullArgument;
    }
    
    if(blockSize < sizeof(VscHeader)) {
        return krbntrEcInvalidArgument;
    }
    
    pPool->blockSize_ = blockSize;
    pBlock = malloc(blockSize);
    if(pBlock == NULL) {
        return krbntrEcNotEnoughMemory;
    }

    #ifdef _DEBUG
    memset(pBlock, 0xDD, blockSize);
    #endif

    pPool->pHead_ = (VscHeader*)pBlock;
    pPool->pHead_->m_.f_.byteCount_ = blockSize - sizeof(VscHeader);
    pPool->pHead_->m_.f_.pPrev_ = NULL;
    pPool->pHead_->m_.f_.pNext_ = NULL;
    pPool->pHead_->m_.f_.inUse_ = 0;

    pPool->pCurrent_ = pPool->pHead_;

    return krbntrEcNoError;
}

void VscMemoryPool_destruct(
    VscMemoryPool* pPool
) {
    free(pPool->pHead_);
}

static VscHeader* findAvailable(
    VscHeader* pStart,
    VscHeader* pEnd,
    size_t byteCount
) {
    VscHeader* pAvailable = NULL;
    VscHeader* pCurrent = pStart;
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

void* VscMemoryPool_allocate(
    VscMemoryPool* pPool,
    size_t byteCount
) {
    char* pChunk = NULL;
    VscHeader* pAvailable;
    VscHeader* pOldNext;

    pAvailable = findAvailable(pPool->pCurrent_, NULL, byteCount);
    if(pAvailable == NULL) {
        pAvailable = findAvailable(pPool->pHead_, pPool->pCurrent_, byteCount);
    }

    if(pAvailable != NULL) {
        pAvailable->m_.f_.inUse_ = 1;
        pChunk = getChunkPointer(pAvailable);
        
        pOldNext = pAvailable->m_.f_.pNext_;
        if(
            pOldNext == NULL
            || (uintptr_t)((char*)pOldNext - pChunk) > byteCount + sizeof(VscHeader)
        ) {
            char* const pNextBytes = pChunk + byteCount;
            size_t nextByteCount = 0;
            VscHeader* pNewNext = NULL;
            if(pNextBytes + sizeof(VscHeader) < getEndOfBound(pPool)) {
                pNewNext = (VscHeader*)((void*)pNextBytes);
                nextByteCount = pAvailable->m_.f_.byteCount_ - (sizeof(VscHeader) + byteCount);
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

        pPool->pCurrent_ = (
            pAvailable->m_.f_.pNext_ != NULL
            ? pAvailable->m_.f_.pNext_
            : pPool->pHead_
        );
        
        #ifdef _DEBUG
        memset(pChunk, 0xAA, pAvailable->m_.f_.byteCount_);
        #endif
    }

    return pChunk;
}

static VscHeader* mergeHeaders(
    VscHeader* pStart
) {
    VscHeader* pMerger;
    VscHeader* pNext;
    VscHeader* pNextOfNext;

    for(
        pMerger = pStart;
        pMerger->m_.f_.pPrev_ != NULL && !pMerger->m_.f_.pPrev_->m_.f_.inUse_;
        pMerger = pMerger->m_.f_.pPrev_
    );
    
    for(
        pNext = pMerger->m_.f_.pNext_;
        pNext != NULL && !pNext->m_.f_.inUse_;
    ) {
        pMerger->m_.f_.byteCount_ += sizeof(VscHeader) + pNext->m_.f_.byteCount_;

        pNextOfNext = pNext->m_.f_.pNext_;
        pMerger->m_.f_.pNext_ = pNextOfNext;
        if(pNextOfNext != NULL) {
            pNextOfNext->m_.f_.pPrev_ = pMerger;
        }

        pNext = pNextOfNext;
    }

    return pMerger;
}

int VscMemoryPool_free(
    VscMemoryPool* pPool,
    void* pChunk
) {
    VscHeader* pHeader;
    char* pHeaderBytes;

    pHeaderBytes = (char*)pChunk - sizeof(VscHeader);
    if(
        pHeaderBytes < getBeginOfBound(pPool)
        || pHeaderBytes >= getEndOfBound(pPool)
    ) {
        return -1;
    }

    pHeader = (VscHeader*)pHeaderBytes;
    if(!pHeader->m_.f_.inUse_) {
        return 0;
    }

    pHeader->m_.f_.inUse_ = 0;

    pPool->pCurrent_ = mergeHeaders(pHeader);
    
    #ifdef _DEBUG
    memset(getChunkPointer(pPool->pCurrent_), 0xDD, pPool->pCurrent_->m_.f_.byteCount_);
    #endif

    return 0;
}
