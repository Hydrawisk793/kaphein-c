#include <stdlib.h>
#include <string.h>
#include "karbonator/collection/algorithm.h"

#define KARBONATOR_x_BEGIN_SORT_FUNCTION_BODY \
    if(pElements == NULL) { \
        return -1; \
    } \
    if(pComparator == NULL) { \
        return -2; \
    } \
    if(elementSize < 1) { \
        return -3; \
    } \
    char* const pTemp = malloc(elementSize); \
    if(pTemp == NULL) { \
        return -4; \
    }
#define KARBONATOR_x_END_SORT_FUNCTION_BODY \
    free(pTemp); \
    return 0;

static void swapTrivials(
    size_t elemSize,
    void* pTemp,
    void* pLhs,
    void* pRhs
) {
    memcpy(pTemp, pRhs, elemSize);
    memcpy(pRhs, pLhs, elemSize);
    memcpy(pLhs, pTemp, elemSize);
}

int selectionSort(
    void* pElements,
    size_t elementCount,
    size_t elementSize,
    ComparatorPtr pComparator
) {
    KARBONATOR_x_BEGIN_SORT_FUNCTION_BODY

    char* pLhs = (char*)pElements;
    char* pRhs = NULL;
    char* pMin = NULL;
    size_t i, j;

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

    KARBONATOR_x_END_SORT_FUNCTION_BODY
}

int bubbleSort(
    void* pElements,
    size_t elementCount,
    size_t elementSize,
    ComparatorPtr pComparator
) {
    KARBONATOR_x_BEGIN_SORT_FUNCTION_BODY

    char* const pLhsStart = (char*)pElements;
    char* pLhs = NULL;
    char* pRhs = NULL;
    size_t i, j;

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

    KARBONATOR_x_END_SORT_FUNCTION_BODY
}

int insertionSort(
    void* pElements,
    size_t elementCount,
    size_t elementSize,
    ComparatorPtr pComparator
) {
    KARBONATOR_x_BEGIN_SORT_FUNCTION_BODY

    char* pRhs = (char*)pElements + elementSize;
    char* pLhs = NULL;
    size_t i, j, jMax;

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

    KARBONATOR_x_END_SORT_FUNCTION_BODY
}

#undef KARBONATOR_x_BEGIN_SORT_FUNCTION_BODY
#undef KARBONATOR_x_END_SORT_FUNCTION_BODY
