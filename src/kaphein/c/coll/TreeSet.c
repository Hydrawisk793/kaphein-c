#ifdef _DEBUG
    #include <stdio.h>
#endif
#include <stdlib.h>
#include <string.h>
#include "kaphein/c/coll/ListStack.h"
#include "kaphein/c/coll/TreeSet.h"

typedef bool (*NodeTraversalHandlerPtr)(
    void*
    , struct kphnCollTreeSetNode*
);

enum RotationDirection
{
    rdLeft = 0
    , rdRight = 1
};

static INLINE bool RbTreeNode_isNil(
    const struct kphnCollTreeSetNode* pNode
)
{
    return pNode->pElement_ == NULL;
}

static INLINE bool RbTreeNode_isNullOrNil(
    const struct kphnCollTreeSetNode* pNode
)
{
    return pNode == NULL || RbTreeNode_isNil(pNode);
}

static INLINE bool RbTreeNode_hasNonNilLeftChild(
    const struct kphnCollTreeSetNode* pNode
)
{
    return pNode->child_.pair_.pLeft_ != NULL
        && !RbTreeNode_isNil(pNode->child_.pair_.pLeft_)
    ;
}

static INLINE bool RbTreeNode_hasNonNilRightChild(
    const struct kphnCollTreeSetNode* pNode
)
{
    return pNode->child_.pair_.pRight_ != NULL
        && !RbTreeNode_isNil(pNode->child_.pair_.pRight_)
    ;
}

static INLINE bool RbTreeNode_isNonNilLeaf(
    const struct kphnCollTreeSetNode* pNode
)
{
    return !RbTreeNode_hasNonNilLeftChild(pNode)
        && !RbTreeNode_hasNonNilRightChild(pNode)
    ;
}

static struct kphnCollTreeSetNode* RbTreeNode_getRoot(
    struct kphnCollTreeSetNode* pNode
)
{
    struct kphnCollTreeSetNode* pRoot = pNode;
    for(; pRoot->pParent_ != NULL; pRoot = pRoot->pParent_);

    return pRoot;
}

static INLINE struct kphnCollTreeSetNode* RbTreeNode_getOtherChild(
    struct kphnCollTreeSetNode* pParent
    , struct kphnCollTreeSetNode* pNode
)
{
    return (
        pParent != NULL
        ? (
            pNode == pParent->child_.pair_.pLeft_
            ? pParent->child_.pair_.pRight_
            : pParent->child_.pair_.pLeft_
        )
        : NULL
    );
}

static INLINE struct kphnCollTreeSetNode* RbTreeNode_getLastChild(
    struct kphnCollTreeSetNode * pNode
)
{
    return (
        !RbTreeNode_isNullOrNil(pNode->child_.pair_.pRight_)
        ? pNode->child_.pair_.pRight_
        : pNode->child_.pair_.pLeft_
    );
}

static INLINE struct kphnCollTreeSetNode* RbTreeNode_getSibling(
    struct kphnCollTreeSetNode * pNode
)
{
    return RbTreeNode_getOtherChild(pNode->pParent_, pNode);
}

static INLINE struct kphnCollTreeSetNode** RbTreeNode_getChildSlot(
    struct kphnCollTreeSetNode* pNode
)
{
    struct kphnCollTreeSetNode* const pParent = pNode->pParent_;
    return (
        pParent != NULL
        ? (
            pNode == pParent->child_.pair_.pLeft_
            ? &(pParent->child_.pair_.pLeft_)
            : &(pParent->child_.pair_.pRight_)
        )
        : NULL
    );
}

struct kphnCollTreeSetNode* kphnCollTreeSetNodeFindLeftMost(
    struct kphnCollTreeSetNode* pNode
)
{
    struct kphnCollTreeSetNode* pCurrent = pNode;
    for(
        ;
        pCurrent != NULL && RbTreeNode_hasNonNilLeftChild(pCurrent);
        pCurrent = pCurrent->child_.pair_.pLeft_
    );

    return pCurrent;
}

