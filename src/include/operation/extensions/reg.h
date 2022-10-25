
#ifndef __cxz_reg_h__
#define __cxz_reg_h__

#include "base/base.h"

namespace CNORXZ
{
    // pseudo extension type to be returned if extension vector of
    // reuqired size is not available
    // no use of Arr = std::array here, since I want ensure that
    // it has exactly a memory size of N
    template <typename T, SizeT N>
    struct Consecutive
    {
	T mD[N];
    };

    // specialize for all kinds of available vector registers:
    template <typename T, SizeT N>
    struct MkConsecutive
    {
	static inline decltype(auto) make(const T* d);

	static inline decltype(auto) make(T* d);

	template <typename... Args>
	static inline decltype(auto) makeA(Args&&... args);
    };
    

    /****************************************
     *   consecutive generating functions   *
     ****************************************/

    template <typename T, class EPosT, SizeT... Is>
    inline decltype(auto) vregi(const T* d, const EPosT& pos, std::index_sequence<Is...> is);

    template <typename T, class EPosT>
    inline decltype(auto) vreg(const T* d, const EPosT& pos);

    template <typename T, class EPosT>
    inline decltype(auto) vreg(T* d, const EPosT& pos);

    /******************************
     *   basic operations: plus   *
     ******************************/
    
    template <typename T, SizeT N>
    constexpr Consecutive<T,N> operator+(const Consecutive<T,N>& a, const Consecutive<T,N>& b);

    template <typename T, SizeT N>
    constexpr Consecutive<T,N> operator+(const Consecutive<T,N>& a, const T& b);

    template <typename T, SizeT N>
    constexpr Consecutive<T,N> operator+(const T& a, const Consecutive<T,N>& b);

    template <typename T, SizeT N>
    constexpr Consecutive<T,N> operator+=(const Consecutive<T,N>& a);

    template <typename T, SizeT N>
    constexpr Consecutive<T,N> operator+=(const T& a);

    /*******************************
     *   basic operations: minus   *
     *******************************/

    template <typename T, SizeT N>
    constexpr Consecutive<T,N> operator-(const Consecutive<T,N>& a, const Consecutive<T,N>& b);

    template <typename T, SizeT N>
    constexpr Consecutive<T,N> operator-(const Consecutive<T,N>& a, const T& b);

    template <typename T, SizeT N>
    constexpr Consecutive<T,N> operator-(const T& a, const Consecutive<T,N>& b);

    template <typename T, SizeT N>
    constexpr Consecutive<T,N> operator-=(const Consecutive<T,N>& a);

    template <typename T, SizeT N>
    constexpr Consecutive<T,N> operator-=(const T& a);

    /***********************************
     *   basic operations: muliplies   *
     ***********************************/

    template <typename T, SizeT N>
    constexpr Consecutive<T,N> operator*(const Consecutive<T,N>& a, const Consecutive<T,N>& b);

    template <typename T, SizeT N>
    constexpr Consecutive<T,N> operator*(const Consecutive<T,N>& a, const T& b);

    template <typename T, SizeT N>
    constexpr Consecutive<T,N> operator*(const T& a, const Consecutive<T,N>& b);

    template <typename T, SizeT N>
    constexpr Consecutive<T,N> operator*=(const Consecutive<T,N>& a);

    template <typename T, SizeT N>
    constexpr Consecutive<T,N> operator*=(const T& a);

    /*********************************
     *   basic operations: divides   *
     *********************************/

    template <typename T, SizeT N>
    constexpr Consecutive<T,N> operator/(const Consecutive<T,N>& a, const Consecutive<T,N>& b);

    template <typename T, SizeT N>
    constexpr Consecutive<T,N> operator/(const Consecutive<T,N>& a, const T& b);

    template <typename T, SizeT N>
    constexpr Consecutive<T,N> operator/(const T& a, const Consecutive<T,N>& b);

    template <typename T, SizeT N>
    constexpr Consecutive<T,N> operator/=(const Consecutive<T,N>& a);

    template <typename T, SizeT N>
    constexpr Consecutive<T,N> operator/=(const T& a);

}

#endif
