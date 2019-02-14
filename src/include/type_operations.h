
#ifndef __type_operations_h__
#define __type_operations_h__

#include <cstdlib>
#include <vector>
#include <algorithm>

#include "base_def.h"
#include "mbase_def.h"

#include "pack_num.h"

namespace MultiArrayTools
{
    namespace
    {
	using namespace MultiArrayHelper;
    }

    // MultiArray
    
    template <typename T, class... Ranges>
    class operate
    {
    public:
	static constexpr bool FISSTATIC = false;

	operate(const std::shared_ptr<typename Ranges::IndexType>&... inds) : ituple(inds...) {}
	
	inline auto apply(const MultiArray<T,Ranges...>& ma)
	    -> OperationRoot<T,Ranges...>
	{
	    return PackNum<sizeof...(Ranges)-1>::mkElemOperation(ma, ituple);
	}

    private:

	// this is, why non-static
	std::tuple<std::shared_ptr<typename Ranges::IndexType>...> ituple;
    };

    template <class OperationClass, typename T, class... Ranges>
    class OperationTemplate<MultiArray<T,Ranges...>,OperationClass> : public OperationBase<MultiArray<T,Ranges...>,OperationClass>
    {
	typedef OperationBase<MultiArray<T,Ranges...>,OperationClass> OB;
	
	auto operator()(const std::shared_ptr<typename Ranges::IndexType>&... indices)
	    -> Operation<OperationRoot<T,Ranges...>,operate<T,Ranges...>,OperationClass>
	{
	    std::shared_ptr<operate<T,Ranges...> > ff(indices...);
	    return Operation<OperationRoot<T,Ranges...>,operate<T,Ranges...>,OperationClass>(ff, OB::THIS());
	}

    private:
	OperationTemplate() = default;
	friend OperationClass;

    };
    
    // vector

    template <typename T>
    class getter
    {
    private:
	size_t mPos;
    public:
	static constexpr bool FISSTATIC = false;

	getter(size_t i) : mPos(i) {}

	inline T operator()(const vector<T>& in)
	{
	    return in[mPos];
	}
    };

    template <typename T>
    vector<T>& operator+=(vector<T>& a, const vector<T>& b)
    {
	std::transform(a.begin(), a.end(), b.begin(), a.begin(), std::plus<T>());
	return a;
    }

    template <typename T>
    vector<T>& operator-=(vector<T>& a, const vector<T>& b)
    {
	std::transform(a.begin(), a.end(), b.begin(), a.begin(), std::minus<T>());
	return a;
    }

    template <typename T>
    vector<T>& operator*=(vector<T>& a, const vector<T>& b)
    {
	std::transform(a.begin(), a.end(), b.begin(), a.begin(), std::multiplies<T>());
	return a;
    }

    template <typename T>
    vector<T>& operator/=(vector<T>& a, const vector<T>& b)
    {
	std::transform(a.begin(), a.end(), b.begin(), a.begin(), std::divides<T>());
	return a;
    }

    template <typename T>
    vector<T> operator+(vector<T>& a, const vector<T>& b)
    {
	vector<T> out(a.size());
	std::transform(a.begin(), a.end(), b.begin(), out.begin(), std::plus<T>());
	return out;
    }

    template <typename T>
    vector<T> operator-(vector<T>& a, const vector<T>& b)
    {
	vector<T> out(a.size());
	std::transform(a.begin(), a.end(), b.begin(), out.begin(), std::minus<T>());
	return out;
    }

    template <typename T>
    vector<T> operator*(vector<T>& a, const vector<T>& b)
    {
	vector<T> out(a.size());
	std::transform(a.begin(), a.end(), b.begin(), out.begin(), std::multiplies<T>());
	return out;
    }

    template <typename T>
    vector<T> operator/(vector<T>& a, const vector<T>& b)
    {
	vector<T> out(a.size());
	std::transform(a.begin(), a.end(), b.begin(), out.begin(), std::divides<T>());
	return out;
    }

    template <class OperationClass, typename T>
    class OperationTemplate<vector<T>,OperationClass> : public OperationBase<vector<T>,OperationClass>
    {
    public:
	typedef OperationBase<vector<T>,OperationClass> OB;

	auto operator[](size_t i)
	    -> Operation<T,getter<T>,OperationClass>
	{
	    std::shared_ptr<getter<T> > ff = std::make_shared<getter<T> >(i);
	    return Operation<T,getter<T>,OperationClass>(ff,OB::THIS());
	}
	
    private:
	OperationTemplate() = default;
	friend OperationClass;
    };

    typedef struct v256 { alignas(32) double _x[4]; } v256;

    template <int N>
    inline void xadd(double* o, const double* a, const double* b)
    {
#pragma omp simd aligned(o, a, b: 32)
	for(int i = 0; i < N; i++) {
	    o[i] = a[i] + b[i];
	}  
    }

    template <int N>
    inline void xsadd(double* o, const double* a)
    {
#pragma omp simd aligned(o, a: 32)
	for(int i = 0; i < N; i++) {
	    o[i] += a[i];
	}  
    }
    
    inline v256 operator+(const v256& a, const v256& b)
    {
	alignas(32) v256 o;
	xadd<4>( reinterpret_cast<double*>(&o), reinterpret_cast<const double*>(&a),
		 reinterpret_cast<const double*>(&b) );
	return o;
    }

    inline v256& operator+=(v256& o, const v256& a)
    {
	xsadd<4>( reinterpret_cast<double*>(&o), reinterpret_cast<const double*>(&a) );
	return o;
    }
/*
    inline v256 operator-(const v256& a, const v256& b)
    {
        	alignas(32) v256 out;
#pragma omp simd aligned(outp, ap, bp: 32)
	for(int i = 0; i < IN; ++i){
	    outp[i] = ap[i] - bp[i];
	}
	return out;
    }

    inline v256 operator*(const v256& a, const v256& b)
    {
	alignas(32) v256 out;
#pragma omp simd aligned(outp, ap, bp: 32)
	for(int i = 0; i < IN; ++i){
	    outp[i] = ap[i] * bp[i];
	}
	return out;
    }
    
    inline v256 operator/(const v256& a, const v256& b)
    {
	alignas(32) v256 out;
#pragma omp simd aligned(outp, ap, bp: 32)
	for(int i = 0; i < IN; ++i){
	    outp[i] = ap[i] / bp[i];
	}
	return out;
    }
    */
    

} // namespace MultiArrayTools

#endif
