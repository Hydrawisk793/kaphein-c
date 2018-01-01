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

#ifndef KAPHEIN_C_DEF_H
#define KAPHEIN_C_DEF_H

#if (defined(_MSC_VER) && _MSC_VER >= 1800) || defined(__STDC_VERSION__)
    #include <stdbool.h>
#else
    typedef unsigned char _Bool;
    #ifndef __cplusplus
        #ifndef bool
            #define bool _Bool
        #endif
        #ifndef false
            #define false ((_Bool)0)
        #endif
        #ifndef true
            #define true ((_Bool)1)
        #endif
    #endif
#endif

#undef INLINE
#undef FORCEINLINE
#if defined(_MSC_VER)
    #if _MSC_VER < 1700
        #define INLINE __inline
        #define FORCEINLINE __forceinline
    #else
        #define INLINE inline
        #define FORCEINLINE __forceinline
    #endif
#elif defined(__GNUC__)
    #define INLINE inline
    #define FORCEINLINE __attribute__ ((alwaysinline))
#endif

#ifdef __cplusplus
    #define KAPHEIN_PP_C_LINKAGE extern "C"
    #define KAPHEIN_PP_C_LINKAGE_BEGIN() extern "C" {
#else
    #define KAPHEIN_PP_C_LINKAGE
    #define KAPHEIN_PP_C_LINKAGE_BEGIN()
#endif
#ifdef __cplusplus
    #define KAPHEIN_PP_C_LINKAGE_END() }
#else
    #define KAPHEIN_PP_C_LINKAGE_END()
#endif

#if defined(KAPHEIN_EXPORT_DLL)
    #if defined(_MSC_VER)
        #define KAPHEIN_PP_DLL_API __declspec(dllexport)
    #elif defined(__GNUC__)
        #define KAPHEIN_PP_DLL_API __attribute__ ((dllexport))
    #else
        #define KAPHEIN_PP_DLL_API
    #endif
#elif defined (KAPHEIN_DISABLE_EXPORT_DLL)
    #define KAPHEIN_PP_DLL_API
#else
    #if defined(_MSC_VER)
        #define KAPHEIN_PP_DLL_API __declspec(dllimport)
    #elif defined(__GNUC__)
        #define KAPHEIN_PP_DLL_API __attribute__ ((dllimport))
    #else
        #define KAPHEIN_PP_DLL_API
    #endif
#endif

#include <stddef.h>
#include <stdint.h>
#if SIZE_MAX >= UINT64_MAX
    typedef uint64_t kphnSizeType;
    typedef int64_t kphnSSizeType;
#elif SIZE_MAX >= UINT32_MAX
    typedef uint32_t kphnSizeType;
    typedef int32_t kphnSSizeType;
#else
    typedef uint16_t kphnSizeType;
    typedef int16_t kphnSSizeType;
#endif
typedef ptrdiff_t kphnPtrDiffType;

enum kphnErrorCode
{
    kphnEcNoError = 0
    , kphnEcGenericError = 1
    , kphnEcNotSupportedOperation = 2
    
    , kphnEcNullPointer = 20
    , kphnEcContainerUnderflow = 22
    , kphnEcContainerOverflow = 23
    , kphnEcNotEnoughBufferSize = 24
    , kphnEcNotEnoughMemory = 25
    , kphnEcMemoryAllocationFailed = 26
    
    , kphnEcInvalidArgument = 30
    , kphnEcInvalidValue = 31
    , kphnEcNullArgument = 32
    , kphnEcIndexOutOfRange = 33
    , kphnEcValueOverflow = 34

    , kphnEcNotFound = 40
    , kphnEcNoSuchElement = 41
    , kphnEcNoMoreMemoryBlocksLeft = 42

    , kphnEcIoError = 100

    , kphnEcSingularMatrix = 200
};

typedef int (*kaphein_ComparatorPtr)(
    const void* pLhs
    , const void* pRhs
);

typedef int (*kaphein_CopyConstructFuncPtr) (const void* pSrc);

typedef int (*kaphein_DestructFuncPtr) (void* pTarget);

typedef int (*kphnCopyAssignFuncPtr) (void* pLhs, const void* pRhs);

