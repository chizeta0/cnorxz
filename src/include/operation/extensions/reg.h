
#ifndef __cxz_reg_h__
#define __cxz_reg_h__

#include "base/base.h"

namespace CNORXZ
{
    
    // no use of Arr = std::array here, since I want ensure that
    // it has exactly a memory size of N
    template <typename T, SizeT N>
    struct Consecutive
    {
	T mD[N];
	Consecutive& operator=(const Consecutive& a)
	{
	    iter<0,N>([&](auto i) { mD[i] = a.mD[i]; VCHECK(mD[i]); }, NoF{} );
	    return *this;
	}

	Consecutive& operator=(Consecutive&& a) = delete;
    };

    template <typename T>
    struct is_consecutive_type { CXZ_CVAL_FALSE; };

    template <typename T>
    struct consecutive_base { typedef T type; };

    template <typename T>
    struct consecutive_size { static constexpr SizeT value = 0; };
    
    template <typename T, SizeT N>
    struct is_consecutive_type<Consecutive<T,N>> { CXZ_CVAL_TRUE; };

    template <typename T, SizeT N>
    struct consecutive_base<Consecutive<T,N>> { typedef T type; };

    template <typename T, SizeT N>
    struct consecutive_size<Consecutive<T,N>> { static constexpr SizeT value = N; };

    /****************************************
     *   consecutive generating functions   *
     ****************************************/

    template <typename T, class EPosT, SizeT... Is>
    inline decltype(auto) vregi(const T* d, const EPosT& pos, std::index_sequence<Is...> is);

    template <typename T, class EPosT>
    inline decltype(auto) vreg(const T* d, const EPosT& pos);

    template <typename T, class EPosT>
    inline decltype(auto) vreg(T* d, const EPosT& pos);

    /******************
     *   ConsecFunc   *
     ******************/

    template <SizeT I, typename T>
    constexpr decltype(auto) consecGet(const T& a);

    template <SizeT I, typename T>
    constexpr decltype(auto) consecGet(T& a);

    template <SizeT I, class F, typename... Args>
    constexpr decltype(auto) consecApply(const F& f, const Args&... args);

    template <SizeT I, class F, typename Dst, typename... Args>
    constexpr Dst& consecAssign(const F& f, Dst& dst, const Args&... args);

    template <class F, typename... Args, SizeT... Is>
    constexpr decltype(auto) consecFuncI(const F& f, const Args&... args,
					 std::index_sequence<Is...> is);
    
    template <class F, typename Dst, typename... Args, SizeT... Is>
    constexpr Dst& consecFuncAI(const F& f, Dst& dst, const Args&... args,
				std::index_sequence<Is...> is);

    template <SizeT N, class F, typename... Args>
    constexpr decltype(auto) consecFunc(const F& f, const Args&... args);

    template <SizeT N, class F, typename Dst, typename... Args>
    constexpr Dst& consecFuncA(const F& f, Dst& dst, const Args&... args);

    /******************************
     *   basic operations: plus   *
     ******************************/

    template <typename T, typename U, SizeT N>
    struct PlusCC
    {
	static constexpr decltype(auto)
	eval(const Consecutive<T,N>& a, const Consecutive<U,N>& b);

	static constexpr decltype(auto)
	aeval(Consecutive<T,N>& a, const Consecutive<U,N>& b);
    };

    template <typename T, typename X, SizeT N>
    struct PlusCX
    {
	static constexpr decltype(auto)
	eval(const Consecutive<T,N>& a, const X& b);

	static constexpr decltype(auto)
	aeval(Consecutive<T,N>& a, const X& b);

	static constexpr decltype(auto)
	eval(const X& a, const Consecutive<T,N>& b);
    };
    
    template <typename T, typename U, SizeT N>
    constexpr decltype(auto) operator+(const Consecutive<T,N>& a, const Consecutive<U,N>& b)
    { return PlusCC<T,U,N>::eval(a,b); }

    template <typename T, typename U, SizeT N>
    constexpr decltype(auto) operator+(const Consecutive<T,N>& a, const U& b)
    { return PlusCX<T,U,N>::eval(a,b); }

    template <typename T, typename U, SizeT N>
    constexpr decltype(auto) operator+(const T& a, const Consecutive<U,N>& b)
    { return PlusCX<U,T,N>::eval(a,b); }

    template <typename T, typename U, SizeT N>
    constexpr Consecutive<T,N>& operator+=(Consecutive<T,N>& o, const Consecutive<U,N>& a)
    { return PlusCC<T,U,N>::aeval(o,a); }

    template <typename T, typename U, SizeT N>
    constexpr Consecutive<T,N>& operator+=(Consecutive<T,N>& o, const U& a)
    { return PlusCX<T,U,N>::aeval(o,a); }

    /*******************************
     *   basic operations: minus   *
     *******************************/

    template <typename T, typename U, SizeT N>
    struct MinusCC
    {
	static constexpr decltype(auto)
	eval(const Consecutive<T,N>& a, const Consecutive<U,N>& b);

	static constexpr decltype(auto)
	aeval(Consecutive<T,N>& a, const Consecutive<U,N>& b);
    };

