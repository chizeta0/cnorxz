
#ifndef __ma_allocator__
#define __ma_allocator__

#include <cstdlib>
#include <new>
#include <vector>
#include <cstdint>
#include <cassert>
#include <iostream>

namespace MultiArrayHelper
{

    template <typename T>
    struct Allocator
    {
	typedef T value_type;
	static constexpr size_t type_size = sizeof(value_type);	
	static constexpr size_t N = 32;
	
	Allocator() = default;

	template <typename U>
	Allocator(const Allocator<U>& x) {}

	T* allocate(size_t n)
	{
	    //constexpr size_t NN = N*type_size;
	    const size_t nn = n*type_size;
	    if(n > static_cast<size_t>(-1-N)) throw std::bad_alloc();
	    auto p = static_cast<char*>(std::malloc(nn + N));
	    if(not p) throw std::bad_alloc();
	    auto ip = reinterpret_cast<std::intptr_t>(p);
	    auto diff = N - (ip % N);
	    p += diff;
	    //auto ipx = reinterpret_cast<std::intptr_t>(p);
	    //std::cout << "ixp: " << ipx << std::endl;
	    //std::cout << "ixp %: " << ipx % N << std::endl;
	    auto ps = reinterpret_cast<std::intptr_t*>(p);
	    ps[-1] = diff;
	    return reinterpret_cast<T*>(p);
	}

	void deallocate(T* p, size_t n)
	{
	    auto ps = reinterpret_cast<std::intptr_t*>(p);
	    std::free(reinterpret_cast<char*>(p)-ps[-1]);
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

    
} // namespace MultiArrayHelper

namespace MultiArrayTools
{
    template <typename T>
    using vector = std::vector<T,MultiArrayHelper::Allocator<T>>;

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
    
} // namespace MultiArrayTools

#endif
