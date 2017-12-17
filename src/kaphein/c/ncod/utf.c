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
#include "kaphein/c/ncod/utf.h"

int kphnNcodUcs4ToUtf8(
    const uint32_t* utf32String
    , size_t utf32StringLength
    , uint8_t* pBufferOut
    , size_t outBufferSize
    , size_t* pOutputByteCountOut
) {
    static const uint32_t byteCountBorders [] = {
        0x80, 0x0800,
        0x010000, 0x200000,
        0x04000000, 0x80000000
    };
    static const uint32_t firstByteMasks [] = {
        0x0000007F, 0x000007C0,
        0x0000F000, 0x001C0000,
        0x03000000, 0x40000000
    };
    static const uint32_t restByteBitMasks [] = {
        0x00000000, 0x0000003F,
        0x00000FC0, 0x0003F000,
        0x00FC0000, 0x3F000000
    };
    static const size_t shiftCounts [] = {
        0, 6, 
        12, 18,
        24, 30
    };
    static const size_t borderCount = sizeof(byteCountBorders) / sizeof(*byteCountBorders);
    static const uint8_t byteCountBits [] = {
        0x00, 0xC0,
        0xE0, 0xF0,
        0xF8, 0xFC
    };
    
    uint8_t* const pBufferEnd = pBufferOut + outBufferSize;
    size_t i = utf32StringLength;
    size_t outputByteCount = 0;
    size_t restByteCount;
    uint32_t ch;

    while(i > 0) {
        --i;

        ch = *utf32String++;

        for(
            restByteCount = 0;
            restByteCount < borderCount;
            ++restByteCount
        ) {
            if(ch < byteCountBorders[restByteCount]) {
                break;
            }
        }
        if(restByteCount >= borderCount) {
            return -1;
        }
        
        if(pBufferOut >= pBufferEnd) {
            return -2;
        }
        *pBufferOut++ = ((uint8_t)((ch & firstByteMasks[restByteCount]) >> shiftCounts[restByteCount])) | byteCountBits[restByteCount];
        ++outputByteCount;

        while(restByteCount > 0) {
            if(pBufferOut >= pBufferEnd) {
                return -2;
            }
            *pBufferOut++ = (uint8_t)(((ch & restByteBitMasks[restByteCount]) >> shiftCounts[restByteCount - 1]) | 0x80);
            ++outputByteCount;

            --restByteCount;
        }
    }

    if(NULL != pOutputByteCountOut) {
        *pOutputByteCountOut = outputByteCount;
    }
    
    return 0;
}

int kphnNcodUtf8ToUcs4(
    const uint8_t* utf8String
    , size_t utf8StringLength
    , uint32_t* pOutBuffer
    , size_t outBufferSize
    , size_t* pCharCountOut
)
{
    const uint8_t* pSrc;
    const uint8_t* pSrcEnd;
    uint32_t* pDest;
    uint32_t* pDestEnd;
    uint8_t srcValue;
    size_t charCount;
    int trailingByteCount;
    int bitMask;

    if(utf8String == NULL || pOutBuffer == NULL) {
        return krbntrEcNullArgument;
    }

    charCount = 0;
    pSrc = utf8String;
    pSrcEnd = pSrc + utf8StringLength;
    pDest = pOutBuffer;
    pDestEnd = pDest + outBufferSize;
    while(pSrc < pSrcEnd) {
        srcValue = *pSrc;

        for(
            trailingByteCount = 0, bitMask = 0xC0;
            (srcValue & bitMask) == bitMask;
            ++trailingByteCount, bitMask >>= 1
        );

        switch(trailingByteCount)
        {
        case 0:
            if(pDest >= pDestEnd) {
                return krbntrEcNotEnoughBufferSize;
            }
            *pDest++ = srcValue;
        break;
        case 1:
        break;
        case 2:
        break;
        case 3:
        break;
        case 4:
        break;
        case 5:
        break;
        case 6:
        break;
        default:
            return krbntrEcInvalidArgument;
        }

        ++charCount;
    }

    if(pCharCountOut != NULL) {
        *pCharCountOut = charCount;
    }

    return krbntrEcNoError;
}