#define KAPHEIN_x_EXPAND(x) x
#define KAPHEIN_PP_x_EXPAND(x) x
#define KAPHEIN_PP_x_VARIADIC_SELECT_LAST_ARGUMENT( \
    _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, \
    _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
    _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, \
    _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, \
    _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, \
    _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, \
    _61, _62, _63, _64, _65, _66, _67, _68, _69, _70, \
    _71, _72, _73, _74, _75, _76, _77, _78, _79, _80, \
    _81, _82, _83, _84, _85, _86, _87, _88, _89, _90, \
    _91, _92, _93, _94, _95, _96, _97, _98, _99, _100, \
    _101, _102, _103, _104, _105, _106, _107, _108, _109, _110, \
    _111, _112, _113, _114, _115, _116, _117, _118, _119, N, ... \
) N
#define KAPHEIN_PP_x_VARIADIC_REVERSED_ARGUMENT_COUNTS() \
    119, 118, 117, 116, 115, 114, 113, 112, 111, 110, \
    109, 108, 107, 106, 105, 104, 103, 102, 101, 100, \
    99, 98, 97, 96, 95, 94, 93, 92, 91, 90, \
    89, 88, 87, 86, 85, 84, 83, 82, 81, 80, \
    79, 78, 77, 76, 75, 74, 73, 72, 71, 70, \
    69, 68, 67, 66, 65, 64, 63, 62, 61, 60, \
    59, 58, 57, 56, 55, 54, 53, 52, 51, 50, \
    49, 48, 47, 46, 45, 44, 43, 42, 41, 40, \
    39, 38, 37, 36, 35, 34, 33, 32, 31, 30, \
    29, 28, 27, 26, 25, 24, 23, 22, 21, 20, \
    19, 18, 17, 16, 15, 14, 13, 12, 11, 10, \
    9, 8, 7, 6, 5, 4, 3, 2, 1, 0
#define KAPHEIN_PP_x_VARIADIC_ARGUMENT_COUNT_EXPAND(...) \
    KAPHEIN_PP_x_EXPAND( \
        KAPHEIN_PP_x_VARIADIC_SELECT_LAST_ARGUMENT(__VA_ARGS__) \
    )
#define KAPHEIN_PP_x_COMMA_REVERSED_SEQUENCE() \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
    1, 1, 1, 1, 1, 1, 1, 1, 0, 0
#define KAPHEIN_PP_x_COMMA(...) ,
#define KAPHEIN_PP_x_HAS_COMMA(...) \
    KAPHEIN_PP_x_VARIADIC_ARGUMENT_COUNT_EXPAND( \
        __VA_ARGS__, \
        KAPHEIN_PP_x_COMMA_REVERSED_SEQUENCE() \
    )
#define KAPHEIN_PP_x_VARIADIC_ARGUMENT_COUNT_BRANCH01(N) 0
#define KAPHEIN_PP_x_VARIADIC_ARGUMENT_COUNT_BRANCH00(N) 1
#define KAPHEIN_PP_x_VARIADIC_ARGUMENT_COUNT_BRANCH11(N) N
#define KAPHEIN_PP_x_VARIADIC_ARGUMENT_COUNT_BRANCH(a, b, N) \
    KAPHEIN_PP_x_VARIADIC_ARGUMENT_COUNT_BRANCH##a##b(N)
#define KAPHEIN_PP_x_VARIADIC_ARGUMENT_COUNT_EXPAND_MACRO_ARGUMENTS(a, b, N) \
    KAPHEIN_PP_x_VARIADIC_ARGUMENT_COUNT_BRANCH(a, b, N)
#define KAPHEIN_PP_x_VARIADIC_ARGUMENT_COUNT(...) \
    KAPHEIN_PP_x_VARIADIC_ARGUMENT_COUNT_EXPAND_MACRO_ARGUMENTS( \
        KAPHEIN_PP_x_HAS_COMMA(__VA_ARGS__), \
        KAPHEIN_PP_x_HAS_COMMA(KAPHEIN_PP_x_COMMA __VA_ARGS__ ()), \
        KAPHEIN_PP_x_VARIADIC_ARGUMENT_COUNT_EXPAND( \
            __VA_ARGS__, \
            KAPHEIN_PP_x_VARIADIC_REVERSED_ARGUMENT_COUNTS() \
        ) \
    )
#define KAPHEIN_PP_GET_ARGUMENT_COUNT(...) \
    KAPHEIN_PP_x_VARIADIC_ARGUMENT_COUNT(__VA_ARGS__)

#endif
