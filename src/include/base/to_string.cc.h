
#ifndef __cxz_to_string_cc_h__
#define __cxz_to_string_cc_h__

#include "to_string.h"
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
    String ToString<vector<T>>::func(const vector& a)
    {
	std::stringstream ss;
	ss << "[";
	auto it = a.begin();
	for(; it != a.end()-1; ++it){
	    ss << *it << ",";
	}
	ss << *it << "]";
	return ss.str();
    }

    template <typename T, size_t N>
    String ToString<std::array<T,N>>::func(const std::array<T,N>& a)
    {
	std::stringstream ss;
	ss << "(";
	auto it = a.begin();
	for(; it != a.end()-1; ++it){
	    ss << *it << ",";
	}
	ss << *it << ")";
	return ss.str();
    }

    template <>
    String ToString<DType>::func(const DType& a)
    {
	return a.str();
    }

    template <typename T>
    String toString(const T& a)
    {
	return ToString::func(a);
    }
}

#endif
