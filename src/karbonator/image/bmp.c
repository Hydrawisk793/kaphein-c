#include <stdlib.h>
#include <string.h>
#include "karbonator/image/bmp.h"

static INLINE int makeMultipleOf4(int value) {
    return ((value + 3) >> 2) << 2;
}

static INLINE size_t readValue(FILE* fin, void* pDest, size_t size)
{
    size_t elementCount = fread(pDest, size, 1, fin);
    if(elementCount < 1) {
        elementCount = (feof(fin) ? 1 : 0);
    }

    return elementCount * size;
}

karbonator_ErrorCode karbonator_image_readBitmapHeadersFromFile(
    FILE* fileIn,
    karbonator_image_BmpHeader* pBmpHeaderOut,
    karbonator_image_BitmapInfoHeader* pBitmapInfoHeaderOut
)
{
    size_t byteCount;

    if(
        fileIn == NULL
        || pBmpHeaderOut == NULL
        || pBitmapInfoHeaderOut == NULL
    ) {
        return krbntrEcNullArgument;
    }

    /*////////////////////////////////*/

    byteCount = 0;
    byteCount += fread(&(pBmpHeaderOut->type), sizeof(pBmpHeaderOut->type), 1, fileIn);
    byteCount += fread(&(pBmpHeaderOut->fileSize), sizeof(pBmpHeaderOut->fileSize), 1, fileIn);
    byteCount += fread(&(pBmpHeaderOut->reserved1), sizeof(pBmpHeaderOut->reserved1), 1, fileIn);
    byteCount += fread(&(pBmpHeaderOut->reserved2), sizeof(pBmpHeaderOut->reserved2), 1, fileIn);
    byteCount += fread(&(pBmpHeaderOut->offsetToBits), sizeof(pBmpHeaderOut->offsetToBits), 1, fileIn);
    if(byteCount < 5) {
        return krbntrEcIoError;
    }

    if(pBmpHeaderOut->type != 0x4D42) {
        return krbntrEcInvalidValue;
    }

    /*////////////////////////////////*/

    /*////////////////////////////////*/

    byteCount = 0;
    byteCount += readValue(fileIn, &(pBitmapInfoHeaderOut->size), sizeof(pBitmapInfoHeaderOut->size));
    byteCount += readValue(fileIn, &(pBitmapInfoHeaderOut->width), sizeof(pBitmapInfoHeaderOut->width));
    byteCount += readValue(fileIn, &(pBitmapInfoHeaderOut->height), sizeof(pBitmapInfoHeaderOut->height));
    byteCount += readValue(fileIn, &(pBitmapInfoHeaderOut->planeCount), sizeof(pBitmapInfoHeaderOut->planeCount));
    byteCount += readValue(fileIn, &(pBitmapInfoHeaderOut->bitsPerPixel), sizeof(pBitmapInfoHeaderOut->bitsPerPixel));
    byteCount += readValue(fileIn, &(pBitmapInfoHeaderOut->compression), sizeof(pBitmapInfoHeaderOut->compression));
    byteCount += readValue(fileIn, &(pBitmapInfoHeaderOut->imageByteCount), sizeof(pBitmapInfoHeaderOut->imageByteCount));
    byteCount += readValue(fileIn, &(pBitmapInfoHeaderOut->pixelsPerMeterX), sizeof(pBitmapInfoHeaderOut->pixelsPerMeterX));
    byteCount += readValue(fileIn, &(pBitmapInfoHeaderOut->pixelsPerMeterY), sizeof(pBitmapInfoHeaderOut->pixelsPerMeterY));
    byteCount += readValue(fileIn, &(pBitmapInfoHeaderOut->colorsInColorTable), sizeof(pBitmapInfoHeaderOut->colorsInColorTable));
    byteCount += readValue(fileIn, &(pBitmapInfoHeaderOut->importantColorCount), sizeof(pBitmapInfoHeaderOut->importantColorCount));
    if(byteCount < sizeof(*pBitmapInfoHeaderOut)) {
        return krbntrEcIoError;
    }

    if(pBitmapInfoHeaderOut->bitsPerPixel < 8) {
        return krbntrEcInvalidValue;
    }

    /*////////////////////////////////*/

    return krbntrEcNoError;
}

