// -*- C++ -*-
/**
   
   @file include/operation/extensions/avx.cc.h
   @brief Register type implementaions for AVX.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_avx_cc_h__
#define __cxz_avx_cc_h__

#include "avx.h"

namespace CNORXZ
{
    /*=====================+
     |   PlusCC / PlusCX   |
     +=====================*/
    
    inline decltype(auto)
    PlusCC<Double,Double,AVX::ND>::eval(const Consecutive<Double,AVX::ND>& a,
					const Consecutive<Double,AVX::ND>& b)
    {
	Consecutive<Double,AVX::ND> o;
	__m256d av = _mm256_load_pd(a.mD);
	__m256d bv = _mm256_load_pd(b.mD);
	__m256d ov = _mm256_add_pd(av, bv);
	_mm256_store_pd(o.mD, ov);
	return o;
    }
    
    inline decltype(auto)
    PlusCC<Double,Double,AVX::ND>::aeval(Consecutive<Double,AVX::ND>& a,
					 const Consecutive<Double,AVX::ND>& b)
    {
	__m256d av = _mm256_load_pd(a.mD);
	__m256d bv = _mm256_load_pd(b.mD);
	__m256d ov = _mm256_add_pd(av, bv);
	_mm256_store_pd(a.mD, ov);
	return a;
    }

    template <typename X>
    inline decltype(auto)
    PlusCX<Double,X,AVX::ND>::eval(const Consecutive<Double,AVX::ND>& a,
				   const X& b)
    {
	Consecutive<Double,AVX::ND> o;
	__m256d av = _mm256_load_pd(a.mD);
	__m256d bv = _mm256_set1_pd( static_cast<Double>(b) );
	__m256d ov = _mm256_add_pd(av, bv);
	_mm256_store_pd(o.mD, ov);
	return o;
    }

    template <typename X>
    inline decltype(auto)
    PlusCX<Double,X,AVX::ND>::aeval(Consecutive<Double,AVX::ND>& a, const X& b)
    {
	__m256d av = _mm256_load_pd(a.mD);
	__m256d bv = _mm256_set1_pd( static_cast<Double>(b) );
	__m256d ov = _mm256_add_pd(av, bv);
	_mm256_store_pd(a.mD, ov);
	return a;
    }

    template <typename X>
    inline decltype(auto)
    PlusCX<Double,X,AVX::ND>::eval(const X& a,
				   const Consecutive<Double,AVX::ND>& b)
    {
	Consecutive<Double,AVX::ND> o;
	__m256d av = _mm256_set1_pd( static_cast<Double>(a) );
	__m256d bv = _mm256_load_pd(b.mD);
	__m256d ov = _mm256_add_pd(av, bv);
	_mm256_store_pd(o.mD, ov);
	return o;
    }

    /*=======================+
     |   MinusCC / MinusCX   |
     +=======================*/

    inline decltype(auto)
    MinusCC<Double,Double,AVX::ND>::eval(const Consecutive<Double,AVX::ND>& a,
					 const Consecutive<Double,AVX::ND>& b)
    {
	Consecutive<Double,AVX::ND> o;
	__m256d av = _mm256_load_pd(a.mD);
	__m256d bv = _mm256_load_pd(b.mD);
	__m256d ov = _mm256_sub_pd(av, bv);
	_mm256_store_pd(o.mD, ov);
	return o;
    }
    
    inline decltype(auto)
    MinusCC<Double,Double,AVX::ND>::aeval(Consecutive<Double,AVX::ND>& a,
					  const Consecutive<Double,AVX::ND>& b)
    {
	__m256d av = _mm256_load_pd(a.mD);
	__m256d bv = _mm256_load_pd(b.mD);
	__m256d ov = _mm256_sub_pd(av, bv);
	_mm256_store_pd(a.mD, ov);
	return a;
    }

    template <typename X>
    inline decltype(auto)
    MinusCX<Double,X,AVX::ND>::eval(const Consecutive<Double,AVX::ND>& a,
				    const X& b)
    {
	Consecutive<Double,AVX::ND> o;
	__m256d av = _mm256_load_pd(a.mD);
	__m256d bv = _mm256_set1_pd( static_cast<Double>(b) );
	__m256d ov = _mm256_sub_pd(av, bv);
	_mm256_store_pd(o.mD, ov);
	return o;
    }

    template <typename X>
    inline decltype(auto)
    MinusCX<Double,X,AVX::ND>::aeval(Consecutive<Double,AVX::ND>& a, const X& b)
    {
	__m256d av = _mm256_load_pd(a.mD);
	__m256d bv = _mm256_set1_pd( static_cast<Double>(b) );
	__m256d ov = _mm256_sub_pd(av, bv);
	_mm256_store_pd(a.mD, ov);
	return a;
    }

    template <typename X>
    inline decltype(auto)
    MinusCX<Double,X,AVX::ND>::eval(const X& a,
				    const Consecutive<Double,AVX::ND>& b)
    {
	Consecutive<Double,AVX::ND> o;
	__m256d av = _mm256_set1_pd( static_cast<Double>(a) );
	__m256d bv = _mm256_load_pd(b.mD);
	__m256d ov = _mm256_sub_pd(av, bv);
	_mm256_store_pd(o.mD, ov);
	return o;
    }

    /*=================================+
     |   MultipliesCC / MultipliesCX   |
     +=================================*/

    inline decltype(auto)
    MultipliesCC<Double,Double,AVX::ND>::eval(const Consecutive<Double,AVX::ND>& a,
					      const Consecutive<Double,AVX::ND>& b)
    {
	Consecutive<Double,AVX::ND> o;
	__m256d av = _mm256_load_pd(a.mD);
	__m256d bv = _mm256_load_pd(b.mD);
	__m256d ov = _mm256_mul_pd(av, bv);
	_mm256_store_pd(o.mD, ov);
	return o;
    }
    
    inline decltype(auto)
    MultipliesCC<Double,Double,AVX::ND>::aeval(Consecutive<Double,AVX::ND>& a,
					       const Consecutive<Double,AVX::ND>& b)
    {
	__m256d av = _mm256_load_pd(a.mD);
	__m256d bv = _mm256_load_pd(b.mD);
	__m256d ov = _mm256_mul_pd(av, bv);
	_mm256_store_pd(a.mD, ov);
	return a;
    }

    template <typename X>
    inline decltype(auto)
    MultipliesCX<Double,X,AVX::ND>::eval(const Consecutive<Double,AVX::ND>& a,
					 const X& b)
    {
	Consecutive<Double,AVX::ND> o;
	__m256d av = _mm256_load_pd(a.mD);
	__m256d bv = _mm256_set1_pd( static_cast<Double>(b) );
	__m256d ov = _mm256_mul_pd(av, bv);
	_mm256_store_pd(o.mD, ov);
	return o;
    }

    template <typename X>
    inline decltype(auto)
    MultipliesCX<Double,X,AVX::ND>::aeval(Consecutive<Double,AVX::ND>& a,
					  const X& b)
    {
	__m256d av = _mm256_load_pd(a.mD);
	__m256d bv = _mm256_set1_pd( static_cast<Double>(b) );
	__m256d ov = _mm256_mul_pd(av, bv);
	_mm256_store_pd(a.mD, ov);
	return a;
    }

    template <typename X>
    inline decltype(auto)
    MultipliesCX<Double,X,AVX::ND>::eval(const X& a,
					 const Consecutive<Double,AVX::ND>& b)
    {
	Consecutive<Double,AVX::ND> o;
	__m256d av = _mm256_set1_pd( static_cast<Double>(a) );
	__m256d bv = _mm256_load_pd(b.mD);
	__m256d ov = _mm256_mul_pd(av, bv);
	_mm256_store_pd(o.mD, ov);
	return o;
    }
    
    
    /*===========================+
     |   DividesCC / DividesCX   |
     +===========================*/

    inline decltype(auto)
    DividesCC<Double,Double,AVX::ND>::eval(const Consecutive<Double,AVX::ND>& a,
					   const Consecutive<Double,AVX::ND>& b)
    {
	Consecutive<Double,AVX::ND> o;
	__m256d av = _mm256_load_pd(a.mD);
	__m256d bv = _mm256_load_pd(b.mD);
	__m256d ov = _mm256_div_pd(av, bv);
	_mm256_store_pd(o.mD, ov);
	return o;
    }
    
    inline decltype(auto)
    DividesCC<Double,Double,AVX::ND>::aeval(Consecutive<Double,AVX::ND>& a,
					    const Consecutive<Double,AVX::ND>& b)
    {
	__m256d av = _mm256_load_pd(a.mD);
	__m256d bv = _mm256_load_pd(b.mD);
	__m256d ov = _mm256_div_pd(av, bv);
	_mm256_store_pd(a.mD, ov);
	return a;
    }

    template <typename X>
    inline decltype(auto)
    DividesCX<Double,X,AVX::ND>::eval(const Consecutive<Double,AVX::ND>& a,
				      const X& b)
    {
	Consecutive<Double,AVX::ND> o;
	__m256d av = _mm256_load_pd(a.mD);
	__m256d bv = _mm256_set1_pd( static_cast<Double>(b) );
	__m256d ov = _mm256_div_pd(av, bv);
	_mm256_store_pd(o.mD, ov);
	return o;
    }

    template <typename X>
    inline decltype(auto)
    DividesCX<Double,X,AVX::ND>::aeval(Consecutive<Double,AVX::ND>& a, const X& b)
    {
	__m256d av = _mm256_load_pd(a.mD);
	__m256d bv = _mm256_set1_pd( static_cast<Double>(b) );
	__m256d ov = _mm256_div_pd(av, bv);
	_mm256_store_pd(a.mD, ov);
	return a;
    }

    template <typename X>
    inline decltype(auto)
    DividesCX<Double,X,AVX::ND>::eval(const X& a,
				      const Consecutive<Double,AVX::ND>& b)
    {
	Consecutive<Double,AVX::ND> o;
	__m256d av = _mm256_set1_pd( static_cast<Double>(a) );
	__m256d bv = _mm256_load_pd(b.mD);
	__m256d ov = _mm256_div_pd(av, bv);
	_mm256_store_pd(o.mD, ov);
	return o;
    }
    
}

#endif
