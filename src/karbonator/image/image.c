#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "karbonator/collection/ListStack.h"
#include "karbonator/image/image.h"

static float karbonator_math_roundf(float v)
{
    const float i = ceilf(v);

    return (v - ceilf(v) < 0.5f ? i + 1 : i);
}

int karbonator_image_toGrayscaleByMean(
    const uint32_t* pInputPixels,
    size_t inputPixelCount,
    int channelFormat,
    uint8_t *pGrayPixelsOut,
    size_t grayPixelsCount
)
{
    const uint32_t* pSrc;
    const uint32_t* pEnd;
    uint8_t * pDest;
    uint32_t value;
    float denominator;

    if(!pInputPixels || !pGrayPixelsOut) {
        return krbntrEcNullArgument;
    }

    if(grayPixelsCount < inputPixelCount) {
        return krbntrEcInvalidArgument;
    }
    
    pSrc = pInputPixels;
    pEnd = pSrc + inputPixelCount;
    pDest = pGrayPixelsOut;

    switch(channelFormat) {
    case krbntrImageR8G8B8A8:
    case krbntrImageB8G8R8A8:
        denominator = 1 / 3.0f;
        
        while(pSrc < pEnd) {
            value = *pSrc++;
            assert((value & 0xFF) == 0);
            *pDest++ = (uint8_t)((((value & 0xFF000000) >> 24) + ((value & 0xFF00) >> 16) + ((value & 0xFF00) >> 8)) * denominator);
        }
    break;
    case krbntrImageA8B8G8R8:
    case krbntrImageA8R8G8B8:
        denominator = 1 / 3.0f;
        
        while(pSrc < pEnd) {
            value = *pSrc++;
            assert((value & 0xFF000000) == 0);
            *pDest++ = (uint8_t)((((value & 0xFF0000) >> 16) + ((value & 0xFF) >> 8) + (value & 0xFF)) * denominator);
        }
    break;
    default:
        return krbntrEcInvalidArgument;
    }

    return krbntrEcNoError;
}

int karbonator_image_binarizeByThreshold(
    uint8_t* pGrayImageInOut,
    size_t rowCount,
    size_t columnCount,
    uint8_t minValue,
    uint8_t maxValue
)
{
    uint8_t* pCurrent;
    uint8_t* pEnd;

    if(!pGrayImageInOut) {
        return krbntrEcNullArgument;
    }

    if(minValue > maxValue) {
        return krbntrEcInvalidArgument;
    }

    pCurrent = pGrayImageInOut;
    pEnd = pGrayImageInOut + rowCount * columnCount;
    for(; pCurrent < pEnd; ++pCurrent) {
        const uint8_t value = *pCurrent;
        *pCurrent = (value >= minValue && value <= maxValue ? 0xFF : 0x00);
    }

    return krbntrEcNoError;
}

int KARBONATOR_PP_DLL_API karbonator_image_binarizeByMean(
    uint8_t* pGrayImageInOut,
    size_t rowCount,
    size_t columnCount
)
{
    const size_t elementCount = rowCount * columnCount;
    uint8_t* pCurrent;
    uint8_t* pEnd;
    unsigned int sum;

    if(!pGrayImageInOut) {
        return -1;
    }

    pCurrent = pGrayImageInOut;
    pEnd = pGrayImageInOut + elementCount;
    sum = 0;
    for(; pCurrent < pEnd; ++pCurrent) {
        sum += *pCurrent;
    }

    return karbonator_image_binarizeByThreshold(
        pGrayImageInOut, rowCount, columnCount,
        (uint8_t)(sum / (float)elementCount),
        255
    );
}

int karbonator_image_binarizeByOtsu(
    uint8_t* pGrayImageInOut,
    size_t rowCount,
    size_t columnCount
)
{
    if(!pGrayImageInOut) {
        return -1;
    }

    rowCount;
    columnCount;

    return -9999;
}

typedef float (*LocalBinarizationFormula) (float, float, void*);

static float niblackFormula(float mean, float variance, void* pParam) {
    return mean + (*((float*)pParam)) * sqrtf(variance);
}

typedef struct SauvolaFormulaParameter_ {
    float k;
    uint8_t range;
} SauvolaFormulaParameter;

static float sauvolaFormula(float mean, float variance, void* pParam) {
    SauvolaFormulaParameter* pFormulaParam = (SauvolaFormulaParameter*)pParam;

    return mean * (1.0f - pFormulaParam->k * (1.0f - sqrtf(variance) / (float)pFormulaParam->range));
}

