#include <stdlib.h>
#include <string.h>
#include "karbonator/collection/ListStack.h"

karbonator_ErrorCode karbonator_collection_ListStack_construct(
    karbonator_collection_ListStack* pStack
) {
    pStack->pTop_ = NULL;
    pStack->count_ = 0;

    return krbntrEcNoError;
}

void karbonator_collection_ListStack_destruct(
    karbonator_collection_ListStack* pStack
) {
    karbonator_collection_ListStack_clear(pStack);
}

bool karbonator_collection_ListStack_isEmpty(
    const karbonator_collection_ListStack* pStack
) {
    return pStack->pTop_ == NULL;
}

size_t karbonator_collection_ListStack_getCount(
    const karbonator_collection_ListStack* pStack
) {
    return pStack->count_;
}

int karbonator_collection_ListStack_peek(
    karbonator_collection_ListStack* pStack,
    void** pDest
) {
    if(pStack->pTop_ == NULL) {
        return -1;
    }

    *pDest = pStack->pTop_->pElement_;

    return 0;
}

void karbonator_collection_ListStack_push(
    karbonator_collection_ListStack* pStack,
    void* pElement
) {
    karbonator_collection_ListStackNode* const pNext = pStack->pTop_;
    karbonator_collection_ListStackNode* const newNode = malloc(sizeof(karbonator_collection_ListStackNode));
    newNode->pElement_ = pElement;
    newNode->pNext_ = pNext;
    pStack->pTop_ = newNode;

    ++pStack->count_;
}

int karbonator_collection_ListStack_pop(
    karbonator_collection_ListStack* pStack,
    void** pDest
) {
    const int result = karbonator_collection_ListStack_peek(pStack, pDest);
    if(result == 0) {
        karbonator_collection_ListStackNode* pNext = pStack->pTop_->pNext_;
        free(pStack->pTop_);
        pStack->pTop_ = pNext;

        --pStack->count_;
    }

    return result;
}

void karbonator_collection_ListStack_clear(
    karbonator_collection_ListStack* pStack
) {
    void* pElem;
    while(!karbonator_collection_ListStack_pop(pStack, &pElem));
}
