
#ifndef __cxz_avx_h__
#define __cxz_avx_h__

#include <immintrin.h>

#include "base/base.h"

namespace CNORXZ
{
    namespace AVX
    {
	static constexpr SizeT ND = AVX_VSIZE/sizeof(Double);
	static constexpr SizeT NF = AVX_VSIZE/sizeof(float);
    }

    template <>
    struct PlusCC<Double,Double,AVX::ND>
    {
	static inline decltype(auto)
	eval(const Consecutive<Double,AVX::ND>& a, const Consecutive<Double,AVX::ND>& b);

	static inline decltype(auto)
	aeval(Consecutive<Double,AVX::ND>& a, const Consecutive<Double,AVX::ND>& b);
    };

    template <typename X>
    struct PlusCX<Double,X,AVX::ND>
    {
	static inline decltype(auto)
	eval(const Consecutive<Double,AVX::ND>& a, const X& b);

	static inline decltype(auto)
	aeval(Consecutive<Double,AVX::ND>& a, const X& b);

    	static inline decltype(auto)
	eval(const X& a, const Consecutive<Double,AVX::ND>& b);
    };

    template <>
    struct MinusCC<Double,Double,AVX::ND>
    {
	static inline decltype(auto)
	eval(const Consecutive<Double,AVX::ND>& a, const Consecutive<Double,AVX::ND>& b);

	static inline decltype(auto)
	aeval(Consecutive<Double,AVX::ND>& a, const Consecutive<Double,AVX::ND>& b);
    };

    template <typename X>
    struct MinusCX<Double,X,AVX::ND>
    {
	static inline decltype(auto)
	eval(const Consecutive<Double,AVX::ND>& a, const X& b);

	static inline decltype(auto)
	aeval(Consecutive<Double,AVX::ND>& a, const X& b);

    	static inline decltype(auto)
	eval(const X& a, const Consecutive<Double,AVX::ND>& b);
    };

    template <>
    struct MultipliesCC<Double,Double,AVX::ND>
    {
	static inline decltype(auto)
	eval(const Consecutive<Double,AVX::ND>& a, const Consecutive<Double,AVX::ND>& b);

	static inline decltype(auto)
	aeval(Consecutive<Double,AVX::ND>& a, const Consecutive<Double,AVX::ND>& b);
    };

    template <typename X>
    struct MultipliesCX<Double,X,AVX::ND>
    {
	static inline decltype(auto)
	eval(const Consecutive<Double,AVX::ND>& a, const X& b);

	static inline decltype(auto)
	aeval(Consecutive<Double,AVX::ND>& a, const X& b);

    	static inline decltype(auto)
	eval(const X& a, const Consecutive<Double,AVX::ND>& b);
    };

    template <>
    struct DividesCC<Double,Double,AVX::ND>
    {
	static inline decltype(auto)
	eval(const Consecutive<Double,AVX::ND>& a, const Consecutive<Double,AVX::ND>& b);

	static inline decltype(auto)
	aeval(Consecutive<Double,AVX::ND>& a, const Consecutive<Double,AVX::ND>& b);
    };

    template <typename X>
    struct DividesCX<Double,X,AVX::ND>
    {
	static inline decltype(auto)
	eval(const Consecutive<Double,AVX::ND>& a, const X& b);

	static inline decltype(auto)
	aeval(Consecutive<Double,AVX::ND>& a, const X& b);

    	static inline decltype(auto)
	eval(const X& a, const Consecutive<Double,AVX::ND>& b);
    };
}

#endif
