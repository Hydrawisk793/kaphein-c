#ifdef _DEBUG
    #include <stdio.h>
#endif
#include <stdlib.h>
#include <string.h>
#include "karbonator/collection/ListStack.h"
#include "karbonator/collection/TreeSet.h"

typedef bool (*NodeTraversalHandlerPtr) (void*, RbTreeNode*);

typedef enum RotationDirection_ {
    rdLeft = 0,
    rdRight = 1
} RotationDirection;

static INLINE bool RbTreeNode_isNil(
    const RbTreeNode* pNode
) {
    return pNode->pElement_ == NULL;
}

static INLINE bool RbTreeNode_isNullOrNil(
    const RbTreeNode* pNode
) {
    return pNode == NULL || RbTreeNode_isNil(pNode);
}

static INLINE bool RbTreeNode_hasNonNilLeftChild(
    const RbTreeNode* pNode
) {
    return pNode->child_.pair_.pLeft_ != NULL
        && !RbTreeNode_isNil(pNode->child_.pair_.pLeft_)
    ;
}

static INLINE bool RbTreeNode_hasNonNilRightChild(
    const RbTreeNode* pNode
) {
    return pNode->child_.pair_.pRight_ != NULL
        && !RbTreeNode_isNil(pNode->child_.pair_.pRight_)
    ;
}

static INLINE bool RbTreeNode_isNonNilLeaf(
    const RbTreeNode* pNode
) {
    return !RbTreeNode_hasNonNilLeftChild(pNode)
        && !RbTreeNode_hasNonNilRightChild(pNode)
    ;
}

static RbTreeNode* RbTreeNode_getRoot(
    RbTreeNode* pNode
) {
    RbTreeNode* pRoot = pNode;
    for(; pRoot->pParent_ != NULL; pRoot = pRoot->pParent_);

    return pRoot;
}

