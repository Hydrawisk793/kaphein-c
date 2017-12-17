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

#ifndef KAPHEIN_NCOD_UTF_H
#define KAPHEIN_NCOD_UTF_H

#include "kaphein/c/def.h"

KAPHEIN_PP_C_LINKAGE_BEGIN()

/**
 *  @brief Converts an UCS-4 string to an UTF-8 encoded string.
 *  @since 2017-05-13
 *  @version 2017-12-10
 */
KAPHEIN_PP_DLL_API
int kphnNcodUcs4ToUtf8(
    const uint32_t* utf32String
    , size_t utf32StringLength
    , uint8_t* pBufferOut
    , size_t outBufferSize
    , size_t* pOutputByteCountOut
);

/**
 *  @brief Converts an UTF-8 encoded string to an UCS-4 string.
 *  @since 2017-05-13
 *  @version 2017-12-10
 */
KAPHEIN_PP_DLL_API
int kphnNcodUtf8ToUcs4(
    const uint8_t* utf8String
    , size_t utf8StringLength
    , uint32_t* pOutBuffer
    , size_t outBufferSize
    , size_t* pCharCountOut
);

/**
 *  @brief Converts an UCS-4 string to an UTF-16 encoded string.
 *  @since 2017-05-13
 *  @version 2017-12-10
 */
KAPHEIN_PP_DLL_API
int kphnNcodUcs4ToUtf16(
    const uint32_t* str
    , size_t strLen
    , uint16_t* bufferOut
    , size_t bufferSize
    , size_t* outputElementCountOut
);

/**
 *  @brief Converts an UTF-16 encoded string to an UCS-4 string.
 *  @since 2017-05-13
 *  @version 2017-12-10
 */
KAPHEIN_PP_DLL_API
int kphnNcodUtf16ToUcs4(
    const uint16_t* utf16String
    , size_t utf16StringLength
    , uint32_t* pOutBuffer
    , size_t outBufferSize
    , size_t* outputCharCountOut
);

int kphnNcodUtf16ToUtf8(
    const uint16_t* utf16String
    , size_t utf16StringLength
    , uint8_t* pOutBuffer
    , size_t outBufferSize
    , size_t* byteCountOut
);

int kphnNcodUtf8ToUtf16(
    const uint8_t* utf8String
    , size_t utf8StringLength
    , uint16_t* pOutBuffer
    , size_t outBufferSize
    , size_t* elementCountOut
);

KAPHEIN_PP_C_LINKAGE_END()

#endif
