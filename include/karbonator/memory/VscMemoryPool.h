#ifndef KARBONATOR_MEMORY_VSCMEMORYPOOL_H
#define KARBONATOR_MEMORY_VSCMEMORYPOOL_H

#include <stddef.h>
#include "karbonator/pp/basic.h"
#include "karbonator/ErrorCode.h"

KARBONATOR_PP_C_LINKAGE_BEGIN()

typedef struct VscHeader_ {
    union {
        struct {
            struct VscHeader_* pPrev_;
            struct VscHeader_* pNext_;
            size_t byteCount_;
            char inUse_;
        } f_;
        char bytes [sizeof(void*) << 2];
    } m_;
} VscHeader;

typedef struct VscMemoryPool_ {
    VscHeader* pHead_;
    VscHeader* pCurrent_;
    size_t blockSize_;
} VscMemoryPool;

KARBONATOR_PP_DLL_API
karbonator_ErrorCode VscMemoryPool_construct(
    VscMemoryPool* pPool,
    size_t blockSize_
);

KARBONATOR_PP_DLL_API
void VscMemoryPool_destruct(
    VscMemoryPool* pPool
);

KARBONATOR_PP_DLL_API
void* VscMemoryPool_allocate(
    VscMemoryPool* pPool,
    size_t byteCount
);

KARBONATOR_PP_DLL_API
int VscMemoryPool_free(
    VscMemoryPool* pPool,
    void* pChunk
);

KARBONATOR_PP_C_LINKAGE_END()

#endif
