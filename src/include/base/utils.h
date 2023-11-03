/**
   
   @file include/base/utils.h
   @brief utilities

   Copyright (c) 2022 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_utils_h__
#define __cxz_utils_h__

#include <cstdlib>
#include "types.h"

namespace CNORXZ
{
    template <typename T, SizeT N>
    Vector<T> toVec(const Arr<T,N>& a)
    {
	return iter<0,N>( [&](auto i) { return std::get<i>(a); },
			  [](const auto&... e) { return Vector<T> { e... }; } );
    }

    template <typename T>
    Vector<T> toVec(const Vector<T>& a)
    {
	return a;
    }

    template <typename T>
    Vector<T> toVec(const T& a)
    {
	return Vector<T> { a };
    }
    
    template <typename T, SizeT N1, SizeT N2>
    constexpr Arr<T,N1+N2> cat2(const Arr<T,N1>& a1, const Arr<T,N2>& a2)
    {
	return iter<0,N1+N2>
	    ( [&](auto i) { if constexpr(i < N1) { return std::get<i>(a1); } else { return std::get<i-N1>(a2); } },
	      [](const auto&... e) { return Arr<T,N1+N2> { e... }; } );
    }

    template <typename T, SizeT N1>
    constexpr Arr<T,N1+1> cat2(const Arr<T,N1>& a1, const T& a2)
    {
	return iter<0,N1>
	    ( [&](auto i) { return std::get<i>(a1); },
	      [&](const auto&... e) { return Arr<T,N1+1> { e..., a2 }; } );
    }

    template <typename T, SizeT N1>
    constexpr Arr<T,N1+1> cat2(const T& a1, const Arr<T,N1>& a2)
    {
	return iter<0,N1>
	    ( [&](auto i) { return std::get<i>(a2); },
	      [&](const auto&... e) { return Arr<T,N1+1> { a1, e... }; } );
    }

    template <typename T>
    constexpr Arr<T,2> cat2(const T& a1, const T& a2)
    {
	return Arr<T,2> { a1, a2 };
    }

    template <typename T, SizeT N2>
    Vector<T> cat2(const Vector<T>& a1, const Arr<T,N2>& a2)
    {
	Vector<T> o(a1.size()+N2);
	std::copy(a1.begin(), a1.end(), o.begin());
	std::copy(a2.begin(), a2.end(), o.begin()+a1.size());
	return o;
    }

    template <typename T, SizeT N1>
    Vector<T> cat2(const Arr<T,N1>& a1, const Vector<T>& a2)
    {
	Vector<T> o(N1+a2.size());
	std::copy(a1.begin(), a1.end(), o.begin());
	std::copy(a2.begin(), a2.end(), o.begin()+N1);
	return o;
    }

    template <typename T>
    Vector<T> cat2(const Vector<T>& a1, const Vector<T>& a2)
    {
	Vector<T> o(a1.size()+a2.size());
	std::copy(a1.begin(), a1.end(), o.begin());
	std::copy(a2.begin(), a2.end(), o.begin()+a1.size());
	return o;
    }

    template <typename T>
    Vector<T> cat2(const Vector<T>& a1, const T& a2)
    {
	Vector<T> o(a1);
	o.push_back(a2);
	return o;
    }

    template <typename T, SizeT N1>
    Vector<T> cat2(const T& a1, const Vector<T>& a2)
    {
	Vector<T> o { a1 };
	o.insert(o.end(), a2.begin(), a2.end());
	return o;
    }

    template <typename T1, typename T2, typename... Ts>
    decltype(auto) concat(const T1& a1, const T2& a2, const Ts&... as)
    {
	if constexpr(sizeof...(Ts) != 0){
	    return cat2(a1, concat(a2, as...));
	}
	else {
	    return cat2(a1, a2);
	}
    }

    template <typename T, SizeT N>
    constexpr Arr<T,N> mul(const Arr<T,N>& a, const T& b)
    {
	return iter<0,N>( [&](auto i) { return std::get<i>(a) * b; },
			  [](const auto&... e) { return Arr<T,N> { e... }; } );
    }

    template <typename T>
    Vector<T> mul(const Vector<T>& a, const T& b)
    {
	Vector<T> o(a.size());
	std::transform(a.begin(), a.end(), o.begin(), [&](const auto& x) { return x*b; } );
	return o;
    }

    template <typename T>
    constexpr T mul(const T& a, const T& b)
    {
	return a*b;
    }

}

#endif