struct kphnCollTreeSetNode* kphnCollTreeSetNodeFindRightMost(
    struct kphnCollTreeSetNode* pNode
)
{
    struct kphnCollTreeSetNode* pCurrent = pNode;
    for(
        ;
        pCurrent != NULL && RbTreeNode_hasNonNilRightChild(pCurrent);
        pCurrent = pCurrent->child_.pair_.pRight_
    );

    return pCurrent;
}

static struct kphnCollTreeSetNode* RbTreeNode_findLeftSubTreeRoot(
    struct kphnCollTreeSetNode* pNode
)
{
    struct kphnCollTreeSetNode* pCurrent = pNode;
    struct kphnCollTreeSetNode* pParent;
    while(pCurrent != NULL) {
        pParent = pCurrent->pParent_;
        if(pParent == NULL || pCurrent == pParent->child_.pair_.pLeft_) {
            break;
        }

        pCurrent = pParent;
    }

    return pCurrent;
}

static struct kphnCollTreeSetNode* RbTreeNode_findRightSubTreeRoot(
    struct kphnCollTreeSetNode* pNode
)
{
    struct kphnCollTreeSetNode* pCurrent = pNode;
    struct kphnCollTreeSetNode* pParent;
    while(pCurrent != NULL) {
        pParent = pCurrent->pParent_;
        if(pParent == NULL || pCurrent == pParent->child_.pair_.pRight_) {
            break;
        }

        pCurrent = pParent;
    }

    return pCurrent;
}

struct kphnCollTreeSetNode* kphnCollTreeSetNodeFindLess(
    struct kphnCollTreeSetNode* pNode
)
{
    struct kphnCollTreeSetNode* pLess = NULL;
    struct kphnCollTreeSetNode* pRstRoot = NULL;
    
    if(RbTreeNode_hasNonNilLeftChild(pNode)) {
        pLess = kphnCollTreeSetNodeFindRightMost(pNode->child_.pair_.pLeft_);
    }
    else {
        pRstRoot = RbTreeNode_findRightSubTreeRoot(pNode);
        if(pRstRoot != NULL) {
            pLess = pRstRoot->pParent_;
        }
    }

    return pLess;
}

struct kphnCollTreeSetNode* kphnCollTreeSetNodeFindGreater(
    struct kphnCollTreeSetNode* pNode
)
{
    struct kphnCollTreeSetNode* pGreater = NULL;

    if(!RbTreeNode_isNil(pNode->child_.pair_.pRight_)) {
        pGreater = kphnCollTreeSetNodeFindLeftMost(pNode->child_.pair_.pRight_);
    }
    else if(pNode->pParent_ != NULL) {
        if(pNode == pNode->pParent_->child_.pair_.pLeft_) {
            pGreater = pNode->pParent_;
        }
        else {
            pGreater = RbTreeNode_findLeftSubTreeRoot(pNode)->pParent_;
        }
    }

    return pGreater;
}

static int RbTreeNode_traverseByPostorder(
    struct kphnCollTreeSetNode* pNode
    , NodeTraversalHandlerPtr pHandler
    , void* pContext
)
{
    struct kphnCollTreeSetNode* pCurrent;
    struct kphnCollTreeSetNode* pLastTraversed = NULL;
    struct kphnCollListStack nodeStack;
    bool stopTraversal = false;
    int childCount;
    void* pPopedNode;

    kphnCollListStackConstruct(&nodeStack);
    kphnCollListStackPush(&nodeStack, pNode);

    while(!stopTraversal && !kphnCollListStackIsEmpty(&nodeStack)) {
        kphnCollListStackPeek(&nodeStack, &pPopedNode);
        pCurrent = (struct kphnCollTreeSetNode*)pPopedNode;
        
        childCount = 0;
        if(RbTreeNode_getLastChild(pCurrent) != pLastTraversed) {
            if(!RbTreeNode_isNullOrNil(pCurrent->child_.pair_.pRight_)) {
                kphnCollListStackPush(&nodeStack, pCurrent->child_.pair_.pRight_);
                ++childCount;
            }
            if(!RbTreeNode_isNullOrNil(pCurrent->child_.pair_.pLeft_)) {
                kphnCollListStackPush(&nodeStack, pCurrent->child_.pair_.pLeft_);
                ++childCount;
            }
        }

        if(childCount < 1) {
            stopTraversal = (*pHandler)(pContext, pCurrent);
            pLastTraversed = pCurrent;
            kphnCollListStackPop(&nodeStack, &pPopedNode);
        }
    }

    kphnCollListStackDestruct(&nodeStack);

    return stopTraversal;
}