static int doLocalBinarization(
    uint8_t* pGrayImageInOut,
    size_t height,
    size_t width,
    size_t segmentSize,
    LocalBinarizationFormula formula,
    void* formulaParam
) {
    const size_t pixelCount = height * width;
    const size_t segBaseYOffInc = segmentSize * width;
    size_t segBaseXOff, segBaseYOff, segEndXOff, segEndYOff;
    size_t segXOff, segYOff;
    size_t segPixelCount;
    float sum, sumOfSq, mean, variance, invSegPixelCount;
    float localThreshold;
    uint8_t value;

    if(!pGrayImageInOut) {
        return krbntrEcNullArgument;
    }
    
    /*//////////////////////////////////////////////////////////*/
    /*Iterate each segments.*/

    for(
        segBaseYOff = 0, segEndYOff = segBaseYOffInc;
        segBaseYOff < pixelCount;
        segBaseYOff += segBaseYOffInc, segEndYOff += segBaseYOffInc
    ) {
        for(
            segBaseXOff = 0, segEndXOff = segmentSize;
            segBaseXOff < width;
            segBaseXOff += segmentSize, segEndXOff += segmentSize
        ) {
            /*//////////////////////////////////////////////////////////*/
            /*Binarize current segment.*/

            segPixelCount = 0;
            sum = 0.0f;
            sumOfSq = 0.0f;

            for(
                segYOff = segBaseYOff;
                segYOff < pixelCount && segYOff < segEndYOff;
                segYOff += width
            ) {
                for(
                    segXOff = segBaseXOff;
                    segXOff < width && segXOff < segEndXOff;
                    ++segXOff
                ) {
                    value = pGrayImageInOut[segXOff + segYOff];

                    sum += value;
                    sumOfSq += (float)value * (float)value;
                    ++segPixelCount;
                }
            }

            invSegPixelCount = 1.0f / segPixelCount;
            mean = sum * invSegPixelCount;
            variance = sumOfSq * invSegPixelCount - mean * mean;
            localThreshold = (*formula)(mean, variance, formulaParam);
            
            for(
                segYOff = segBaseYOff;
                segYOff < pixelCount && segYOff < segEndYOff;
                segYOff += width
            ) {
                for(
                    segXOff = segBaseXOff;
                    segXOff < width && segXOff < segEndXOff;
                    ++segXOff
                ) {
                    value = pGrayImageInOut[segXOff + segYOff];

                    pGrayImageInOut[segXOff + segYOff] = (value < localThreshold ? 0x00 : 0xFF);
                }
            }

            /*//////////////////////////////////////////////////////////*/
        }
    }

    /*//////////////////////////////////////////////////////////*/

    return krbntrEcNoError;
}

int karbonator_image_binarizeByNiblack(
    uint8_t* pGrayImageInOut,
    size_t height,
    size_t width,
    size_t segmentSize,
    float k
)
{
    return doLocalBinarization(pGrayImageInOut, height, width, segmentSize, niblackFormula, &k);
}

int karbonator_image_binarizeBySauvola(
    uint8_t* pGrayImageInOut,
    size_t height,
    size_t width,
    size_t segmentSize,
    float k,
    uint8_t range
)
{
    SauvolaFormulaParameter param;
    param.k = k;
    param.range = range;

    return doLocalBinarization(pGrayImageInOut, height, width, segmentSize, sauvolaFormula, &param);
}

int karbonator_image_getHistogram(
    const uint8_t* pGrayImageIn,
    size_t rowCount,
    size_t columnCount,
    size_t* pCountsOut,
    size_t outSize
)
{
    const uint8_t* pCurrent;
    const uint8_t* pEnd;

    if(!pGrayImageIn) {
        return -1;
    }

    if(!pCountsOut) {
        return -2;
    }

    if(outSize < 256) {
        return -3;
    }

    memset(pCountsOut, 0, sizeof(*pCountsOut) * outSize);
    
    pCurrent = pGrayImageIn;
    pEnd = pGrayImageIn + rowCount * columnCount;
    for(; pCurrent < pEnd; ++pCurrent) {
        ++pCountsOut[*pCurrent];
    }

    return 0;
}

