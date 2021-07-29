
#ifndef __cxz_allocator__
#define __cxz_allocator__

#include <cstdlib>
#include <new>
#include <vector>
#include <cstdint>
#include <cassert>
#include <iostream>

#define MIB_SIZE 1024*1024 // 1MiB
#define WARN_SIZE MIB_SIZE*100 // 100 MiB

namespace CNORXZInternal
{

    template <typename T>
    struct Allocator
    {
	typedef T value_type;
	static constexpr size_t type_size = sizeof(value_type);	
	static constexpr size_t N = 32;

        struct VX
        {
            alignas(N) char x[N];
        };
        
	Allocator() = default;

	template <typename U>
	Allocator(const Allocator<U>& x) {}

	T* allocate(size_t n)
	{
	    const size_t nn = n*type_size;
            if(nn >= WARN_SIZE){
                std::cout << __func__ << ": WARNING: allocating " << nn/(MIB_SIZE) << " MiB" << std::endl;
            }
            const size_t off = nn%N;
            const size_t nnx = (off == 0) ? nn : nn + N - off;
            const size_t nnd = nnx/N;
            VX* vx = new VX[nnd];
	    return reinterpret_cast<T*>(vx);
	}

	void deallocate(T* p, size_t n)
	{
            VX* vx = reinterpret_cast<VX*>(p);
            delete [] vx;
	}
    };

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

    
} // namespace CNORXZInternal

namespace CNORXZ
{
    template <typename T>
    using vector = std::vector<T,CNORXZInternal::Allocator<T>>;

    template <typename T>
    inline std::vector<T> toStdVec(const vector<T>& v)
    {
        return std::vector<T>(v.begin(), v.end());
    }

    template <typename T>
    inline vector<T> toMatVec(const std::vector<T>& v)
    {
        return vector<T>(v.begin(), v.end());
    }
    
} // namespace CNORXZ

#endif
