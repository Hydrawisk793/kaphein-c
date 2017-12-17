#include <stdlib.h>
#include <string.h>
#include "kaphein/coll/func.h"

#define KAPHEIN_x_BEGIN_SORT_FUNCTION_BODY \
    if(pElements == NULL) { \
        return -1; \
    } \
    if(pComparator == NULL) { \
        return -2; \
    } \
    if(elementSize < 1) { \
        return -3; \
    } \
    pTemp = malloc(elementSize); \
    if(pTemp == NULL) { \
        return -4; \
    }

#define KAPHEIN_x_END_SORT_FUNCTION_BODY \
    free(pTemp); \
    return 0;

static void swapTrivials(
    size_t elemSize
    , void* pTemp
    , void* pLhs
    , void* pRhs
) {
    memcpy(pTemp, pRhs, elemSize);
    memcpy(pRhs, pLhs, elemSize);
    memcpy(pLhs, pTemp, elemSize);
}

int kphnCollSelectionSort(
    void* pElements
    , size_t elementCount
    , size_t elementSize
    , kaphein_ComparatorPtr pComparator
) {
    char* pLhs = (char*)pElements;
    char* pRhs = NULL;
    char* pMin = NULL;
    char* pTemp = NULL;
    size_t i, j;

    KAPHEIN_x_BEGIN_SORT_FUNCTION_BODY

    for(i = elementCount; i > 0; pLhs += elementSize) {
        --i;

        pMin = pLhs;
        pRhs = pLhs + elementSize;

        for(j = i; j > 0; pRhs += elementSize) {
            --j;

            if((*pComparator)(pRhs, pMin) < 0) {
                pMin = pRhs;
            }
        }

        if(pLhs != pMin) {
            swapTrivials(elementSize, pTemp, pLhs, pMin);
        }
    }

    KAPHEIN_x_END_SORT_FUNCTION_BODY
}

int kphnCollBubbleSort(
    void* pElements
    , size_t elementCount
    , size_t elementSize
    , kaphein_ComparatorPtr pComparator
) {
    char* pTemp = NULL;
    char* const pLhsStart = (char*)pElements;
    char* pLhs = NULL;
    char* pRhs = NULL;
    size_t i, j;

    KAPHEIN_x_BEGIN_SORT_FUNCTION_BODY

    for(i = elementCount; i > 1; ) {
        --i;
        
        pLhs = pLhsStart;
        pRhs = pLhs + elementSize;

        for(j = i; j > 0; pRhs += elementSize, pLhs += elementSize) {
            --j;

            if((*pComparator)(pRhs, pLhs) < 0) {
                swapTrivials(elementSize, pTemp, pLhs, pRhs);
            }
        }
    }

    KAPHEIN_x_END_SORT_FUNCTION_BODY
}

int kphnCollInsertionSort(
    void* pElements
    , size_t elementCount
    , size_t elementSize
    , kaphein_ComparatorPtr pComparator
) {
    char* pRhs = (char*)pElements + elementSize;
    char* pLhs = NULL;
    char* pTemp = NULL;
    size_t i, j, jMax;
    
    KAPHEIN_x_BEGIN_SORT_FUNCTION_BODY
    
    for(i = elementCount, jMax = 1; i > 1; pRhs += elementSize, ++jMax) {
        --i;

        memcpy(pTemp, pRhs, elementSize);
        pLhs = pRhs;
        for(j = jMax; j > 0; ) {
            --j;
            
            pLhs -= elementSize;
            if((*pComparator)(pTemp, pLhs) < 0) {
                memcpy(pLhs + elementSize, pLhs, elementSize);
            }
            else {
                pLhs += elementSize;
                j = 0;
            }
        }
        
        if(pLhs != pRhs) {
            memcpy(pLhs, pTemp, elementSize);
        }
    }

    KAPHEIN_x_END_SORT_FUNCTION_BODY
}

#undef KAPHEIN_x_BEGIN_SORT_FUNCTION_BODY
#undef KAPHEIN_x_END_SORT_FUNCTION_BODY
