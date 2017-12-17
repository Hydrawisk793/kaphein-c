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

#include <stdlib.h>
#include <string.h>
#include "kaphein/ncod/base64.h"

int kphnNcodBytesToBase64(
    const uint8_t* pBytes
    , size_t byteCount
    , char* pBase64StrOut
    , size_t base64BufferSize
    , size_t* pBase64StrLenOut
) {
    static const char* base64Chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789"
        "+/"
    ;
    
    char* const pBase64BufferEnd = pBase64StrOut + base64BufferSize;
    union
    { 
        uint32_t buffer;
        char chars [4];
    } fourBase64Chars;
    size_t remainingByteCount = byteCount;
    size_t byteCountToCopy;
    size_t base64StrLen = 0;
    int result = krbntrEcNoError;

    while(remainingByteCount > 0) {
        fourBase64Chars.chars[0] = base64Chars[pBytes[0] >> 2];
        fourBase64Chars.chars[1] = base64Chars[((pBytes[0] << 4) | (pBytes[1] >> 4)) & 0x3F];
        fourBase64Chars.chars[2] = base64Chars[((pBytes[1] << 2) | (pBytes[2] >> 6)) & 0x3F];
        fourBase64Chars.chars[3] = base64Chars[pBytes[2] & 0x3F];
        
        switch(remainingByteCount) {
        case 0:
        break;
        case 1:
            fourBase64Chars.chars[2] = '=';

            //Fall through.
        case 2:
            fourBase64Chars.chars[3] = '=';

            remainingByteCount = 0;
        break;
        default:
            remainingByteCount -= 3;
            pBytes += 3;
        }

        byteCountToCopy = pBase64BufferEnd - pBase64StrOut;
        if(byteCountToCopy > 4) {
            byteCountToCopy = 4;
        }
        else {
            result = krbntrEcNotEnoughBufferSize;

            remainingByteCount = 0;
        }

        memcpy(pBase64StrOut, fourBase64Chars.chars, byteCountToCopy);
        pBase64StrOut += byteCountToCopy;
        base64StrLen += byteCountToCopy;
    }

    if(NULL != pBase64StrLenOut) {
        *pBase64StrLenOut = base64StrLen;
    }

    return result;
}

int kphnNcodBase64ToBytes(
    const char* base64Str
    , size_t base64StrLen
    , uint8_t* bytesOut
    , size_t byteBufferSize
    , size_t* byteCountOut
) {
    int result = krbntrEcNoError;
    uint8_t* const bytesEnd = bytesOut + byteBufferSize;
    char threeBytes [3];
    char base64Char;
    uint8_t base64CharIndex;
    size_t byteCountToCopy;
    size_t count = base64StrLen;
    size_t base64CharCount = 0;
    size_t byteCount = 0;

    while(count > 0) {
        --count;
        
        if(bytesOut >= bytesEnd) {
            result = krbntrEcNotEnoughBufferSize;
            break;
        }

        base64Char = *base64Str++;
        if(base64Char >= 'A' && base64Char <= 'Z') {
            base64CharIndex = base64Char - 'A';
        }
        else if(base64Char >= 'a' && base64Char <= 'z') {
            base64CharIndex = base64Char - 'a' + ('Z' - 'A' + 1);
        }
        else if(base64Char >= '0' && base64Char <= '9') {
            base64CharIndex = base64Char - '0' + ('Z' - 'A' + 1) + ('z' - 'a' + 1);
        }
        else if(base64Char == '+') {
            base64CharIndex = 62;
        }
        else if(base64Char == '/') {
            base64CharIndex = 63;
        }
        else if(base64Char == '=') {
            break;
        }
        else {
            result = krbntrEcInvalidValue;
            break;
        }

        switch(base64CharCount) {
        case 0:
            memset(threeBytes, 0x00, sizeof(threeBytes));

            threeBytes[0] = base64CharIndex << 2;

            ++base64CharCount;
        break;
        case 1:
            threeBytes[0] |= base64CharIndex >> 4;
            threeBytes[1] = base64CharIndex << 4;

            ++base64CharCount;
        break;
        case 2:
            threeBytes[1] |= base64CharIndex >> 2;
            threeBytes[2] = base64CharIndex << 6;

            ++base64CharCount;
        break;
        case 3:
            threeBytes[2] |= base64CharIndex;

            base64CharCount = 0;

            byteCountToCopy = bytesEnd - bytesOut;
            if(byteCountToCopy >= sizeof(threeBytes)) {
                byteCountToCopy = sizeof(threeBytes);
            }

            memcpy(bytesOut, threeBytes, byteCountToCopy);
            bytesOut += byteCountToCopy;
            byteCount += byteCountToCopy;
        break;
        }
    }
    
    byteCountToCopy = bytesEnd - bytesOut;
    if(byteCountToCopy >= base64CharCount) {
        byteCountToCopy = base64CharCount;
    }

    memcpy(bytesOut, threeBytes, byteCountToCopy);
    byteCount += byteCountToCopy;

    if(NULL != byteCountOut) {
        *byteCountOut = byteCount;
    }

    return result;
}
