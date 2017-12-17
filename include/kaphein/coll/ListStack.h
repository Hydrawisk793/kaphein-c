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

#ifndef KAPHEIN_COLL_LISTSTACK_H
#define KAPHEIN_COLL_LISTSTACK_H

#include "kaphein/def.h"

KAPHEIN_PP_C_LINKAGE_BEGIN()

struct kphnCollListStackNode
{
    void* pElement_;

    struct kphnCollListStackNode* pNext_;
};

struct kphnCollListStack
{
    struct kphnCollListStackNode* pTop_;

    size_t count_;
};

KAPHEIN_PP_DLL_API
int kphnCollListStackConstruct(
    struct kphnCollListStack* pStack
);

KAPHEIN_PP_DLL_API
void kphnCollListStackDestruct(
    struct kphnCollListStack* pStack
);

KAPHEIN_PP_DLL_API
bool kphnCollListStackIsEmpty(
    const struct kphnCollListStack* pStack
);

KAPHEIN_PP_DLL_API
size_t kphnCollListStackGetCount(
    const struct kphnCollListStack* pStack
);

KAPHEIN_PP_DLL_API
int kphnCollListStackPeek(
    struct kphnCollListStack* pStack
    , void** pDest
);

KAPHEIN_PP_DLL_API
void kphnCollListStackPush(
    struct kphnCollListStack* pStack
    , void* pElement
);

KAPHEIN_PP_DLL_API
int kphnCollListStackPop(
    struct kphnCollListStack* pStack
    , void** pDest
);

KAPHEIN_PP_DLL_API
void kphnCollListStackClear(
    struct kphnCollListStack* pStack
);

KAPHEIN_PP_C_LINKAGE_END()

#endif
