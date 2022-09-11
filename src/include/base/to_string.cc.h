
#ifndef __cxz_to_string_cc_h__
#define __cxz_to_string_cc_h__

#include "to_string.h"
#include <sstream>

namespace CNORXZ
{
    template <typename T>
    String toString(const T& a)
    {
	std::stringstream ss;
	ss << a;
	return ss.str();
    }

    template <typename T>
    String toString<vector<T>>(const vector& a)
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
    String toString<std::array<T,N>>(const std::array<T,N>& a)
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
    String toString<DType>(const DType& a)
    {
	return a.str();
    }

}

#endif
