// -*- C++ -*-
/**
   
   @file include/memory/allocator.cc.h
   @brief Allocator template member function implementation

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_allocator_cc_h__
#define __cxz_allocator_cc_h__

#include "allocator.h"
#include "memcount.h"

namespace CNORXZ
{

    template <typename T>
    T* Allocator<T>::allocate(SizeT n)
    {
	const SizeT nn = n*type_size;
	MemCount::add(nn);
	const SizeT off = nn%N;
	const SizeT nnx = (off == 0) ? nn : nn + N - off;
	const SizeT nnd = nnx/N;
	VX* vx = new VX[nnd];
	return reinterpret_cast<T*>(vx);
    }

    template <typename T>
    void Allocator<T>::deallocate(T* p, SizeT n)
    {
	const SizeT nn = n*type_size;
	MemCount::sub(nn);
	VX* vx = reinterpret_cast<VX*>(p);
	delete [] vx;
    }

    template <class T, class U>
    bool operator==(const Allocator<T>& a, const Allocator<U>& b)
    {
	return true;
    }

    template <class T, class U>
    bool operator!=(const Allocator<T>& a, const Allocator<U>& b)
    {
	return false;
    }

}

#endif
