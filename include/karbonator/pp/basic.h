#ifndef KARBONATOR_PP_BASIC_H
#define KARBONATOR_PP_BASIC_H

#if defined(__STDC_VERSION__) || (defined(_MSC_VER) && _MSC_VER >= 1800)
    #include <stdbool.h>
#else
    typedef unsigned char _Bool;
    #ifndef __cplusplus
        #define bool _Bool
        #define false ((_Bool)0)
        #define true ((_Bool)1)
    #endif
#endif

#undef INLINE
#undef FORCEINLINE
#if defined(_MSC_VER)
    #if _MSC_VER < 1700
        #define INLINE __inline
        #define FORCEINLINE __forceinline
    #else
        #define INLINE inline
        #define FORCEINLINE __forceinline
    #endif
#elif defined(__GNUC__)
    #define INLINE inline
    #define FORCEINLINE __attribute__ ((alwaysinline))
#endif

#ifdef __cplusplus
    #define KARBONATOR_PP_C_LINKAGE_BEGIN() extern "C" {    
#else
    #define KARBONATOR_PP_C_LINKAGE_BEGIN()
#endif
#ifdef __cplusplus
    #define KARBONATOR_PP_C_LINKAGE_END() }
#else
    #define KARBONATOR_PP_C_LINKAGE_END()
#endif

#ifdef KARBONATOR_EXPORT_DLL
    #if defined(_MSC_VER)
        #define KARBONATOR_PP_DLL_API __declspec(dllexport)
    #elif defined(__GNUC__)
        #define KARBONATOR_PP_DLL_API __attribute__ ((dllexport))
    #else
        #define KARBONATOR_PP_DLL_API
    #endif
#else
    #if defined(_MSC_VER)
        #define KARBONATOR_PP_DLL_API __declspec(dllimport)
    #elif defined(__GNUC__)
        #define KARBONATOR_PP_DLL_API __attribute__ ((dllimport))
    #else
        #define KARBONATOR_PP_DLL_API
    #endif
#endif

/*typedef int (*karbonator_CopyConstructFuncPtr) (const void* pSrc);*/

/*typedef int (*karbonator_DestructFuncPtr) (void* pTarget);*/

/*typedef int (*karbonator_CopyAssignmentFuncPtr) (const void* pRhs);*/

#endif