static INLINE RbTreeNode* RbTreeNode_getOtherChild(
    RbTreeNode* pParent,
    RbTreeNode* pNode
) {
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

static INLINE RbTreeNode* RbTreeNode_getLastChild(
    RbTreeNode * pNode
) {
    return (
        !RbTreeNode_isNullOrNil(pNode->child_.pair_.pRight_)
        ? pNode->child_.pair_.pRight_
        : pNode->child_.pair_.pLeft_
    );
}

static INLINE RbTreeNode* RbTreeNode_getSibling(
    RbTreeNode * pNode
) {
    return RbTreeNode_getOtherChild(pNode->pParent_, pNode);
}

static INLINE RbTreeNode** RbTreeNode_getChildSlot(
    RbTreeNode* pNode
) {
    RbTreeNode* const pParent = pNode->pParent_;
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

RbTreeNode* RbTreeNode_findLeftMost(
    RbTreeNode* pNode
) {
    RbTreeNode* pCurrent = pNode;
    for(
        ;
        pCurrent != NULL && RbTreeNode_hasNonNilLeftChild(pCurrent);
        pCurrent = pCurrent->child_.pair_.pLeft_
    );

    return pCurrent;
}

RbTreeNode* RbTreeNode_findRightMost(
    RbTreeNode* pNode
) {
    RbTreeNode* pCurrent = pNode;
    for(
        ;
        pCurrent != NULL && RbTreeNode_hasNonNilRightChild(pCurrent);
        pCurrent = pCurrent->child_.pair_.pRight_
    );

    return pCurrent;
}

static RbTreeNode* RbTreeNode_findLeftSubTreeRoot(
    RbTreeNode* pNode
) {
    RbTreeNode* pCurrent = pNode;
    RbTreeNode* pParent;
    while(pCurrent != NULL) {
        pParent = pCurrent->pParent_;
        if(pParent == NULL || pCurrent == pParent->child_.pair_.pLeft_) {
            break;
        }

        pCurrent = pParent;
    }

    return pCurrent;
}

static RbTreeNode* RbTreeNode_findRightSubTreeRoot(
    RbTreeNode* pNode
) {
    RbTreeNode* pCurrent = pNode;
    RbTreeNode* pParent;
    while(pCurrent != NULL) {
        pParent = pCurrent->pParent_;
        if(pParent == NULL || pCurrent == pParent->child_.pair_.pRight_) {
            break;
        }

        pCurrent = pParent;
    }

    return pCurrent;
}

RbTreeNode* RbTreeNode_findLess(
    RbTreeNode* pNode
) {
    RbTreeNode* pLess = NULL;
    RbTreeNode* pRstRoot = NULL;
    
    if(RbTreeNode_hasNonNilLeftChild(pNode)) {
        pLess = RbTreeNode_findRightMost(pNode->child_.pair_.pLeft_);
    }
    else {
        pRstRoot = RbTreeNode_findRightSubTreeRoot(pNode);
        if(pRstRoot != NULL) {
            pLess = pRstRoot->pParent_;
        }
    }

    return pLess;
}

RbTreeNode* RbTreeNode_findGreater(
    RbTreeNode* pNode
) {
    RbTreeNode* pGreater = NULL;

    if(!RbTreeNode_isNil(pNode->child_.pair_.pRight_)) {
        pGreater = RbTreeNode_findLeftMost(pNode->child_.pair_.pRight_);
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
    RbTreeNode* pNode,
    NodeTraversalHandlerPtr pHandler,
    void* pContext
) {
    RbTreeNode* pCurrent;
    RbTreeNode* pLastTraversed = NULL;
    karbonator_collection_ListStack nodeStack;
    bool stopTraversal = false;
    int childCount;
    void* pPopedNode;

    karbonator_collection_ListStack_construct(&nodeStack);
    karbonator_collection_ListStack_push(&nodeStack, pNode);

    while(!stopTraversal && !karbonator_collection_ListStack_isEmpty(&nodeStack)) {
        karbonator_collection_ListStack_peek(&nodeStack, &pPopedNode);
        pCurrent = (RbTreeNode*)pPopedNode;
        
        childCount = 0;
        if(RbTreeNode_getLastChild(pCurrent) != pLastTraversed) {
            if(!RbTreeNode_isNullOrNil(pCurrent->child_.pair_.pRight_)) {
                karbonator_collection_ListStack_push(&nodeStack, pCurrent->child_.pair_.pRight_);
                ++childCount;
            }
            if(!RbTreeNode_isNullOrNil(pCurrent->child_.pair_.pLeft_)) {
                karbonator_collection_ListStack_push(&nodeStack, pCurrent->child_.pair_.pLeft_);
                ++childCount;
            }
        }

        if(childCount < 1) {
            stopTraversal = (*pHandler)(pContext, pCurrent);
            pLastTraversed = pCurrent;
            karbonator_collection_ListStack_pop(&nodeStack, &pPopedNode);
        }
    }

    karbonator_collection_ListStack_destruct(&nodeStack);

    return stopTraversal;
}

static void RbTreeNode_rotate(
    RbTreeNode* pNode,
    RotationDirection direction
) {
    const int dirNdx = direction & 0x01;
    const int otherDirNdx = ~dirNdx & 0x01;
    RbTreeNode** pChildSlot = RbTreeNode_getChildSlot(pNode);
    RbTreeNode* const pParent = pNode->pParent_;
    RbTreeNode* const pLeftChildOfRightChild = pNode->child_.ptrs_[otherDirNdx]->child_.ptrs_[dirNdx];

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

static const RbTreeNode nilNode = {
    NULL,
    NULL,
    NULL, NULL,
    0
};

static RbTreeNode* TreeSet_createNode(
    RbTreeNode* pParent,
    void* pElement
) {
    RbTreeNode* const newNode = (RbTreeNode*)malloc(sizeof(RbTreeNode));
    if(newNode != NULL) {
        newNode->pElement_ = pElement;
        newNode->pParent_ = pParent;
        newNode->child_.pair_.pLeft_ = (RbTreeNode*)&nilNode;
        newNode->child_.pair_.pRight_ = (RbTreeNode*)&nilNode;
        newNode->red_ = 1;
    }

    return newNode;
}

static RbTreeNode* TreeSet_findNode(
    const TreeSet* pTreeSet,
    const void* pElement,
    SearchTarget searchTarget
) {
    RbTreeNode* pPrevious = NULL;
    RbTreeNode* pCurrent = pTreeSet->pRoot_;
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
            pCurrent = RbTreeNode_findLess(pCurrent);
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
            pCurrent = RbTreeNode_findGreater(pCurrent);
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
    TreeSet* pTreeSet,
    RbTreeNode* pTarget
) {
    RbTreeNode* pParent;
    RbTreeNode* pUncle;
    RbTreeNode* pGrandParent;
    RbTreeNode* pCurrent = pTarget;

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
                    (RotationDirection)(pCurrent == pParent->child_.pair_.pLeft_)
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
    TreeSet* pTreeSet,
    RbTreeNode* pReplacement
) {
    RbTreeNode* pSibling;
    RbTreeNode* pParent;
    RbTreeNode* pCurrent = pReplacement;
    
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
                    (RotationDirection)(pCurrent != pParent->child_.pair_.pLeft_)
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
                        RbTreeNode_rotate(pParent, (RotationDirection)childNdx);
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

karbonator_ErrorCode TreeSet_construct(
    TreeSet* pTreeSet,
    ComparatorPtr pComparator
) {
    if(pTreeSet == NULL) {
        return krbntrEcNullArgument;
    }

    if(pComparator == NULL) {
        return krbntrEcNullArgument;
    }

    pTreeSet->pComparator_ = pComparator;
    pTreeSet->pRoot_ = NULL;
    pTreeSet->count_ = 0;

    return krbntrEcNoError;
}

void TreeSet_destruct(
    TreeSet* pTreeSet
) {
    TreeSet_clear(pTreeSet);
}

size_t TreeSet_getCount(
    const TreeSet* pTreeSet
) {
    return pTreeSet->count_;
}

bool TreeSet_has(
    const TreeSet* pTreeSet,
    const void* pElement
) {
    return TreeSet_findNode(pTreeSet, pElement, stEqual) != NULL;
}

RbTreeNode* TreeSet_add(
    TreeSet* pTreeSet,
    void* pElement
) {
    RbTreeNode* newNode = NULL;
    RbTreeNode* pCurrent;
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
                
                pCurrent = (RbTreeNode*)&nilNode;
            }
            else {
                pCurrent = pCurrent->child_.pair_.pLeft_;
            }
        }
        else if(cmpResult > 0) {
            if(RbTreeNode_isNil(pCurrent->child_.pair_.pRight_)) {
                newNode = TreeSet_createNode(pCurrent, pElement);
                pCurrent->child_.pair_.pRight_ = newNode;
                
                pCurrent = (RbTreeNode*)&nilNode;
            }
            else {
                pCurrent = pCurrent->child_.pair_.pRight_;
            }
        }
        else {
            pCurrent = (RbTreeNode*)&nilNode;
        }
    }
    
    if(newNode != NULL) {
        TreeSet_rebalanceAfterInsertion(pTreeSet, newNode);
        ++pTreeSet->count_;

        pTreeSet->pRoot_ = RbTreeNode_getRoot(pTreeSet->pRoot_);
    }

    return newNode;
}

RbTreeNode* TreeSet_remove(
    TreeSet* pTreeSet,
    void* pElement
) {
    RbTreeNode** pChildSlot;
    RbTreeNode* pMaxOfLeftSubTree;
    RbTreeNode* pChildOfRemoved = NULL;
    RbTreeNode* removed = TreeSet_findNode(pTreeSet, pElement, stEqual);
    int loop = 1;
    int removedIsRed;

    if(removed != NULL) {
        while(loop) {
            const int caseNumber = (RbTreeNode_isNullOrNil(removed->child_.pair_.pRight_) ? 0x01 : 0x00)
                | (RbTreeNode_isNullOrNil(removed->child_.pair_.pLeft_) ? 0x02 : 0x00)
            ;
            switch(caseNumber) {
            case 0: //Has two non-nil children.
                pMaxOfLeftSubTree = RbTreeNode_findRightMost(removed->child_.pair_.pLeft_);
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
                    *pChildSlot = (RbTreeNode*)&nilNode;
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

static bool TreeSet_removeAllHandler(
    void* pContext,
    RbTreeNode* pNode
) {
    karbonator_collection_ListStack_push((karbonator_collection_ListStack*)pContext, pNode);

    return false;
}

void TreeSet_clear(
    TreeSet* pTreeSet
) {
    if(pTreeSet->pRoot_ != NULL) {
        void* pNode;
        karbonator_collection_ListStack nodeStack;
        karbonator_collection_ListStack_construct(&nodeStack);

        RbTreeNode_traverseByPostorder(
            pTreeSet->pRoot_,
            TreeSet_removeAllHandler,
            (void*)&nodeStack
        );

        while(!karbonator_collection_ListStack_isEmpty(&nodeStack)) {
            karbonator_collection_ListStack_pop(&nodeStack, &pNode);
            free(pNode);
        }

        karbonator_collection_ListStack_destruct(&nodeStack);

        pTreeSet->pRoot_ = NULL;
        pTreeSet->count_ = 0;
    }

    #ifdef _DEBUG
    if(pTreeSet->pRoot_ == NULL && pTreeSet->count_ > 0) {
        system("pause");
    }
    #endif
}
