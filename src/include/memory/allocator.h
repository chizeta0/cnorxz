
#ifndef __cxz_allocator__
#define __cxz_allocator__

#include <cstdlib>
#include <new>
#include <vector>
#include <cstdint>
#include <cassert>
#include <iostream>
#include "base/types.h"

#define MIB_SIZE 1024*1024 // 1MiB
#define WARN_SIZE MIB_SIZE*100 // 100 MiB

namespace CNORXZ
{

    template <typename T>
    class Allocator
    {
    public:

	typedef T value_type;
	
	static constexpr SizeT type_size = sizeof(T);	
	static constexpr SizeT N = 32; // get from environment!!!

        struct VX
        {
            alignas(N) char x[N];
        };
        
	Allocator() = default;

	template <typename U>
	Allocator(const Allocator<U>& x) {}

	T* allocate(SizeT n);
	void deallocate(T* p, SizeT n);
    };

    template <class T, class U>
    bool operator==(const Allocator<T>& a, const Allocator<U>& b);

    template <class T, class U>
    bool operator!=(const Allocator<T>& a, const Allocator<U>& b);

    
} // namespace CNORXZ

#endif
