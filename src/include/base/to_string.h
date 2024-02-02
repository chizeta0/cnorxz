// -*- C++ -*-
/**
   
   @file include/base/to_string.h
   @brief String converter

   Declaration of functions that convert a given object/type to a string
   
   Copyright (c) 2022 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_to_string_h__
#define __cxz_to_string_h__

#include "types.h"

namespace CNORXZ
{
    /** ***
	Generic cast to string
	@tparam T type to be casted
     */
    template <typename T>
    struct ToString
    {
	/** cast to string
	    @param a object to be casted
	 */
	static String func(const T& a);
    };

    /** ***
	Specialization of ToString for strings
     */
    template <>
    struct ToString<String>
    {
	/** cast to string
	    @param a string to be casted
	 */
	static String func(const String& a);
    };
    
    /** ***
	Specialization of ToString for vectors
	@tparam T vector element type
     */
    template <typename T>
    struct ToString<Vector<T>>
    {
	/** cast to string
	    @param a vector to be casted
	 */
	static String func(const Vector<T>& a);
    };

    /** ***
	Specialization of ToString for arrays
	@tparam T array element type
	@tparam N array size
     */
    template <typename T, SizeT N>
    struct ToString<Arr<T,N>>
    {
	/** cast to string
	    @param a array to be casted
	 */
	static String func(const Arr<T,N>& a);
    };

    /** ***
	Specialization of ToString for tuples
	@tparam Ts tuple element types
     */
    template <typename... Ts>
    struct ToString<Tuple<Ts...>>
    {
	/** cast to string
	    @param a tuple to be casted
	 */
	static String func(const Tuple<Ts...>& t);
    };

    /** ***
	Specialization of ToString for pairs
	@tparam T first element type
	@tparam S second element type
     */
    template <typename T, typename S>
    struct ToString<std::pair<T,S>>
    {
	/** cast to string
	    @param a pair to be casted
	 */
	static String func(const std::pair<T,S>& t);
    };

    /** ***
	Specialization of ToString for maps
	@tparam T key type
	@tparam S value type
     */
    template <typename T, typename S>
    struct ToString<std::map<T,S>>
    {
	/** cast to string
	    @param a map to be casted
	 */
	static String func(const std::map<T,S>& t);
    };

    /** ***
	Specialization of ToString for DType
     */
    template <>
    struct ToString<DType>
    {
	/** cast to string
	    @param a DType to be casted
	 */
	static String func(const DType& a);
    };

    /** wrapper function for ToString
	@tparam T type to be casted
	@param a object to be casted
     */
    template <typename T>
    String toString(const T& a);

}

#endif