karbonator_ErrorCode karbonator_image_readBitmapPixelsFromFile(
    FILE* fileIn,
    const karbonator_image_BmpHeader* pBmpHeader,
    const karbonator_image_BitmapInfoHeader* pBitmapInfoHeader,
    void* pPixelsOut,
    size_t pixelsOutLength,
    uint32_t* pPaletteOut,
    size_t paletteOutLength
)
{
    size_t pixelByteCount;
    size_t byteCount;
    int widthMultiple4ByteCount;
    int x, y;
    uint16_t bytesPerPixel;
    void* pixelBytes;

    pBmpHeader;

    memset(pPaletteOut, 0, sizeof(*pPaletteOut) * paletteOutLength);
    if(pBitmapInfoHeader->colorsInColorTable > 0) {
        if(paletteOutLength < pBitmapInfoHeader->colorsInColorTable) {
            return krbntrEcNotEnoughBufferSize;
        }

        if(!fread(pPaletteOut, sizeof(uint32_t), pBitmapInfoHeader->colorsInColorTable, fileIn)) {
            return krbntrEcIoError;
        }
    }
    
    if(pixelsOutLength < (size_t)(pBitmapInfoHeader->width * pBitmapInfoHeader->height * (pBitmapInfoHeader->bitsPerPixel >> 3))) {
        return krbntrEcNotEnoughBufferSize;
    }

    bytesPerPixel = pBitmapInfoHeader->bitsPerPixel >> 3;
    widthMultiple4ByteCount = bytesPerPixel * makeMultipleOf4(pBitmapInfoHeader->width);
    pixelByteCount = widthMultiple4ByteCount * pBitmapInfoHeader->height;
    pixelBytes = malloc(pixelByteCount);
    byteCount = readValue(fileIn, pixelBytes, pixelByteCount);
    if(byteCount < pixelByteCount) {
        free(pixelBytes);

        return krbntrEcIoError;
    }

    if(pBitmapInfoHeader->colorsInColorTable > 0) {
        uint8_t* pDest = (uint8_t*)pPixelsOut;
        uint8_t* pSrc = (uint8_t*)pixelBytes;
        const size_t withByteCount = pBitmapInfoHeader->width * bytesPerPixel;
        size_t destYOffset = 0;
        size_t srcYOffset = (pBitmapInfoHeader->height - 1) * widthMultiple4ByteCount;

        for(y = 0; y < pBitmapInfoHeader->height; ++y) {
            for(x = 0; x < pBitmapInfoHeader->width; ++x) {
                const size_t colorIndex = pSrc[srcYOffset + x * bytesPerPixel];
                const uint32_t colorValue = pPaletteOut[colorIndex];

                switch(bytesPerPixel) {
                case 0:
                break;
                case 1:
                    pDest[destYOffset + x * bytesPerPixel] = (uint8_t)(colorValue & 0xFF);
                break;
                case 2:
                    pDest[destYOffset + x * bytesPerPixel] = (uint8_t)((colorValue & 0xFF00) >> 8);
                    pDest[destYOffset + x * bytesPerPixel + 1] = (uint8_t)(colorValue & 0xFF);
                break;
                case 3:
                    pDest[destYOffset + x * bytesPerPixel] = (uint8_t)((colorValue & 0xFF) >> 16);
                    pDest[destYOffset + x * bytesPerPixel + 1] = (uint8_t)((colorValue & 0xFF) >> 8);
                    pDest[destYOffset + x * bytesPerPixel + 2] = (uint8_t)((colorValue & 0xFF));
                break;
                case 4:
                    pDest[destYOffset + x * bytesPerPixel] = (uint8_t)((colorValue & 0xFF) >> 24);
                    pDest[destYOffset + x * bytesPerPixel + 1] = (uint8_t)((colorValue & 0xFF) >> 16);
                    pDest[destYOffset + x * bytesPerPixel + 2] = (uint8_t)((colorValue & 0xFF) >> 8);
                    pDest[destYOffset + x * bytesPerPixel + 3] = (uint8_t)((colorValue & 0xFF));
                break;
                }
            }

            destYOffset += withByteCount;
            srcYOffset -= widthMultiple4ByteCount;
        }
    }
    else {
        uint8_t* pDest = (uint8_t*)pPixelsOut;
        uint8_t* pSrc = (uint8_t*)pixelBytes;
        const size_t withByteCount = pBitmapInfoHeader->width * bytesPerPixel;
        size_t destYOffset = 0;
        size_t srcYOffset = (pBitmapInfoHeader->height - 1) * widthMultiple4ByteCount;

        for(y = 0; y < pBitmapInfoHeader->height; ++y) {
            memcpy(pDest + destYOffset, pSrc + srcYOffset, withByteCount);
            
            destYOffset += withByteCount;
            srcYOffset -= widthMultiple4ByteCount;
        }
    }

    free(pixelBytes);

    return krbntrEcNoError;
}