static int calculateNormalizedSum(
    const uint8_t* pGrayImageIn,
    size_t rowCount,
    size_t columnCount,
    float sumFactor,
    uint8_t* pNormalizedSumOut
) {
    const size_t elementCount = rowCount * columnCount;
    const float scaleFactor = 0xFF / (float)elementCount;
    size_t histogram[256];
    size_t sum;
    int i;

    if(!pGrayImageIn) {
        return krbntrEcNullArgument;
    }

    karbonator_image_getHistogram(pGrayImageIn, rowCount, columnCount, histogram, 256);

    memset(pNormalizedSumOut, 0, 256 * sizeof(*pNormalizedSumOut));
    sum = 0;
    for(i = 0; i < 256; ++i) {
        sum += histogram[i];
        pNormalizedSumOut[i] = (uint8_t)(sum * scaleFactor + sumFactor);
    }

    return krbntrEcNoError;
}

int karbonator_image_equalizeHistogram(
    uint8_t* pGrayImageInOut,
    size_t rowCount,
    size_t columnCount,
    float sumFactor
)
{
    uint8_t normalizedSum[256];
    uint8_t* pCurrent;
    uint8_t* pEnd;
    int result;

    result = calculateNormalizedSum(pGrayImageInOut, rowCount, columnCount, sumFactor, normalizedSum);
    if(result) {
        return result;
    }

    pCurrent = pGrayImageInOut;
    pEnd = pGrayImageInOut + rowCount * columnCount;
    for(; pCurrent < pEnd; ++pCurrent) {
        *pCurrent = normalizedSum[*pCurrent];
    }

    return krbntrEcNoError;
}

int karbonator_image_stretchHistogram(
    uint8_t* pGrayImageInOut,
    size_t rowCount,
    size_t columnCount
)
{
    const size_t imageSize = rowCount * columnCount;
    size_t valueRangeLength;
    uint8_t* pCurrent;
    uint8_t* pEnd;
    uint8_t minValue = 0xFF, maxValue = 0x00;
    float scaleFactor;
    uint8_t* adjustedPixels;
    unsigned int i;

    if(!pGrayImageInOut) {
        return krbntrEcNullArgument;
    }

    pCurrent = pGrayImageInOut;
    pEnd = pGrayImageInOut + imageSize;
    for(; pCurrent < pEnd; ++pCurrent) {
        const uint8_t value = *pCurrent;
        
        if(minValue > value) {
            minValue = value;
        }

        if(maxValue < value) {
            maxValue = value;
        }
    }

    valueRangeLength = maxValue - minValue;
    scaleFactor = 0xFF / (float)(valueRangeLength);
    adjustedPixels = (uint8_t*)malloc(valueRangeLength);
    pCurrent = adjustedPixels;
    pEnd = pCurrent + valueRangeLength;
    for(i = 0; pCurrent < pEnd; ++pCurrent, ++i) {
        *pCurrent = (uint8_t)(i * scaleFactor);
    }

    pCurrent = pGrayImageInOut;
    pEnd = pGrayImageInOut + imageSize;
    for(; pCurrent < pEnd; ++pCurrent) {
        *pCurrent = adjustedPixels[(*pCurrent - minValue)];
    }

    free(adjustedPixels);

    return krbntrEcNoError;
}

/**
 * TODO : 코드 마저 작성
 */
