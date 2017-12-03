#ifndef KARBONATOR_STRING_ENCODING_H
#define KARBONATOR_STRING_ENCODING_H

#include <stddef.h>
#include <stdint.h>
#include <wchar.h>
#include "karbonator/ErrorCode.h"
#include "karbonator/pp/basic.h"

KARBONATOR_PP_C_LINKAGE_BEGIN()

KARBONATOR_PP_DLL_API
karbonator_ErrorCode bytesToBase64(
    size_t* base64StrLenOut,
    char* base64StrOut,
    size_t base64BufferSize,
    const uint8_t* bytes,
    size_t byteCount
);

KARBONATOR_PP_DLL_API
karbonator_ErrorCode base64ToBytes(
    size_t* byteCountOut,
    uint8_t* bytesOut,
    size_t byteBufferSize,
    const char* base64Str,
    size_t base64StrLen
);

int utf16ToUtf8(
    size_t* byteCountOut,
    uint8_t* pOutBuffer,
    size_t outBufferSize,
    const uint16_t* utf16String,
    size_t utf16StringLength
);

int utf8ToUtf16(
    size_t* byteCountOut,
    uint16_t* pOutBuffer,
    size_t outBufferSize,
    const uint8_t* utf8String,
    size_t utf8StringLength
);

KARBONATOR_PP_DLL_API
int ucs4ToUtf8(
    size_t* outputByteCountOut,
    uint8_t* bufferOut,
    size_t bufferSize,
    const uint32_t* str,
    size_t strLen
);

int utf8ToUcs4(
    size_t* outputByteCountOut,
    uint32_t* pOutBuffer,
    size_t outBufferSize,
    const uint8_t* utf8String,
    size_t utf8StringLength
);

KARBONATOR_PP_C_LINKAGE_END()

#endif