static void RbTreeNode_rotate(
    struct kphnCollTreeSetNode* pNode
    , int direction
)
{
    const int dirNdx = direction & 0x01;
    const int otherDirNdx = ~dirNdx & 0x01;
    struct kphnCollTreeSetNode** pChildSlot = RbTreeNode_getChildSlot(pNode);
    struct kphnCollTreeSetNode* const pParent = pNode->pParent_;
    struct kphnCollTreeSetNode* const pLeftChildOfRightChild = pNode->child_.ptrs_[otherDirNdx]->child_.ptrs_[dirNdx];

    pNode->child_.ptrs_[otherDirNdx]->child_.ptrs_[dirNdx] = pNode;
    pNode->pParent_ = pNode->child_.ptrs_[otherDirNdx];

    pNode->child_.ptrs_[otherDirNdx]->pParent_ = pParent;
    if(pChildSlot != NULL) {
        *pChildSlot = pNode->child_.ptrs_[otherDirNdx];
    }

    pNode->child_.ptrs_[otherDirNdx] = pLeftChildOfRightChild;
    if(!RbTreeNode_isNil(pLeftChildOfRightChild)) {
        pLeftChildOfRightChild->pParent_ = pNode;
    }
}

static const struct kphnCollTreeSetNode nilNode = {
    NULL
    , NULL
    , NULL
    , NULL
    , 0
};

static struct kphnCollTreeSetNode* TreeSet_createNode(
    struct kphnCollTreeSetNode* pParent,
    void* pElement
)
{
    struct kphnCollTreeSetNode* const newNode = (struct kphnCollTreeSetNode*)malloc(sizeof(struct kphnCollTreeSetNode));
    if(newNode != NULL) {
        newNode->pElement_ = pElement;
        newNode->pParent_ = pParent;
        newNode->child_.pair_.pLeft_ = (struct kphnCollTreeSetNode*)&nilNode;
        newNode->child_.pair_.pRight_ = (struct kphnCollTreeSetNode*)&nilNode;
        newNode->red_ = 1;
    }

    return newNode;
}

static struct kphnCollTreeSetNode* TreeSet_findNode(
    const struct kphnCollTreeSet* pTreeSet
    , const void* pElement
    , int searchTarget
)
{
    struct kphnCollTreeSetNode* pPrevious = NULL;
    struct kphnCollTreeSetNode* pCurrent = pTreeSet->pRoot_;
    int cmpResult;
    int found = 0;

    while(!found && !RbTreeNode_isNullOrNil(pCurrent)) {
        cmpResult = (*pTreeSet->pComparator_)(pElement, pCurrent->pElement_);
        if(cmpResult < 0) {
            pPrevious = pCurrent;
            pCurrent = pCurrent->child_.pair_.pLeft_;
        }
        else if(cmpResult > 0) {
            pPrevious = pCurrent;
            pCurrent = pCurrent->child_.pair_.pRight_;
        }
        else {
            found = 1;
        }
    }

    switch(searchTarget) {
    case stNotGreater:
        if(!RbTreeNode_isNullOrNil(pCurrent)) {
            break;
        }

    case stLess:
        if(RbTreeNode_isNullOrNil(pCurrent)) {
            pCurrent = pPrevious;
        }
        
        while(
            !RbTreeNode_isNullOrNil(pCurrent)
            && (*pTreeSet->pComparator_)(pCurrent->pElement_, pElement) >= 0
        ) {
            pCurrent = kphnCollTreeSetNodeFindLess(pCurrent);
        }
    break;
    case stNotLess:
        if(!RbTreeNode_isNullOrNil(pCurrent)) {
            break;
        }

    case stGreater:
        if(RbTreeNode_isNullOrNil(pCurrent)) {
            pCurrent = pPrevious;
        }
        
        while(
            !RbTreeNode_isNullOrNil(pCurrent)
            && (*pTreeSet->pComparator_)(pElement, pCurrent->pElement_) >= 0
        ) {
            pCurrent = kphnCollTreeSetNodeFindGreater(pCurrent);
        }
    break;
    case stEqual:
    default:
        pCurrent = (
            (pCurrent != NULL && !RbTreeNode_isNil(pCurrent))
            ? pCurrent
            : NULL
        );
    }

    return pCurrent;
}

