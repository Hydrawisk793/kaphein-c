#ifndef KARBONATOR_IMAGE_BMP_H
#define KARBONATOR_IMAGE_BMP_H

#include <stdint.h>
#include <stdio.h>
#include "karbonator/ErrorCode.h"
#include "karbonator/pp/basic.h"

KARBONATOR_PP_C_LINKAGE_BEGIN()

typedef struct karbonator_image_BmpHeader_
{
    uint16_t type;
    uint32_t fileSize;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offsetToBits;
} karbonator_image_BmpHeader;

typedef struct karbonator_image_BitmapInfoHeader_
{
    uint32_t size;
    int32_t width;
    int32_t height;
    uint16_t planeCount;
    uint16_t bitsPerPixel;
    uint32_t compression;
    uint32_t imageByteCount;
    int32_t pixelsPerMeterX;
    int32_t pixelsPerMeterY;
    uint32_t colorsInColorTable;
    uint32_t importantColorCount;
} karbonator_image_BitmapInfoHeader;

KARBONATOR_PP_DLL_API
karbonator_ErrorCode karbonator_image_readBitmapHeadersFromFile(
    FILE* fileIn,
    karbonator_image_BmpHeader* pBmpHeaderOut,
    karbonator_image_BitmapInfoHeader* pBitmapInfoHeaderOut
);

KARBONATOR_PP_DLL_API
karbonator_ErrorCode karbonator_image_readBitmapPixelsFromFile(
    FILE* fileIn,
    const karbonator_image_BmpHeader* pBmpHeader,
    const karbonator_image_BitmapInfoHeader* pBitmapInfoHeader,
    void* pPixelsOut,
    size_t pixelsOutLength,
    uint32_t* pPaletteOut,
    size_t paletteOutLength
);

KARBONATOR_PP_C_LINKAGE_END()

#endif
