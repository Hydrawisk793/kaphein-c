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

#ifndef KAPHEIN_C_COLL_TREESTACK_H
#define KAPHEIN_C_COLL_TREESTACK_H

#include "../def.h"

enum kaphein_SearchTarget
{
    stNotGreater
    , stLess
    , stNotLess
    , stGreater
    , stEqual
};

struct kphnCollTreeSetNode
{
    void* pElement_;

    struct kphnCollTreeSetNode* pParent_;

    union
    {
        struct kphnCollTreeSetNode* ptrs_ [2];

        struct
        {
            struct kphnCollTreeSetNode* pLeft_;

            struct kphnCollTreeSetNode* pRight_;
        } pair_;
    } child_;

    char red_;
};

KAPHEIN_PP_DLL_API
struct kphnCollTreeSetNode* kphnCollTreeSetNodeFindLeftMost(
    struct kphnCollTreeSetNode* pNode
);

KAPHEIN_PP_DLL_API
struct kphnCollTreeSetNode* kphnCollTreeSetNodeFindRightMost(
    struct kphnCollTreeSetNode* pNode
);

KAPHEIN_PP_DLL_API
struct kphnCollTreeSetNode* kphnCollTreeSetNodeFindLess(
    struct kphnCollTreeSetNode* pNode
);

KAPHEIN_PP_DLL_API
struct kphnCollTreeSetNode* kphnCollTreeSetNodeFindGreater(
    struct kphnCollTreeSetNode* pNode
);

struct kphnCollTreeSet
{
    struct kphnCollTreeSetNode* pRoot_;

    kphnSizeType count_;

    kaphein_ComparatorPtr pComparator_;
};

KAPHEIN_PP_DLL_API
int kphnCollTreeSetConstruct(
    struct kphnCollTreeSet* pTreeSet
    , kaphein_ComparatorPtr pComparator
);

KAPHEIN_PP_DLL_API
void kphnCollTreeSetDestruct(
    struct kphnCollTreeSet* pTreeSet
);

KAPHEIN_PP_DLL_API
kphnSizeType kphnCollTreeSetGetCount(
    const struct kphnCollTreeSet* pTreeSet
);

KAPHEIN_PP_DLL_API
bool kphnCollTreeSetHas(
    const struct kphnCollTreeSet* pTreeSet
    , const void* pElement
);

void* kphnCollTreeSetFindLessThan(
    const struct kphnCollTreeSet* pTreeSet
    , const void* pElement
);

void* kphnCollTreeSetFindNotGreaterThan(
    const struct kphnCollTreeSet* pTreeSet
    , const void* pElement
);

void* kphnCollTreeSetFindGreaterThan(
    const struct kphnCollTreeSet* pTreeSet
    , const void* pElement
);

void* kphnCollTreeSetFindNotLessThan(
    const struct kphnCollTreeSet* pTreeSet
    , const void* pElement
);

KAPHEIN_PP_DLL_API
struct kphnCollTreeSetNode* kphnCollTreeSetAdd(
    struct kphnCollTreeSet* pTreeSet
    , void* pElement
);

KAPHEIN_PP_DLL_API
struct kphnCollTreeSetNode* kphnCollTreeSetRemove(
    struct kphnCollTreeSet* pTreeSet
    , void* pElement
);

KAPHEIN_PP_DLL_API
void kphnCollTreeSetClear(
    struct kphnCollTreeSet* pTreeSet
);

#endif
