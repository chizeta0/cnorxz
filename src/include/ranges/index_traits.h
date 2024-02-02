// -*- C++ -*-
/**
   
   @file include/ranges/index_traits.h
   @brief index traits

   Copyright (c) 2023 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_index_traits__
#define __cxz_index_traits__

namespace CNORXZ
{
    /** *****
	check if given type is index-type

	@tparam I type to be checked
     **/
    template <class I>
    struct is_index
    { static constexpr bool value = std::is_base_of<IndexInterface<I,typename I::MetaType>,I>::value; };
    
    /** *****
	check if given type is index-type with a static range size

	@tparam I type to be checked
     **/
    template <class I>
    struct index_has_const_size
    { static constexpr bool value = false; };

    /** *****
	return static range size of an index (0 if there is no static size)

	@tparam I type to be checked
     **/
    template <class I>
    struct index_const_size
    { static constexpr SizeT value = 0; };

    /** *****
	return the static dimension of an index (1 if index is not statically multi-dimensional)
	
	@tparam I type to be checked
     **/
    template <class I>
    struct index_dim
    { static constexpr SizeT value = 1; };

    /** *****
	check if given type is index-type with sub-indices

	@tparam I type to be checked
     **/
    template <class T>
    struct has_sub
    { static constexpr bool value = false; };

    /** *****
	check if given type is index-type with static sub-indices

	@tparam I type to be checked
     **/
    template <class T>
    struct has_static_sub
    { static constexpr bool value = false; };

    /** *****
	check if given type is a multi-index-type 

	@tparam I type to be checked
     **/
    template <class I>
    struct index_is_multi
    { static constexpr bool value = false; };

    /** *****
	check if given type is index-type that can be used in an expression,
	i.e. has a member function xpr()

	@tparam I type to be checked
     **/
    template <class I>
    struct index_expression_exists
    { static constexpr bool value = false; };

}

#endif
