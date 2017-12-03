#ifndef KARBONATOR_COLLECTION_ALGORITHM_H
#define KARBONATOR_COLLECTION_ALGORITHM_H

#include <stddef.h>
#include "karbonator/pp/basic.h"
#include "fwd.h"

KARBONATOR_PP_C_LINKAGE_BEGIN()

KARBONATOR_PP_DLL_API
int selectionSort(
    void* pElements,
    size_t elementCount,
    size_t elementSize,
    ComparatorPtr pComparator
);

KARBONATOR_PP_DLL_API
int bubbleSort(
    void* pElements,
    size_t elementCount,
    size_t elementSize,
    ComparatorPtr pComparator
);

KARBONATOR_PP_DLL_API
int insertionSort(
    void* pElements,
    size_t elementCount,
    size_t elementSize,
    ComparatorPtr pComparator
);

KARBONATOR_PP_C_LINKAGE_END()

#endif
