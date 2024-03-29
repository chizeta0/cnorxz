// -*- C++ -*-
/**
   
   @file include/base/to_string.cc.h
   @brief String converter implementation

   Implementation of functions that convert a given object/type to a string
   
   Copyright (c) 2022 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_to_string_cc_h__
#define __cxz_to_string_cc_h__

#include "to_string.h"
#include "iter.h"
#include <sstream>

namespace CNORXZ
{
    template <typename T>
    String ToString<T>::func(const T& a)
    {
	std::stringstream ss;
	ss << a;
	return ss.str();
    }

    template <typename T>
    String ToString<Vector<T>>::func(const Vector<T>& a)
    {
	std::stringstream ss;
	ss << "[";
	if(a.size() == 0){
	    ss << "]";
	}
	else {
	    auto it = a.begin();
	    for(; it != a.end()-1; ++it){
		ss << toString(*it) << ",";
	    }
	    ss << toString(*it) << "]";
	}
	return ss.str();
    }

    template <typename T, size_t N>
    String ToString<Arr<T,N>>::func(const Arr<T,N>& a)
    {
	std::stringstream ss;
	ss << "(";
	if constexpr(N == 0){
	    ss << ")";
	}
	else {
	    auto it = a.begin();
	    for(; it != a.end()-1; ++it){
		ss << toString(*it) << ",";
	    }
	    ss << toString(*it) << ")";
	}
	return ss.str();
    }

    template <typename... Ts>
    String ToString<Tuple<Ts...>>::func(const Tuple<Ts...>& t)
    {
	const String blim = "(";
	const String elim = ")";
	const String dlim = ",";
	return iter<1,sizeof...(Ts)>
	    ( [&](auto i) { return toString(std::get<i>(t)); },
	      [&](const auto&... xs) {
		  return blim + toString(std::get<0>(t)) + ( (dlim + xs) + ... ) + elim;
	      } );
    }

    template <typename T, typename S>
    String ToString<std::pair<T,S>>::func(const std::pair<T,S>& p)
    {
	return String("(") + toString(p.first) + "," + toString(p.second) + ")";
    }

    template <typename T, typename S>
    String ToString<std::map<T,S>>::func(const std::map<T,S>& p)
    {
	std::stringstream ss;
	ss << "{";
	if(p.size() == 0){
	    ss << "}";
	}
	else {
	    auto it = p.begin();
	    auto e = p.end();
	    e--;
	    for(; it != e; ++it){
		ss << toString(it->first) << ":" << toString(it->second) << ",";
	    }
	    ss << toString(it->first) << ":" << toString(it->second) << "}";
	}
	return ss.str();
    }

    template <typename T>
    String toString(const T& a)
    {
	return ToString<T>::func(a);
    }
}

#endif
