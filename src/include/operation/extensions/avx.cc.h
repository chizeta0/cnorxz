
#ifndef __cxz_avx_cc_h__
#define __cxz_avx_cc_h__

#include "avx.h"

namespace CNORXZ
{
    inline decltype(auto) MkConsecutive<Double,AVX_SIZE/sizeof(Double)>::make(const Double* d)
    {
	return *reinterpret_cast<const AVX::ConsecutiveD*>( d );
    }
    
    inline decltype(auto) MkConsecutive<Double,AVX_SIZE/sizeof(Double)>::make(Double* d)
    {
	return *reinterpret_cast<AVX::ConsecutiveD*>( d );
    }
    
    template <typename... Args>
    inline decltype(auto) MkConsecutive<Double,AVX_SIZE/sizeof(Double)>::makeA(Args&&... args)
    {
	static_assert(sizeof...(Args) == AVX_SIZE/sizeof(Double),
		      "got inconsistent number of arguments");
	return AVX::ConsecutiveD { _mm256_setr_pd(args...); }
    }

    inline decltype(auto) MkConsecutive<Int,AVX_SIZE/sizeof(Int)>::make(const Int* d)
    {
	return *reinterpret_cast<const AVX::ConsecutiveI*>( d );
    }
    
    inline decltype(auto) MkConsecutive<Int,AVX_SIZE/sizeof(Int)>::make(Int* d)
    {
	return *reinterpret_cast<AVX::ConsecutiveI*>( d );
    }
    
    template <typename... Args>
    inline decltype(auto) MkConsecutive<Int,AVX_SIZE/sizeof(Int)>::makeA(Args&&... args)
    {
	static_assert(sizeof(Int) == 32/8, "lib error: Int size has changed");
	static_assert(sizeof...(Args) == AVX_SIZE/sizeof(Int),
		      "got inconsistent number of arguments");
	return AVX::ConsecutiveI { _mm256_setr_epi32(args...); }
    }

}

#endif
