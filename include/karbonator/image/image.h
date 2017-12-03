#ifndef KARBONATOR_IMAGE_IMAGE_H
#define KARBONATOR_IMAGE_IMAGE_H

#include <stddef.h>
#include <stdint.h>
#include "karbonator/ErrorCode.h"
#include "karbonator/pp/basic.h"

KARBONATOR_PP_C_LINKAGE_BEGIN()

struct karbonator_image_Range {
    int width;
    int height;
    int targetX;
    int targetY;
    int targetWidth;
    int targetHeight;
};

KARBONATOR_PP_DLL_API
FORCEINLINE bool karbonator_image_Range_isValid(
    const struct karbonator_image_Range* pRange
)
{
    return pRange->width >= 0
        && pRange->height >= 0
        && pRange->targetWidth >= 0
        && pRange->targetHeight >= 0
    ;
}

KARBONATOR_PP_DLL_API
FORCEINLINE bool karbonator_image_Range_isXInRange(
    const struct karbonator_image_Range* pRange,
    int x
)
{
    return x >= 0
        && x < pRange->width
        && pRange->targetWidth
    ;
}

KARBONATOR_PP_DLL_API
FORCEINLINE bool karbonator_image_Range_isYInRange(
    const struct karbonator_image_Range* pRange,
    int y
)
{
    return y >= 0
        && y < pRange->height
        && pRange->targetHeight
    ;
}

struct karbonator_image_LabeledFeature {
    size_t x;
    size_t y;
    size_t width;
    size_t height;
    size_t area;
    uint32_t label;
};

enum karbonator_image_ChannelFormat
{
    krbntrImageR8G8B8A8 = 0x00,
    krbntrImageA8B8G8R8 = 0x01,
    krbntrImageA8R8G8B8 = 0x02,
    krbntrImageB8G8R8A8 = 0x03
};

KARBONATOR_PP_DLL_API
int karbonator_image_toGrayscaleByMean(
    const uint32_t* pInputPixels,
    size_t inputPixelCount,
    int channelFormat,
    uint8_t *pGrayPixelsOut,
    size_t grayPixelsCount
);

KARBONATOR_PP_DLL_API
int karbonator_image_binarizeByThreshold(
    uint8_t* pGrayImageInOut,
    size_t rowCount,
    size_t columnCount,
    uint8_t minValue,
    uint8_t maxValue
);

KARBONATOR_PP_DLL_API
int karbonator_image_binarizeByMean(
    uint8_t* pGrayImageInOut,
    size_t rowCount,
    size_t columnCount
);

KARBONATOR_PP_DLL_API
int karbonator_image_binarizeByOtsu(
    uint8_t* pGrayImageInOut,
    size_t rowCount,
    size_t columnCount
);

/**
 * @breif Performs local binarization using niblack algorithm.
 * @param pGrayImageInOut 
 * @param height 
 * @param width 
 * @param k -0.02 
 * @details 
 */
KARBONATOR_PP_DLL_API
int karbonator_image_binarizeByNiblack(
    uint8_t* pGrayImageInOut,
    size_t height,
    size_t width,
    size_t segmentSize,
    float k
);

/**
 * @breif Performs local binarization using niblack algorithm.
 * @param pGrayImageInOut 
 * @param height 
 * @param width 
 * @param k 0.5
 * @param range 128
 * @details 
 */
KARBONATOR_PP_DLL_API
int karbonator_image_binarizeBySauvola(
    uint8_t* pGrayImageInOut,
    size_t height,
    size_t width,
    size_t segmentSize,
    float k,
    uint8_t range
);

KARBONATOR_PP_DLL_API
int karbonator_image_getHistogram(
    const uint8_t* pGrayImageIn,
    size_t rowCount,
    size_t columnCount,
    size_t* pCountsOut,
    size_t outSize
);

KARBONATOR_PP_DLL_API
int karbonator_image_equalizeHistogram(
    uint8_t* pGrayImageInOut,
    size_t rowCount,
    size_t columnCount,
    float sumFactor
);

KARBONATOR_PP_DLL_API
int karbonator_image_stretchHistogram(
    uint8_t* pGrayImageInOut,
    size_t rowCount,
    size_t columnCount
);

KARBONATOR_PP_DLL_API
int karbonator_image_specifyHistogram(
    uint8_t* pGrayImageInOut,
    size_t rowCount,
    size_t columnCount,
    float sumFactor,
    const uint8_t* pDesiredGrayImageIn,
    size_t desiredRowCount,
    size_t desiredColumnCount,
    float desiredSumFactor
);

KARBONATOR_PP_DLL_API
int karbonator_image_labelFeaturesByGrassfire(
    const uint8_t* pGrayImageIn,
    size_t height,
    size_t width,
    uint8_t featureValue,
    uint32_t* pLabelMapOut,
    size_t labelMapSize,
    struct karbonator_image_LabeledFeature* pFeatureOut,
    size_t featureOutSize,
    size_t* pFeatureOutCount
);

KARBONATOR_PP_DLL_API
int karbonator_image_stretchColors(
    uint32_t* pImageInOut,
    size_t imageSize,
    uint32_t min,
    uint32_t max
);

KARBONATOR_PP_C_LINKAGE_END()

#endif
