#include <stdlib.h>
#include <string.h>
#include "karbonator/memory/FscMemoryPool.h"

karbonator_ErrorCode FscMemoryPool_construct(
    FscMemoryPool* pPool,
    size_t chunkSize,
    size_t chunkCount
) {
    pPool->chunkSize_ = chunkSize;

    pPool->blockSize_ = chunkSize * chunkCount;    
    pPool->pBlock_ = malloc(pPool->blockSize_);
    if(NULL == pPool->pBlock_) {
        return krbntrEcNotEnoughMemory;
    }

    pPool->pCurrent_ = pPool->pBlock_;

    return krbntrEcNoError;
}

void FscMemoryPool_destruct(
    FscMemoryPool* pPool
) {
    free(pPool->pBlock_);
}

void* FscMemoryPool_allocate(
    FscMemoryPool* pPool,
    size_t byteCount
) {
    pPool;
    byteCount;
    
    return NULL;
}

int FscMemoryPool_free(
    FscMemoryPool* pPool,
    void* pChunk
) {
    pPool;
    pChunk;

    return -1;
}
