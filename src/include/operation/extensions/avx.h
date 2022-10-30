
#ifndef __cxz_avx_h__
#define __cxz_avx_h__

#include <immintrin.h>

#include "base/base.h"

#define AVX_VSIZE (256/8)

namespace CNORXZ
{
    namespace AVX
    {
	static constexpr SizeT ND = AVX_VSIZE/sizeof(Double);
    }
    
    template <>
    struct PlusCC<Double,Double,ND>
    {
	static constexpr decltype(auto)
	eval(const Consecutive<Double,ND>& a, const Consecutive<Double,ND>& b);

	static constexpr decltype(auto)
	aeval(Consecutive<Double,ND>& a, const Consecutive<Double,ND>& b);
    };

    template <typename X>
    struct PlusCX<Double,X,ND>
    {
	static constexpr decltype(auto)
	eval(const Consecutive<Double,ND>& a, const X& b);

	static constexpr decltype(auto)
	aeval(Consecutive<Double,ND>& a, const X& b);

    	static constexpr decltype(auto)
	eval(const X& a, const Consecutive<Double,ND>& b);
    };

    template <>
    struct MinusCC<Double,Double,ND>
    {
	static constexpr decltype(auto)
	eval(const Consecutive<Double,ND>& a, const Consecutive<Double,ND>& b);

	static constexpr decltype(auto)
	aeval(Consecutive<Double,ND>& a, const Consecutive<Double,ND>& b);
    };

    template <typename X>
    struct MinusCX<Double,X,ND>
    {
	static constexpr decltype(auto)
	eval(const Consecutive<Double,ND>& a, const X& b);

	static constexpr decltype(auto)
	aeval(Consecutive<Double,ND>& a, const X& b);

    	static constexpr decltype(auto)
	eval(const X& a, const Consecutive<Double,ND>& b);
    };

    template <>
    struct MultipliesCC<Double,Double,ND>
    {
	static constexpr decltype(auto)
	eval(const Consecutive<Double,ND>& a, const Consecutive<Double,ND>& b);

	static constexpr decltype(auto)
	aeval(Consecutive<Double,ND>& a, const Consecutive<Double,ND>& b);
    };

    template <typename X>
    struct MultipliesCX<Double,X,ND>
    {
	static constexpr decltype(auto)
	eval(const Consecutive<Double,ND>& a, const X& b);

	static constexpr decltype(auto)
	aeval(Consecutive<Double,ND>& a, const X& b);

    	static constexpr decltype(auto)
	eval(const X& a, const Consecutive<Double,ND>& b);
    };

    template <>
    struct DividesCC<Double,Double,ND>
    {
	static constexpr decltype(auto)
	eval(const Consecutive<Double,ND>& a, const Consecutive<Double,ND>& b);

	static constexpr decltype(auto)
	aeval(Consecutive<Double,ND>& a, const Consecutive<Double,ND>& b);
    };

    template <typename X>
    struct DividesCX<Double,X,ND>
    {
	static constexpr decltype(auto)
	eval(const Consecutive<Double,ND>& a, const X& b);

	static constexpr decltype(auto)
	aeval(Consecutive<Double,ND>& a, const X& b);

    	static constexpr decltype(auto)
	eval(const X& a, const Consecutive<Double,ND>& b);
    };
}

#endif