    template <typename T, typename X, SizeT N>
    struct MinusCX
    {
	static constexpr decltype(auto)
	eval(const Consecutive<T,N>& a, const X& b);

	static constexpr decltype(auto)
	aeval(Consecutive<T,N>& a, const X& b);

	static constexpr decltype(auto)
	eval(const X& a, const Consecutive<T,N>& b);
    };
    
    template <typename T, typename U, SizeT N>
    constexpr decltype(auto) operator-(const Consecutive<T,N>& a, const Consecutive<U,N>& b)
    { return MinusCC<T,U,N>::eval(a,b); }

    template <typename T, typename U, SizeT N>
    constexpr decltype(auto) operator-(const Consecutive<T,N>& a, const U& b)
    { return MinusCX<T,U,N>::eval(a,b); }

    template <typename T, typename U, SizeT N>
    constexpr decltype(auto) operator-(const T& a, const Consecutive<U,N>& b)
    { return MinusCX<U,T,N>::eval(a,b); }

    template <typename T, typename U, SizeT N>
    constexpr Consecutive<T,N>& operator-=(Consecutive<T,N>& o, const Consecutive<U,N>& a)
    { return MinusCC<T,U,N>::eval(o,a); }

    template <typename T, typename U, SizeT N>
    constexpr Consecutive<T,N>& operator-=(Consecutive<T,N>& o, const U& a)
    { return MinusCX<T,U,N>::eval(o,a); }

    /***********************************
     *   basic operations: muliplies   *
     ***********************************/

    template <typename T, typename U, SizeT N>
    struct MultipliesCC
    {
	static constexpr decltype(auto)
	eval(const Consecutive<T,N>& a, const Consecutive<U,N>& b);

	static constexpr decltype(auto)
	aeval(Consecutive<T,N>& a, const Consecutive<U,N>& b);
    };

    template <typename T, typename X, SizeT N>
    struct MultipliesCX
    {
	static constexpr decltype(auto)
	eval(const Consecutive<T,N>& a, const X& b);

	static constexpr decltype(auto)
	aeval(Consecutive<T,N>& a, const X& b);

	static constexpr decltype(auto)
	eval(const X& a, const Consecutive<T,N>& b);
    };
    
    template <typename T, typename U, SizeT N>
    constexpr decltype(auto) operator*(const Consecutive<T,N>& a, const Consecutive<U,N>& b)
    { return MultipliesCC<T,U,N>::eval(a,b); }

    template <typename T, typename U, SizeT N>
    constexpr decltype(auto) operator*(const Consecutive<T,N>& a, const U& b)
    { return MultipliesCX<T,U,N>::eval(a,b); }

    template <typename T, typename U, SizeT N>
    constexpr decltype(auto) operator*(const T& a, const Consecutive<U,N>& b)
    { return MultipliesCX<U,T,N>::eval(a,b); }

    template <typename T, typename U, SizeT N>
    constexpr Consecutive<T,N>& operator*=(Consecutive<T,N>& o, const Consecutive<U,N>& a)
    { return MultipliesCC<T,U,N>::eval(o,a); }

    template <typename T, typename U, SizeT N>
    constexpr Consecutive<T,N>& operator*=(Consecutive<T,N>& o, const U& a)
    { return MultipliesCX<T,U,N>::eval(o,a); }

    /*********************************
     *   basic operations: divides   *
     *********************************/

    template <typename T, typename U, SizeT N>
    struct DividesCC
    {
	static constexpr decltype(auto)
	eval(const Consecutive<T,N>& a, const Consecutive<U,N>& b);

	static constexpr decltype(auto)
	aeval(Consecutive<T,N>& a, const Consecutive<U,N>& b);
    };

    template <typename T, typename X, SizeT N>
    struct DividesCX
    {
	static constexpr decltype(auto)
	eval(const Consecutive<T,N>& a, const X& b);

	static constexpr decltype(auto)
	aeval(Consecutive<T,N>& a, const X& b);

	static constexpr decltype(auto)
	eval(const X& a, const Consecutive<T,N>& b);
    };
    
    template <typename T, typename U, SizeT N>
    constexpr decltype(auto) operator/(const Consecutive<T,N>& a, const Consecutive<U,N>& b)
    { return DividesCC<T,U,N>::eval(a,b); }

    template <typename T, typename U, SizeT N>
    constexpr decltype(auto) operator/(const Consecutive<T,N>& a, const U& b)
    { return DividesCX<T,U,N>::eval(a,b); }

    template <typename T, typename U, SizeT N>
    constexpr decltype(auto) operator/(const T& a, const Consecutive<U,N>& b)
    { return DividesCX<U,T,N>::eval(a,b); }

    template <typename T, typename U, SizeT N>
    constexpr Consecutive<T,N>& operator/=(Consecutive<T,N>& o, const Consecutive<U,N>& a)
    { return DividesCC<T,U,N>::eval(o,a); }

    template <typename T, typename U, SizeT N>
    constexpr Consecutive<T,N>& operator/=(Consecutive<T,N>& o, const U& a)
    { return DividesCX<T,U,N>::eval(o,a); }

}

#endif
