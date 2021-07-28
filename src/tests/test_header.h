

#include <cstdlib>
#include "gtest/gtest.h"
#include <iostream>

#include "cnorxz.h"

#include <ctime>
#include <cmath>

namespace MAT = CNORXZ;

double xround(double arg)
{
    if(std::isnan(arg)) { return 0.; }
    return roundf(arg * 100000.) / 100000.;
}
    
using namespace MAT;

typedef ClassicRange CR;
typedef ClassicRF CRF;
typedef DynamicRange DR;
typedef DynamicRangeFactory DRF;

template <class Factory>
void swapFactory(std::shared_ptr<RangeFactoryBase>& fptr)
{
    auto nptr = std::make_shared<Factory>();
    fptr = nptr;
}
    
template <class Factory, typename T>
void swapFactory(std::shared_ptr<RangeFactoryBase>& fptr, std::initializer_list<T> ilist)
{
    vector<T> tmp = ilist;
    auto nptr = std::make_shared<Factory>( tmp );
    fptr = nptr;
}

template <class Factory, typename T>
void swapFactory(std::shared_ptr<RangeFactoryBase>& fptr, vector<T>& ilist)
{
    vector<T> tmp = ilist;
    auto nptr = std::make_shared<Factory>( tmp );
    fptr = nptr;
}

    
template <class Factory, class... Rs>
void swapMFactory(std::shared_ptr<RangeFactoryBase>& fptr, const Rs&... rs)
{
    auto nptr = std::make_shared<Factory>( rs... );
    fptr = nptr;
}

template <typename... Ts>
auto mkt(Ts&&... ts) -> decltype(std::make_tuple(ts...))
{
    return std::make_tuple(ts...);
}

template <typename... Ts>
auto mkts(Ts&&... ts) -> decltype(std::make_tuple(ts...))
{
    return std::make_tuple(static_cast<size_t>( ts )...);
}

//typedef Expressions1 EC1;
    
template <typename T>
struct Pow
{
    static constexpr bool FISSTATIC = true;
    typedef T value_type;
	
    static inline T apply(T b, T e)
    {
	return pow(b, e);
    }

    template <class... Ops>
    static auto mk(const Ops&... ops)
	-> Operation<T,Pow<T>,Ops...>
    {
	return Operation<T,Pow<T>,Ops...>(ops...);
    }

    static inline T apply(const std::tuple<double, double>& arg)
    {
	return pow(std::get<0>(arg), std::get<1>(arg));
    }
};


template <typename T>
struct Monopole
{
    static constexpr bool FISSTATIC = true;

    static inline T apply(T f0, T x, T n)
    {
	return f0 / ( 1 + x / n );
    }

    template <class... Ops>
    static auto mk(const Ops&... ops)
	-> Operation<T,Monopole<T>,Ops...>
    {
	return Operation<T,Monopole<T>,Ops...>(ops...);
    }
};