int kphnNcodUcs4ToUtf16(
    const uint32_t* str
    , size_t strLen
    , uint16_t* bufferOut
    , size_t bufferSize
    , size_t* outputElementCountOut
)
{
    const uint32_t* pSrcChar;
    const uint32_t* pSrcCharEnd;
    size_t elementCount;
    uint16_t* pDest;
    uint16_t* pDestEnd;
    uint32_t srcChar;
    uint16_t srcHigh;
    uint16_t srcLow;

    if(
        bufferOut == NULL
        || str == NULL
    ) {
        return krbntrEcNullArgument;
    }

    elementCount = 0;
    pSrcChar = str;
    pSrcCharEnd = pSrcChar + strLen;
    pDest = bufferOut;
    pDestEnd = pDest + bufferSize;
    while(pSrcChar < pSrcCharEnd) {
        srcChar = *pSrcChar++;
        srcHigh = (uint16_t)((srcChar & 0xFFFF0000) >> 16);
        srcLow = (uint16_t)(srcChar & 0x0000FFFF);

        switch((!!srcHigh) & 0x01) {
        case 0:
            if(pDest >= pDestEnd) {
                return krbntrEcNotEnoughBufferSize;
            }
            *pDest++ = srcLow;
            ++elementCount;
        break;
        case 1:
            if(srcHigh >= 0x10) {
                return krbntrEcValueOverflow;
            }
            
            if(pDest >= pDestEnd) {
                return krbntrEcNotEnoughBufferSize;
            }
            *pDest++ = (uint16_t)(0xD800 | (((srcHigh & 0x1F) - 1) << 6) | ((srcLow & 0xFC00) >> 10));
            ++elementCount;
            
            if(pDest >= pDestEnd) {
                return krbntrEcNotEnoughBufferSize;
            }
            *pDest++ = (uint16_t)(0xDC00 | (srcLow & 0x03FF));
            ++elementCount;
        break;
        }
    }

    if(outputElementCountOut != NULL) {
        *outputElementCountOut = elementCount;
    }

    return krbntrEcNoError;
}

int kphnNcodUtf16ToUcs4(
    const uint16_t* utf16String
    , size_t utf16StringLength
    , uint32_t* pOutBuffer
    , size_t outBufferSize
    , size_t* outputCharCountOut
)
{
    const uint16_t* pSrc;
    const uint16_t* pSrcEnd;
    size_t charCount;
    uint32_t* pDest;
    uint32_t* pDestEnd;
    uint16_t srcHigh;
    uint16_t srcLow;

    if(
        outputCharCountOut == NULL
        || pOutBuffer == NULL
        || utf16String == NULL
    ) {
        return krbntrEcNullArgument;
    }
    
    charCount = 0;
    pSrc = utf16String;
    pSrcEnd = pSrc + utf16StringLength;
    pDest = pOutBuffer;
    pDestEnd = pDest + outBufferSize;
    while(pSrc < pSrcEnd) {
        srcHigh = *pSrc++;

        switch((!!(srcHigh & 0xD800)) & 0x01) {
        case 0:
            if(pDest >= pDestEnd) {
                return krbntrEcNotEnoughBufferSize;
            }
            *pDest++ = srcHigh;
        break;
        case 1:
            if(pSrc < pSrcEnd) {
                return krbntrEcInvalidArgument;
            }
            srcLow = *pSrc++;

            if(pDest >= pDestEnd) {
                return krbntrEcNotEnoughBufferSize;
            }
            *pDest++ = (
                (((srcHigh & 0x03C0) >> 6) + 1)
                | (((srcHigh & 0x3F) << 10) | (srcLow & 0x03FF))
            );
        break;
        }

        ++charCount;
    }

    if(outputCharCountOut != NULL) {
        *outputCharCountOut = charCount;
    }

    return krbntrEcNoError;
}
