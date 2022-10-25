
#ifndef __cxz_avx_h__
#define __cxz_avx_h__

#include <immintrin.h>

#include "base/base.h"

#define AVX_VSIZE (256/8)

namespace CNORXZ
{
    namespace AVX
    {
	// define for all types that are defined in base/types.h
	struct ConsecutiveD
	{
	    __m256d mD;
	};

	struct ConsecutiveI
	{
	    __m256i mD;
	};
    }
    
    template <>
    struct MkConsecutive<Double,AVX_SIZE/sizeof(Double)>
    {
	static inline decltype(auto) make(const Double* d);

	static inline decltype(auto) make(Double* d);

	template <typename... Args>
	static inline decltype(auto) makeA(Args&&... args);
    };
    
    template <>
    struct MkConsecutive<Double,AVX_SIZE/sizeof(Int)>
    {
	static inline decltype(auto) make(const Int* d);

	static inline decltype(auto) make(Int* d);

	template <typename... Args>
	static inline decltype(auto) makeA(Args&&... args);
    };
    
}

#endif
