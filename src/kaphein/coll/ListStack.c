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
#include "kaphein/coll/ListStack.h"
 
int kphnCollListStackConstruct(
    struct kphnCollListStack* pStack
)
{
    pStack->pTop_ = NULL;
    pStack->count_ = 0;

    return krbntrEcNoError;
}

void kphnCollListStackDestruct(
    struct kphnCollListStack* pStack
)
{
    kphnCollListStackClear(pStack);
}

bool kphnCollListStackIsEmpty(
    const struct kphnCollListStack* pStack
)
{
    return pStack->pTop_ == NULL;
}

size_t kphnCollListStackGetCount(
    const struct kphnCollListStack* pStack
)
{
    return pStack->count_;
}

int kphnCollListStackPeek(
    struct kphnCollListStack* pStack
    , void** pDest
)
{
    if(pStack->pTop_ == NULL) {
        return -1;
    }

    *pDest = pStack->pTop_->pElement_;

    return 0;
}

void kphnCollListStackPush(
    struct kphnCollListStack* pStack
    , void* pElement
)
{
    struct kphnCollListStackNode* const pNext = pStack->pTop_;
    struct kphnCollListStackNode* const newNode = (struct kphnCollListStackNode*)malloc(sizeof(struct kphnCollListStackNode));
    newNode->pElement_ = pElement;
    newNode->pNext_ = pNext;
    pStack->pTop_ = newNode;

    ++pStack->count_;
}

int kphnCollListStackPop(
    struct kphnCollListStack* pStack
    , void** pDest
)
{
    const int result = kphnCollListStackPeek(pStack, pDest);
    if(result == 0) {
        struct kphnCollListStackNode* pNext = pStack->pTop_->pNext_;
        free(pStack->pTop_);
        pStack->pTop_ = pNext;

        --pStack->count_;
    }

    return result;
}

void kphnCollListStackClear(
    struct kphnCollListStack* pStack
)
{
    void* pElem;

    while(!kphnCollListStackPop(pStack, &pElem));
}
