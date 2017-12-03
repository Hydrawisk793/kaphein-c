#ifndef KARBONATOR_ERRORCODE_H
#define KARBONATOR_ERRORCODE_H

typedef enum karbonator_ErrorCode_ {
    krbntrEcNoError = 0,
    krbntrEcGenericError = 1,
    krbntrEcNotSupportedOperation = 2,
    
    krbntrEcNullPointer = 20,
    krbntrEcContainerUnderflow = 22,
    krbntrEcContainerOverflow = 23,
    krbntrEcNotEnoughBufferSize = 24,
    krbntrEcNotEnoughMemory = 25,
    
    krbntrEcInvalidArgument = 30,
    krbntrEcInvalidValue = 31,
    krbntrEcNullArgument = 32,
    krbntrEcIndexOutOfRange = 33,

    krbntrEcNotFound = 40,
    krbntrEcNoSuchElement = 41,
    krbntrEcNoMoreMemoryBlocksLeft = 42,

    krbntrEcIoError = 100
} karbonator_ErrorCode;

#endif
