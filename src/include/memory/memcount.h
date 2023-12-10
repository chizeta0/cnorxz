// -*- C++ -*-
/**
   
   @file include/memory/memcount.h
   @brief ...


   Copyright (c) 2022 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_memcount_h__
#define __cxz_memcount_h__

#include "base/types.h"

namespace CNORXZ
{
    /** *****
	Static class to track memory usage by cnorxz types
	The usage variable can be changed only be an Allocator instance

	@see Allocator
     **/
    class MemCount
    {
    private:
	static SizeT sMemUsage; /**< current memory usage (bytes) */ 

	/** increas memory usage
	    @param x number of bytes
	 */
	static void add(SizeT x);// { sMemUsage += x; }
	
	/** decreas memory usage
	    @param x number of bytes
	 */
	static void sub(SizeT x);// { sMemUsage -= x; }
	
    public:
	/** no instance construction (static) */
	MemCount() = delete;

	/** return current memory usage (bytes) */
	static SizeT usage();

	template <typename T>
	friend class Allocator;
    };

} // namespace CNORXZ

#endif
