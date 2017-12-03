#ifndef KARBONATOR_COLLECTION_LISTSTACK_H
#define KARBONATOR_COLLECTION_LISTSTACK_H

#include <stddef.h>
#include "karbonator/ErrorCode.h"
#include "karbonator/pp/basic.h"

KARBONATOR_PP_C_LINKAGE_BEGIN()

typedef struct karbonator_collection_ListStackNode_ {
    void* pElement_;
    struct karbonator_collection_ListStackNode_* pNext_;
} karbonator_collection_ListStackNode;

typedef struct karbonator_collection_ListStack_ {
    karbonator_collection_ListStackNode* pTop_;
    size_t count_;
} karbonator_collection_ListStack;

KARBONATOR_PP_DLL_API
karbonator_ErrorCode karbonator_collection_ListStack_construct(
    karbonator_collection_ListStack* pStack
);

KARBONATOR_PP_DLL_API
void karbonator_collection_ListStack_destruct(
    karbonator_collection_ListStack* pStack
);

KARBONATOR_PP_DLL_API
bool karbonator_collection_ListStack_isEmpty(
    const karbonator_collection_ListStack* pStack
);

KARBONATOR_PP_DLL_API
size_t karbonator_collection_ListStack_getCount(
    const karbonator_collection_ListStack* pStack
);

KARBONATOR_PP_DLL_API
int karbonator_collection_ListStack_peek(
    karbonator_collection_ListStack* pStack,
    void** pDest
);

KARBONATOR_PP_DLL_API
void karbonator_collection_ListStack_push(
    karbonator_collection_ListStack* pStack,
    void* pElement
);

KARBONATOR_PP_DLL_API
int karbonator_collection_ListStack_pop(
    karbonator_collection_ListStack* pStack,
    void** pDest
);

KARBONATOR_PP_DLL_API
void karbonator_collection_ListStack_clear(
    karbonator_collection_ListStack* pStack
);

KARBONATOR_PP_C_LINKAGE_END()

#endif
