
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
	auto it = a.begin();
	for(; it != a.end()-1; ++it){
	    ss << toString(*it) << ",";
	}
	ss << toString(*it) << "]";
	return ss.str();
    }

    template <typename T, size_t N>
    String ToString<Arr<T,N>>::func(const Arr<T,N>& a)
    {
	std::stringstream ss;
	ss << "(";
	auto it = a.begin();
	for(; it != a.end()-1; ++it){
	    ss << toString(*it) << ",";
	}
	ss << toString(*it) << ")";
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

    template <typename T>
    String toString(const T& a)
    {
	return ToString<T>::func(a);
    }
}

#endif
