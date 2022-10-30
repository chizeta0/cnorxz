
#ifndef __cxz_avx_cc_h__
#define __cxz_avx_cc_h__

#include "avx.h"

namespace CNORXZ
{
    /***********************
     *   PlusCC / PlusCX   *
     ***********************/
    
    constexpr decltype(auto) PlusCC<Double,Double,ND>::eval(const Consecutive<Double,ND>& a,
							    const Consecutive<Double,ND>& b)
    {
	Consecutive<Double,ND> o;
	__m256d av = _mm256_load_pd(a.mD);
	__m256d bv = _mm256_load_pd(b.mD);
	__m256d ov = _mm256_add_pd(av, bv);
	_mm256_store_pd(o.mD, ov);
	return o;
    }
    
    constexpr decltype(auto) PlusCC<Double,Double,ND>::aeval(Consecutive<Double,ND>& a,
							     const Consecutive<Double,ND>& b)
    {
	__m256d av = _mm256_load_pd(a.mD);
	__m256d bv = _mm256_load_pd(b.mD);
	__m256d ov = _mm256_add_pd(av, bv);
	_mm256_store_pd(a.mD, ov);
	return a;
    }

    template <typename X>
    static constexpr decltype(auto)
    PlusCX<Double,X,ND>::eval(const Consecutive<Double,ND>& a, const X& b)
    {
	Consecutive<Double,ND> o;
	__m256d av = _mm256_load_pd(a.mD);
	__m256d bv = _mm256_set1_pd( static_cast<Double>(b) );
	__m256d ov = _mm256_add_pd(av, bv);
	_mm256_store_pd(o.mD, ov);
	return o;
    }

    template <typename X>
    static constexpr decltype(auto)
    PlusCX<Double,X,ND>::aeval(Consecutive<Double,ND>& a, const X& b)
    {
	__m256d av = _mm256_load_pd(a.mD);
	__m256d bv = _mm256_set1_pd( static_cast<Double>(b) );
	__m256d ov = _mm256_add_pd(av, bv);
	_mm256_store_pd(a.mD, ov);
	return a;
    }

    template <typename X>
    static constexpr decltype(auto)
    PlusCX<Double,X,ND>::eval(const X& a, const Consecutive<Double,ND>& b)
    {
	Consecutive<Double,ND> o;
	__m256d av = _mm256_set1_pd( static_cast<Double>(a) );
	__m256d bv = _mm256_load_pd(b.mD);
	__m256d ov = _mm256_add_pd(av, bv);
	_mm256_store_pd(o.mD, ov);
	return o;
    }

    /*************************
     *   MinusCC / MinusCX   *
     *************************/

    constexpr decltype(auto) MinusCC<Double,Double,ND>::eval(const Consecutive<Double,ND>& a,
							     const Consecutive<Double,ND>& b)
    {
	Consecutive<Double,ND> o;
	__m256d av = _mm256_load_pd(a.mD);
	__m256d bv = _mm256_load_pd(b.mD);
	__m256d ov = _mm256_sub_pd(av, bv);
	_mm256_store_pd(o.mD, ov);
	return o;
    }
    
    constexpr decltype(auto) MinusCC<Double,Double,ND>::aeval(Consecutive<Double,ND>& a,
							      const Consecutive<Double,ND>& b)
    {
	__m256d av = _mm256_load_pd(a.mD);
	__m256d bv = _mm256_load_pd(b.mD);
	__m256d ov = _mm256_sub_pd(av, bv);
	_mm256_store_pd(a.mD, ov);
	return a;
    }

    template <typename X>
    static constexpr decltype(auto)
    MinusCX<Double,X,ND>::eval(const Consecutive<Double,ND>& a, const X& b)
    {
	Consecutive<Double,ND> o;
	__m256d av = _mm256_load_pd(a.mD);
	__m256d bv = _mm256_set1_pd( static_cast<Double>(b) );
	__m256d ov = _mm256_sub_pd(av, bv);
	_mm256_store_pd(o.mD, ov);
	return o;
    }

    template <typename X>
    static constexpr decltype(auto)
    MinusCX<Double,X,ND>::aeval(Consecutive<Double,ND>& a, const X& b)
    {
	__m256d av = _mm256_load_pd(a.mD);
	__m256d bv = _mm256_set1_pd( static_cast<Double>(b) );
	__m256d ov = _mm256_sub_pd(av, bv);
	_mm256_store_pd(a.mD, ov);
	return a;
    }

    template <typename X>
    static constexpr decltype(auto)
    MinusCX<Double,X,ND>::eval(const X& a, const Consecutive<Double,ND>& b)
    {
	Consecutive<Double,ND> o;
	__m256d av = _mm256_set1_pd( static_cast<Double>(a) );
	__m256d bv = _mm256_load_pd(b.mD);
	__m256d ov = _mm256_sub_pd(av, bv);
	_mm256_store_pd(o.mD, ov);
	return o;
    }

