#ifndef KARBONATOR_MEMORY_FSCMEMORYPOOL_H
#define KARBONATOR_MEMORY_FSCMEMORYPOOL_H

#include <stddef.h>
#include "karbonator/pp/basic.h"
#include "karbonator/ErrorCode.h"

KARBONATOR_PP_C_LINKAGE_BEGIN()

typedef struct FscMemoryPool_ {
    void* pCurrent_;
    void* pBlock_;
    size_t blockSize_;
    size_t chunkSize_;
} FscMemoryPool;

KARBONATOR_PP_DLL_API
karbonator_ErrorCode FscMemoryPool_construct(
    FscMemoryPool* pPool,
    size_t chunkSize,
    size_t chunkCount
);

KARBONATOR_PP_DLL_API
void FscMemoryPool_destruct(
    FscMemoryPool* pPool
);

void* FscMemoryPool_allocate(
    FscMemoryPool* pPool,
    size_t byteCount
);

int FscMemoryPool_free(
    FscMemoryPool* pPool,
    void* pChunk
);

KARBONATOR_PP_C_LINKAGE_END()

#endif
