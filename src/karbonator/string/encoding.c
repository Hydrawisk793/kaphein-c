#include <stdlib.h>
#include <string.h>
#include "karbonator/string/encoding.h"

karbonator_ErrorCode bytesToBase64(
    size_t* pBase64StrLenOut,
    char* pBase64StrOut,
    size_t base64BufferSize,
    const uint8_t* pBytes,
    size_t byteCount
) {
    static const char* base64Chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789"
        "+/"
    ;
    
    char* const pBase64BufferEnd = pBase64StrOut + base64BufferSize;
    union { 
        uint32_t buffer;
        char chars [4];
    } fourBase64Chars;
    size_t remainingByteCount = byteCount;
    size_t byteCountToCopy;
    size_t base64StrLen = 0;
    karbonator_ErrorCode result = krbntrEcNoError;

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

karbonator_ErrorCode base64ToBytes(
    size_t* byteCountOut,
    uint8_t* bytesOut,
    size_t byteBufferSize,
    const char* base64Str,
    size_t base64StrLen
) {
    karbonator_ErrorCode result = krbntrEcNoError;
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

int ucs4ToUtf8(
    size_t* pOutputByteCountOut,
    uint8_t* pBufferOut,
    size_t outBufferSize,
    const uint32_t* utf32String,
    size_t utf32StringLength
) {
    static const uint32_t byteCountBorders [] = {
        0x80, 0x0800,
        0x010000, 0x200000,
        0x04000000, 0x80000000
    };
    static const size_t borderCount = sizeof(byteCountBorders) / sizeof(*byteCountBorders);
    static const uint32_t firstByteMasks [] = {
        0x0000007F, 0x000007C0,
        0x0000F000, 0x001C0000,
        0x03000000, 0x40000000
    };
    static const int shiftCounts [] = {
        0, 6, 
        12, 18,
        24, 30
    };
    static const uint32_t restByteBitMasks [] = {
        0x00000000, 0x0000003F,
        0x00000FC0, 0x0003F000,
        0x00FC0000, 0x3F000000
    };
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
