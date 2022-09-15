
#ifndef __cxz_to_string_h__
#define __cxz_to_string_h__

#include "types.h"

namespace CNORXZ
{
    template <typename T>
    struct ToString
    {
	static String func(const T& a);
    };

    template <>
    struct ToString<String>
    {
	static String func(const String& a);
    };
    
    template <typename T>
    struct ToString<Vector<T>>
    {
	static String func(const Vector<T>& a);
    };

    template <typename T, SizeT N>
    struct ToString<Arr<T,N>>
    {
	static String func(const Arr<T,N>& a);
    };

    template <>
    struct ToString<DType>
    {
	static String func(const DType& a);
    };

    template <typename T>
    String toString(const T& a);

}

#endif
