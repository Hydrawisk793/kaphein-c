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

#ifndef KAPHEIN_MEM_FWD_H
#define KAPHEIN_MEM_FWD_H

#include "kaphein/def.h"

KAPHEIN_PP_C_LINKAGE_BEGIN()

/**
 *  @brief A struct that contains allocator information.
 *  @since 2017-12-30
 */
struct kphnMemAllocator
{
    /**
     *  @brief A pointer to allocate function of an allocator.
     *  <br/>Allocates a memory block with specified allocator and size of the block.
     *  @param pAllocator A pointer to an allocator struct.
     *  @param byteCount The size of memory block to be allocated.
     *  @return A non-null pointer to the allocated memory block, null pointer otherwise.
     */
    void* (*allocate) (
        void* pAllocator
        , kphnSizeType byteCount
    );
    
    /**
     *  @brief A pointer to deallocate function of an allocator.
     *  <br/>Deallocates the memory block allocated by specified allocator.
     *  @param pAllocator A pointer to an allocator struct.
     *  @param pMem The memory block allocated by the specified allocator.
     *  @param byteCount The size of the memory block.
     *  <br/>This must be equal to the exact size of the memory block. If not so, the behaviour is undefined.
     *  @return An error code returned by this function.
     */
    int (*deallocate) (
        void* pAllocator
        , void* pMem
        , kphnSizeType byteCount
    );
    
    /**
     *  @brief A pointer to an allocator struct.
     */
    void* pAllocator;
};

KAPHEIN_PP_C_LINKAGE_END()

#endif
