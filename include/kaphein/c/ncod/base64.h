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

#ifndef KAPHEIN_C_NCOD_BASE64_H
#define KAPHEIN_C_NCOD_BASE64_H

#include "../def.h"

KAPHEIN_PP_C_LINKAGE_BEGIN()

/**
 *  @brief Encodes an array of plain bytes and outputs a base64-encoded string.
 *  @param pBytes
 *  @param byteCount
 *  @param pBase64StrOut
 *  @param base64BufferSize
 *  @param pBase64StrLenOut
 *  @return An error code returned by this function.
 *  @since 2017-05-13
 */
KAPHEIN_PP_DLL_API
int kphnNcodBytesToBase64(
    const uint8_t* pBytes
    , kphnSizeType byteCount
    , char* pBase64StrOut
    , kphnSizeType base64BufferSize
    , kphnSizeType* pBase64StrLenOut
);

/**
 *  @brief Decodes a base64-encoded string and outputs an array of plain bytes.
 *  @param base64Str
 *  @param base64StrLen
 *  @param bytesOut
 *  @param byteBufferSize
 *  @param byteCountOut
 *  @return An error code returned by this function.
 *  @since 2017-05-13
 */
KAPHEIN_PP_DLL_API
int kphnNcodBase64ToBytes(
    const char* base64Str
    , kphnSizeType base64StrLen
    , uint8_t* bytesOut
    , kphnSizeType byteBufferSize
    , kphnSizeType* byteCountOut
);

KAPHEIN_PP_C_LINKAGE_END()

#endif