static void TreeSet_rebalanceAfterInsertion(
    struct kphnCollTreeSet* pTreeSet
    , struct kphnCollTreeSetNode* pTarget
)
{
    struct kphnCollTreeSetNode* pParent;
    struct kphnCollTreeSetNode* pUncle;
    struct kphnCollTreeSetNode* pGrandParent;
    struct kphnCollTreeSetNode* pCurrent = pTarget;

    while(pCurrent != NULL) {
        pParent = pCurrent->pParent_;
        if(pParent == NULL) {
            pCurrent->red_ = 0;

            pCurrent = NULL;
        }
        else if(pParent->red_) {
            pGrandParent = pParent->pParent_;
            pUncle = RbTreeNode_getOtherChild(pGrandParent, pParent);
            if(pUncle != NULL && pUncle->red_) {
                pUncle->red_ = 0;
                pParent->red_ = 0;
                
                pCurrent = pGrandParent;
                if(pCurrent != NULL) {
                    pCurrent->red_ = 1;
                }
            }
            else {
                if(
                    pCurrent == pParent->child_.pair_.pRight_
                    && pParent == pGrandParent->child_.pair_.pLeft_
                ) {
                    RbTreeNode_rotate(pParent, rdLeft);
                    pCurrent = pCurrent->child_.pair_.pLeft_;
                }
                else if(
                    pCurrent == pParent->child_.pair_.pLeft_
                    && pParent == pGrandParent->child_.pair_.pRight_
                ) {
                    RbTreeNode_rotate(pParent, rdRight);
                    pCurrent = pCurrent->child_.pair_.pRight_;
                }
                
                pParent = pCurrent->pParent_;
                pParent->red_ = 0;
                pGrandParent->red_ = 1;
                RbTreeNode_rotate(
                    pGrandParent,
                    (pCurrent == pParent->child_.pair_.pLeft_)
                );

                pCurrent = pGrandParent;
                pParent = pGrandParent->pParent_;
                if(pParent->pParent_ == NULL) {
                    pTreeSet->pRoot_ = pParent;
                }

                pCurrent = NULL;
            }
        }
        else {
            pCurrent = NULL;
        }
    }
}

static void TreeSet_rebalanceAfterRemoval(
    struct kphnCollTreeSet* pTreeSet,
    struct kphnCollTreeSetNode* pReplacement
)
{
    struct kphnCollTreeSetNode* pSibling;
    struct kphnCollTreeSetNode* pParent;
    struct kphnCollTreeSetNode* pCurrent = pReplacement;
    
    while(pCurrent != NULL) {
        pParent = pCurrent->pParent_;
        if(pParent == NULL) {
            pTreeSet->pRoot_ = pCurrent;

            pCurrent = NULL;
        }
        else {
            pSibling = RbTreeNode_getSibling(pCurrent);
            if(pSibling->red_) {
                pParent->red_ = 1;
                pSibling->red_ = 0;

                RbTreeNode_rotate(
                    pParent,
                    (pCurrent != pParent->child_.pair_.pLeft_)
                );
                if(pSibling->pParent_ == NULL) {
                    pTreeSet->pRoot_ = pSibling;
                }

                pSibling = RbTreeNode_getSibling(pCurrent);
            }

            if(!pSibling->red_) {
                const int caseNumber = (pSibling->child_.pair_.pRight_->red_ ? 0x02 : 0x00)
                    | (pSibling->child_.pair_.pLeft_->red_ ? 0x01 : 0x00)
                ;

                if(caseNumber == 0) {
                    if(!pParent->red_) {
                        pSibling->red_ = 1;

                        pCurrent = pParent;
                    }
                    else {
                        pSibling->red_ = 1;
                        pParent->red_ = 0;

                        pCurrent = NULL;
                    }
                }
                else {
                    switch(caseNumber) {
                    case 0:
                    break;
                    case 1:
                        if(pCurrent == pParent->child_.pair_.pLeft_) {
                            pSibling->red_ = 1;
                            pSibling->child_.pair_.pLeft_->red_ = 0;
                            RbTreeNode_rotate(pSibling, rdRight);

                            pSibling = RbTreeNode_getSibling(pCurrent);
                        }
                    break;
                    case 2:
                        if(pCurrent == pParent->child_.pair_.pRight_) {
                            pSibling->red_ = 1;
                            pSibling->child_.pair_.pRight_->red_ = 0;
                            RbTreeNode_rotate(pSibling, rdLeft);
                            
                            pSibling = RbTreeNode_getSibling(pCurrent);
                        }
                    break;
                    case 3:
                    break;
                    }

                    if(!pSibling->red_) {
                        const int childNdx = (pCurrent != pParent->child_.pair_.pLeft_ ? 1 : 0);

                        pSibling->red_ = pParent->red_;
                        pParent->red_ = 0;
                        
                        pSibling->child_.ptrs_[~childNdx & 0x01]->red_ = 0;
                        RbTreeNode_rotate(pParent, childNdx);
                        if(pSibling->pParent_ == NULL) {
                            pTreeSet->pRoot_ = pSibling;
                        }
                    }

                    pCurrent = NULL;
                }
            }
            else {
                pCurrent = NULL;
            }
        }
    }
}

