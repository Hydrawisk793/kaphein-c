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

#ifndef KAPHEIN_MEM_VSCPOOL_H
#define KAPHEIN_MEM_VSCPOOL_H

#include "kaphein/c/def.h"

/**
 *  @brief A struct for representing variable-size chunk header.
 *  @deial All members of this struct are considered INVISIBLE.
 *      <br/> You should NOT rely on these members.
 *  @since 2016-06-18
 */
struct kphnMemVscHeader
{
    union
    {
        struct
        {
            struct kphnMemVscHeader* pPrev_;
            
            struct kphnMemVscHeader* pNext_;
            
            size_t byteCount_;
            
            char inUse_;
        } f_;

        char bytes [sizeof(void*) << 2];
    } m_;
};

/**
 *  @brief A struct for representing memory pools that hold variable-size chunks.
 *  @deial All members of this struct are considered INVISIBLE.
 *      <br/> You should NOT rely on these members.
 *  @since 2016-06-18
 */
struct kphnMemVscPool
{
    struct kphnMemVscHeader* pHead_;

    struct kphnMemVscHeader* pCurrent_;

    size_t blockSize_;
};

/**
 *  @breif Constructs a variable-size chunk pool.
 *  @param pPool
 *  @param blockSize
 *  @return An error code returned by this function.
 *  @since 2016-06-18
 */
KAPHEIN_PP_DLL_API
int kphnMemVscPoolConstruct(
    struct kphnMemVscPool* pPool
    , size_t blockSize
);

/**
 *  @breif Destructs specified variable-size chunk pool.
 *  @param pPool
 *  @details Before destructing the pool, You MUST free ALL memories allocated by the pool.
 *  @since 2016-06-18
 */
KAPHEIN_PP_DLL_API
void kphnMemVscPoolDestruct(
    struct kphnMemVscPool* pPool
);

/**
 *  @brief 
 *  @param pPool
 *  @param byteCount
 *  @return A pointer to the allocated memory block if succeeded, NULL otherwise.
 *  @since 2016-06-18
 */
KAPHEIN_PP_DLL_API
void* kphnMemVscPoolAllocate(
    struct kphnMemVscPool* pPool
    , size_t byteCount
);

/**
 *  @brief 
 *  @param pPool
 *  @param pChunk
 *  @return An error code returned by this function.
 *  @since 2016-06-18
 */
KAPHEIN_PP_DLL_API
int kphnMemVscPoolFree(
    struct kphnMemVscPool* pPool
    , void* pChunk
);

KAPHEIN_PP_C_LINKAGE_END()

#endif