    /***********************************
     *   MultipliesCC / MultipliesCX   *
     ***********************************/

    constexpr decltype(auto) MultipliesCC<Double,Double,ND>::eval(const Consecutive<Double,ND>& a,
								  const Consecutive<Double,ND>& b)
    {
	Consecutive<Double,ND> o;
	__m256d av = _mm256_load_pd(a.mD);
	__m256d bv = _mm256_load_pd(b.mD);
	__m256d ov = _mm256_mul_pd(av, bv);
	_mm256_store_pd(o.mD, ov);
	return o;
    }
    
    constexpr decltype(auto) MultipliesCC<Double,Double,ND>::aeval(Consecutive<Double,ND>& a,
								   const Consecutive<Double,ND>& b)
    {
	__m256d av = _mm256_load_pd(a.mD);
	__m256d bv = _mm256_load_pd(b.mD);
	__m256d ov = _mm256_mul_pd(av, bv);
	_mm256_store_pd(a.mD, ov);
	return a;
    }

    template <typename X>
    static constexpr decltype(auto)
    MultipliesCX<Double,X,ND>::eval(const Consecutive<Double,ND>& a, const X& b)
    {
	Consecutive<Double,ND> o;
	__m256d av = _mm256_load_pd(a.mD);
	__m256d bv = _mm256_set1_pd( static_cast<Double>(b) );
	__m256d ov = _mm256_mul_pd(av, bv);
	_mm256_store_pd(o.mD, ov);
	return o;
    }

    template <typename X>
    static constexpr decltype(auto)
    MultipliesCX<Double,X,ND>::aeval(Consecutive<Double,ND>& a, const X& b)
    {
	__m256d av = _mm256_load_pd(a.mD);
	__m256d bv = _mm256_set1_pd( static_cast<Double>(b) );
	__m256d ov = _mm256_mul_pd(av, bv);
	_mm256_store_pd(a.mD, ov);
	return a;
    }

    template <typename X>
    static constexpr decltype(auto)
    MultipliesCX<Double,X,ND>::eval(const X& a, const Consecutive<Double,ND>& b)
    {
	Consecutive<Double,ND> o;
	__m256d av = _mm256_set1_pd( static_cast<Double>(a) );
	__m256d bv = _mm256_load_pd(b.mD);
	__m256d ov = _mm256_mul_pd(av, bv);
	_mm256_store_pd(o.mD, ov);
	return o;
    }
    
    
    /*****************************
     *   DividesCC / DividesCX   *
     *****************************/

    constexpr decltype(auto) DividesCC<Double,Double,ND>::eval(const Consecutive<Double,ND>& a,
							       const Consecutive<Double,ND>& b)
    {
	Consecutive<Double,ND> o;
	__m256d av = _mm256_load_pd(a.mD);
	__m256d bv = _mm256_load_pd(b.mD);
	__m256d ov = _mm256_div_pd(av, bv);
	_mm256_store_pd(o.mD, ov);
	return o;
    }
    
    constexpr decltype(auto) DividesCC<Double,Double,ND>::aeval(Consecutive<Double,ND>& a,
								const Consecutive<Double,ND>& b)
    {
	__m256d av = _mm256_load_pd(a.mD);
	__m256d bv = _mm256_load_pd(b.mD);
	__m256d ov = _mm256_div_pd(av, bv);
	_mm256_store_pd(a.mD, ov);
	return a;
    }

    template <typename X>
    static constexpr decltype(auto)
    DividesCX<Double,X,ND>::eval(const Consecutive<Double,ND>& a, const X& b)
    {
	Consecutive<Double,ND> o;
	__m256d av = _mm256_load_pd(a.mD);
	__m256d bv = _mm256_set1_pd( static_cast<Double>(b) );
	__m256d ov = _mm256_div_pd(av, bv);
	_mm256_store_pd(o.mD, ov);
	return o;
    }

    template <typename X>
    static constexpr decltype(auto)
    DividesCX<Double,X,ND>::aeval(Consecutive<Double,ND>& a, const X& b)
    {
	__m256d av = _mm256_load_pd(a.mD);
	__m256d bv = _mm256_set1_pd( static_cast<Double>(b) );
	__m256d ov = _mm256_div_pd(av, bv);
	_mm256_store_pd(a.mD, ov);
	return a;
    }

    template <typename X>
    static constexpr decltype(auto)
    DividesCX<Double,X,ND>::eval(const X& a, const Consecutive<Double,ND>& b)
    {
	Consecutive<Double,ND> o;
	__m256d av = _mm256_set1_pd( static_cast<Double>(a) );
	__m256d bv = _mm256_load_pd(b.mD);
	__m256d ov = _mm256_div_pd(av, bv);
	_mm256_store_pd(o.mD, ov);
	return o;
    }
    
}

#endif