int kphnCollTreeSetConstruct(
    struct kphnCollTreeSet* pTreeSet
    , kaphein_ComparatorPtr pComparator
)
{
    if(pTreeSet == NULL) {
        return kphnEcNullArgument;
    }

    if(pComparator == NULL) {
        return kphnEcNullArgument;
    }

    pTreeSet->pComparator_ = pComparator;
    pTreeSet->pRoot_ = NULL;
    pTreeSet->count_ = 0;

    return kphnEcNoError;
}

void kphnCollTreeSetDestruct(
    struct kphnCollTreeSet* pTreeSet
)
{
    kphnCollTreeSetClear(pTreeSet);
}

kphnSizeType kphnCollTreeSetGetCount(
    const struct kphnCollTreeSet* pTreeSet
)
{
    return pTreeSet->count_;
}

bool kphnCollTreeSetHas(
    const struct kphnCollTreeSet* pTreeSet
    , const void* pElement
)
{
    return TreeSet_findNode(pTreeSet, pElement, stEqual) != NULL;
}

struct kphnCollTreeSetNode* kphnCollTreeSetAdd(
    struct kphnCollTreeSet* pTreeSet
    , void* pElement
)
{
    struct kphnCollTreeSetNode* newNode = NULL;
    struct kphnCollTreeSetNode* pCurrent;
    int cmpResult;

    if(pTreeSet->pRoot_ == NULL) {
        newNode = TreeSet_createNode(NULL, pElement);
        pTreeSet->pRoot_ = newNode;
    }
    else for(
        pCurrent = pTreeSet->pRoot_;
        !RbTreeNode_isNil(pCurrent);
    ) {
        cmpResult = (*pTreeSet->pComparator_)(pElement, pCurrent->pElement_);
        if(cmpResult < 0) {
            if(RbTreeNode_isNil(pCurrent->child_.pair_.pLeft_)) {
                newNode = TreeSet_createNode(pCurrent, pElement);
                pCurrent->child_.pair_.pLeft_ = newNode;
                
                pCurrent = (struct kphnCollTreeSetNode*)&nilNode;
            }
            else {
                pCurrent = pCurrent->child_.pair_.pLeft_;
            }
        }
        else if(cmpResult > 0) {
            if(RbTreeNode_isNil(pCurrent->child_.pair_.pRight_)) {
                newNode = TreeSet_createNode(pCurrent, pElement);
                pCurrent->child_.pair_.pRight_ = newNode;
                
                pCurrent = (struct kphnCollTreeSetNode*)&nilNode;
            }
            else {
                pCurrent = pCurrent->child_.pair_.pRight_;
            }
        }
        else {
            pCurrent = (struct kphnCollTreeSetNode*)&nilNode;
        }
    }
    
    if(newNode != NULL) {
        TreeSet_rebalanceAfterInsertion(pTreeSet, newNode);
        ++pTreeSet->count_;

        pTreeSet->pRoot_ = RbTreeNode_getRoot(pTreeSet->pRoot_);
    }

    return newNode;
}