int karbonator_image_specifyHistogram(
    uint8_t* pGrayImageInOut,
    size_t rowCount,
    size_t columnCount,
    float sumFactor,
    const uint8_t* pDesiredGrayImageIn,
    size_t desiredRowCount,
    size_t desiredColumnCount,
    float desiredSumFactor
)
{
    uint8_t inOutNormalizedSum[256];
    uint8_t desiredNormalizedSum[256];
    uint8_t selectedSum[256];
    uint8_t minSum, diff;
    int i, j;

    if(!pGrayImageInOut || !pDesiredGrayImageIn) {
        return krbntrEcNullArgument;
    }

    calculateNormalizedSum(pGrayImageInOut, rowCount, columnCount, sumFactor, inOutNormalizedSum);

    calculateNormalizedSum(pDesiredGrayImageIn, desiredRowCount, desiredColumnCount, desiredSumFactor, desiredNormalizedSum);
    
    for(i = 0; i < 256; ++i) {
        minSum = 0xFF;
        for(j = 0; j < 256; ++j) {
            diff = (uint8_t)abs(inOutNormalizedSum[i] - desiredNormalizedSum[j]);
            if(diff < minSum) {
                selectedSum;
            }
        }
    }

    return krbntrEcNoError;
}

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
)
{
    struct Point {
        size_t x;
        size_t y;
    };
    
    struct Point* pointStack;
    struct Point* pPoint;
    struct karbonator_image_LabeledFeature* pCurrentFeature;
    size_t imageSize;
    size_t pointStackByteCount;
    size_t pointStackTop;
    size_t featureIndex;
    size_t labelMapIndex, area;
    size_t imgX, imgY, x, y, xEnd, yEnd;
    size_t minX, maxX, minY, maxY;
    bool featureFound;

    if(!pGrayImageIn || !pLabelMapOut) {
        return krbntrEcNullArgument;
    }

    imageSize = (size_t)(height * width);
    if(labelMapSize < imageSize) {
        return krbntrEcNotEnoughBufferSize;
    }

    featureIndex = 0;
    memset(pLabelMapOut, 0, sizeof(*pLabelMapOut) * labelMapSize);
    pointStackTop = 0;
    pointStackByteCount = sizeof(struct Point) * imageSize;
    pointStack = (struct Point*)malloc(pointStackByteCount);
    memset(pointStack, 0, pointStackByteCount);

    for(imgY = 0; imgY < height; ++imgY) {
        for(imgX = 0; imgX < width; ++imgX) {
            pPoint = pointStack + pointStackTop;
            pPoint->x = imgX;
            pPoint->y = imgY;
            ++pointStackTop;

            featureFound = false;
            area = 0;
            minX = (size_t)~0;
            maxX = 0;
            minY = (size_t)~0;
            maxY = 0;

            while(pointStackTop > 0) {
                --pointStackTop;
                pPoint = pointStack + pointStackTop;

                x = pPoint->x;
                y = pPoint->y;
                labelMapIndex = y * width + x;
                if(pLabelMapOut[labelMapIndex] == 0 && pGrayImageIn[labelMapIndex] == featureValue) {
                    pLabelMapOut[labelMapIndex] = featureIndex + 1;
                    ++area;
                    if(minX > x) {
                        minX = x;
                    }
                    if(maxX < x) {
                        maxX = x;
                    }
                    if(minY > y) {
                        minY = y;
                    }
                    if(maxY < y) {
                        maxY = y;
                    }

                    xEnd = pPoint->x + 2;
                    yEnd = pPoint->y + 2;
                    for(y = (pPoint->y > 0 ? pPoint->y - 1 : 0); y < width && y < yEnd; ++y) {
                        for(x = (pPoint->x > 0 ? pPoint->x - 1 : 0); x < width && x < xEnd; ++x) {
                            pPoint = pointStack + pointStackTop;
                            pPoint->x = x;
                            pPoint->y = y;
                            ++pointStackTop;
                        }
                    }

                    featureFound = true;
                }
            }
            
            if(featureFound && pFeatureOut && featureOutSize > 0) {
                pCurrentFeature = pFeatureOut + featureIndex;
                pCurrentFeature->label = featureIndex + 1;
                pCurrentFeature->area = area;
                pCurrentFeature->x = minX;
                pCurrentFeature->y = minY;
                pCurrentFeature->width = maxX - minX + 1;
                pCurrentFeature->height = maxY - minY + 1;

                ++featureIndex;
            }
        }
    }

    if(pFeatureOutCount) {
        *pFeatureOutCount = featureIndex; 
    }

    free(pointStack);

    return krbntrEcNoError;
}

KARBONATOR_PP_DLL_API
int karbonator_image_stretchColors(
    uint32_t* pImageInOut,
    size_t imageSize,
    uint32_t min,
    uint32_t max
)
{
    size_t i;
    uint32_t minColor;
    uint32_t maxColor;
    uint32_t oldLen;
    uint32_t newLen;
    uint32_t color;
    float normFactor;

    if(!pImageInOut) {
        return krbntrEcNullArgument;
    }

    maxColor = (uint32_t)0;
    minColor = (uint32_t)~0;
    for(i = imageSize; i > 0; ) {
        --i;

        color = pImageInOut[i];

        if(maxColor < color) {
            maxColor = color;
        }
        if(minColor > color) {
            minColor = color;
        }
    }

    oldLen = maxColor - minColor;
    normFactor = 1.0f / (float)(oldLen);
    newLen = max - min;
    for(i = imageSize; i > 0; ) {
        --i;
        
        pImageInOut[i] = (uint32_t)(((pImageInOut[i] - minColor) * normFactor) * newLen + min);
    }

    return krbntrEcNoError;
}
