// -*- C++ -*-
/**
   
   @file include/memory/allocator.h
   @brief Allocator declaration

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_allocator__
#define __cxz_allocator__

#include <cstdlib>
#include <new>
#include <vector>
#include <cstdint>
#include <cassert>
#include <iostream>
#include "base/types.h"
#include "base/intrin.h"

#define MIB_SIZE 1024*1024 // 1MiB
#define WARN_SIZE MIB_SIZE*100 // 100 MiB

namespace CNORXZ
{
    /** *****
	Allocator implementation
	Takes care of proper alignment
     */
    template <typename T>
    class Allocator
    {
    public:

	typedef T value_type;
	
	static constexpr SizeT type_size = sizeof(T); /** < type size */
	static constexpr SizeT N = MAX_VSIZE; /** < size of the larges available intrinsics vector */

	/** ***
	    Aligned data block type
	    The size equals the maximal intrinsics vectors size
	 */
        struct VX
        {
            alignas(N) char x[N];
        };

	/** default constructor */
	Allocator() = default;

	/** (copy) construct from allocator for different data type
	    @tparam U input data type
	    @param x input allocator
	 */
	template <typename U>
	Allocator(const Allocator<U>& x) {}

	/** allocate n element of type T
	    @param n number of elements
	 */
	T* allocate(SizeT n);

	/** deallocate n elements
	    @param p pointer to first element
	    @param n number of elements
	 */
	void deallocate(T* p, SizeT n);
    };

    /** compare two cnorxz allocators; equality check returns always true */
    template <class T, class U>
    bool operator==(const Allocator<T>& a, const Allocator<U>& b);

    /** compare two cnorxz allocators; unequality check returns always false */
    template <class T, class U>
    bool operator!=(const Allocator<T>& a, const Allocator<U>& b);

    
} // namespace CNORXZ

#endif