struct kphnCollTreeSetNode* kphnCollTreeSetRemove(
    struct kphnCollTreeSet* pTreeSet
    , void* pElement
)
{
    struct kphnCollTreeSetNode** pChildSlot;
    struct kphnCollTreeSetNode* pMaxOfLeftSubTree;
    struct kphnCollTreeSetNode* pChildOfRemoved = NULL;
    struct kphnCollTreeSetNode* removed = TreeSet_findNode(pTreeSet, pElement, stEqual);
    int loop = 1;
    int removedIsRed;

    if(removed != NULL) {
        while(loop) {
            const int caseNumber = (RbTreeNode_isNullOrNil(removed->child_.pair_.pRight_) ? 0x01 : 0x00)
                | (RbTreeNode_isNullOrNil(removed->child_.pair_.pLeft_) ? 0x02 : 0x00)
            ;
            switch(caseNumber) {
            case 0: //Has two non-nil children.
                pMaxOfLeftSubTree = kphnCollTreeSetNodeFindRightMost(removed->child_.pair_.pLeft_);
                removed->pElement_ = pMaxOfLeftSubTree->pElement_;
                removed = pMaxOfLeftSubTree;
            break;
            case 1: //Has left non-nil child.
            case 2: //Has right non-nil child.
                pChildOfRemoved = removed->child_.ptrs_[caseNumber - 1];
                pChildOfRemoved->pParent_ = removed->pParent_;

                pChildSlot = RbTreeNode_getChildSlot(removed);
                if(pChildSlot != NULL) {
                    *pChildSlot = pChildOfRemoved;
                }
                
                if(!removed->red_) {
                    if(pChildOfRemoved->red_) {
                        pChildOfRemoved->red_ = 0;

                        pTreeSet->pRoot_ = RbTreeNode_getRoot(pChildOfRemoved);
                    }
                    else {
                        TreeSet_rebalanceAfterRemoval(pTreeSet, pChildOfRemoved);
                    }
                }
                else {
                    pTreeSet->pRoot_ = RbTreeNode_getRoot(pChildOfRemoved);
                }

                loop = 0;
            break;
            case 3: //Has no non-nil children.
                removedIsRed = removed->red_;
                if(!removedIsRed) {
                    TreeSet_rebalanceAfterRemoval(pTreeSet, removed);
                }
                
                pChildSlot = RbTreeNode_getChildSlot(removed);
                if(pChildSlot != NULL) {
                    *pChildSlot = (struct kphnCollTreeSetNode*)&nilNode;
                    pTreeSet->pRoot_ = RbTreeNode_getRoot(removed->pParent_);
                }
                else {
                    pTreeSet->pRoot_ = NULL;
                }

                loop = 0;
            break;
            }
        }

        free(removed);

        --pTreeSet->count_;
    }
    
    return NULL;
}

static bool kphnCollTreeSetRemoveAllHandler(
    void* pContext
    , struct kphnCollTreeSetNode* pNode
)
{
    kphnCollListStackPush(
        (struct kphnCollListStack*)pContext
        , pNode
    );

    return false;
}

void kphnCollTreeSetClear(
    struct kphnCollTreeSet* pTreeSet
)
{
    void* pNode;
    struct kphnCollListStack nodeStack;
    
    if(pTreeSet->pRoot_ != NULL) {
        kphnCollListStackConstruct(&nodeStack);

        RbTreeNode_traverseByPostorder(
            pTreeSet->pRoot_
            , kphnCollTreeSetRemoveAllHandler
            , (void*)&nodeStack
        );

        while(!kphnCollListStackIsEmpty(&nodeStack)) {
            kphnCollListStackPop(&nodeStack, &pNode);
            free(pNode);
        }

        kphnCollListStackDestruct(&nodeStack);

        pTreeSet->pRoot_ = NULL;
        pTreeSet->count_ = 0;
    }

    #ifdef _DEBUG
    if(pTreeSet->pRoot_ == NULL && pTreeSet->count_ > 0) {
        system("pause");
    }
    #endif
}
