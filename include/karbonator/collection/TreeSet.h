#ifndef KARBONATOR_COLLECTION_TREESET_H
#define KARBONATOR_COLLECTION_TREESET_H

#include <stddef.h>
#include "karbonator/ErrorCode.h"
#include "karbonator/pp/basic.h"
#include "fwd.h"

KARBONATOR_PP_C_LINKAGE_BEGIN()

typedef enum SearchTarget_ {
    stNotGreater,
    stLess,
    stNotLess,
    stGreater,
    stEqual,
} SearchTarget;

typedef struct RbTreeNode_ {
    void* pElement_;
    struct RbTreeNode_* pParent_;
    union {
        struct RbTreeNode_* ptrs_ [2];
        struct {
            struct RbTreeNode_* pLeft_;
            struct RbTreeNode_* pRight_;
        } pair_;
    } child_;
    char red_;
} RbTreeNode;

KARBONATOR_PP_DLL_API
RbTreeNode* RbTreeNode_findLeftMost(
    RbTreeNode* pNode
);

KARBONATOR_PP_DLL_API
RbTreeNode* RbTreeNode_findRightMost(
    RbTreeNode* pNode
);

KARBONATOR_PP_DLL_API
RbTreeNode* RbTreeNode_findLess(
    RbTreeNode* pNode
);

KARBONATOR_PP_DLL_API
RbTreeNode* RbTreeNode_findGreater(
    RbTreeNode* pNode
);

typedef struct TreeSet_ {
    RbTreeNode* pRoot_;
    size_t count_;
    ComparatorPtr pComparator_;
} TreeSet;

KARBONATOR_PP_DLL_API
karbonator_ErrorCode TreeSet_construct(
    TreeSet* pTreeSet,
    ComparatorPtr pComparator
);

KARBONATOR_PP_DLL_API
void TreeSet_destruct(
    TreeSet* pTreeSet
);

KARBONATOR_PP_DLL_API
size_t TreeSet_getCount(
    const TreeSet* pTreeSet
);

KARBONATOR_PP_DLL_API
bool TreeSet_has(
    const TreeSet* pTreeSet,
    const void* pElement
);

void* TreeSet_findLessThan(
    const TreeSet* pTreeSet,
    const void* pElement
);

void* TreeSet_findNotGreaterThan(
    const TreeSet* pTreeSet,
    const void* pElement
);

void* TreeSet_findGreaterThan(
    const TreeSet* pTreeSet,
    const void* pElement
);

void* TreeSet_findNotLessThan(
    const TreeSet* pTreeSet,
    const void* pElement
);

KARBONATOR_PP_DLL_API
RbTreeNode* TreeSet_add(
    TreeSet* pTreeSet,
    void* pElement
);

KARBONATOR_PP_DLL_API
RbTreeNode* TreeSet_remove(
    TreeSet* pTreeSet,
    void* pElement
);

KARBONATOR_PP_DLL_API
void TreeSet_clear(
    TreeSet* pTreeSet
);

KARBONATOR_PP_C_LINKAGE_END()

#endif
