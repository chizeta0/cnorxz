
#ifndef __type_operations_h__
#define __type_operations_h__

#include <cstdlib>
#include <vector>
#include <algorithm>

#include "base_def.h"
#include "mbase_def.h"

#include "pack_num.h"
#include <cmath>

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

    struct v256
    {
        alignas(32) double _x[4];
    };

    template <int N>
    inline void xadd(double* o, const double* a, const double* b)
    {
#pragma omp simd aligned(o, a, b: 32)
	for(int i = 0; i < N; i++) {
	    o[i] = a[i] + b[i];
	}  
    }

    template <int N>
    inline void x1add(double* o, const double* a, const double& b)
    {
#pragma omp simd aligned(o, a: 32)
	for(int i = 0; i < N; i++) {
	    o[i] = a[i] + b;
	}  
    }

    template <int N>
    inline void x2add(double* o, const double& a, const double* b)
    {
#pragma omp simd aligned(o, b: 32)
	for(int i = 0; i < N; i++) {
	    o[i] = a + b[i];
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

    template <int N>
    inline void xradd(double& o, const double* a)
    {
#pragma omp simd reduction(+: o) aligned(a: 32)
	for(int i = 0; i < N; i++) {
	    o += a[i];
	}
    }

    template <int N>
    inline void xsub(double* o, const double* a, const double* b)
    {
#pragma omp simd aligned(o, a, b: 32)
	for(int i = 0; i < N; i++) {
	    o[i] = a[i] - b[i];
	}  
    }

    template <int N>
    inline void xssub(double* o, const double* a)
    {
#pragma omp simd aligned(o, a: 32)
	for(int i = 0; i < N; i++) {
	    o[i] -= a[i];
	}  
    }

    template <int N>
    inline void xrsub(double& o, const double* a)
    {
#pragma omp simd reduction(-: o) aligned(a: 32)
	for(int i = 0; i < N; i++) {
	    o -= a[i];
	}
    }

    template <int N>
    inline void x1sub(double* o, const double* a, const double& b)
    {
#pragma omp simd aligned(o, a: 32)
	for(int i = 0; i < N; i++) {
	    o[i] = a[i] - b;
	}  
    }

    template <int N>
    inline void x2sub(double* o, const double& a, const double* b)
    {
#pragma omp simd aligned(o, b: 32)
	for(int i = 0; i < N; i++) {
	    o[i] = a - b[i];
	}  
    }

    template <int N>
    inline void xmul(double* o, const double* a, const double* b)
    {
#pragma omp simd aligned(o, a, b: 32)
	for(int i = 0; i < N; i++) {
	    o[i] = a[i] * b[i];
	}  
    }

    template <int N>
    inline void xsmul(double* o, const double* a)
    {
#pragma omp simd aligned(o, a: 32)
	for(int i = 0; i < N; i++) {
	    o[i] *= a[i];
	}  
    }

    template <int N>
    inline void xrmul(double& o, const double* a)
    {
#pragma omp simd reduction(*: o) aligned(a: 32)
	for(int i = 0; i < N; i++) {
	    o *= a[i];
	}
    }

    template <int N>
    inline void x1mul(double* o, const double* a, const double& b)
    {
#pragma omp simd aligned(o, a: 32)
	for(int i = 0; i < N; i++) {
	    o[i] = a[i] * b;
	}  
    }

    template <int N>
    inline void x2mul(double* o, const double& a, const double* b)
    {
#pragma omp simd aligned(o, b: 32)
	for(int i = 0; i < N; i++) {
	    o[i] = a * b[i];
	}  
    }

    template <int N>
    inline void xdiv(double* o, const double* a, const double* b)
    {
#pragma omp simd aligned(o, a, b: 32)
	for(int i = 0; i < N; i++) {
	    o[i] = a[i] / b[i];
	}  
    }

    template <int N>
    inline void xsdiv(double* o, const double* a)
    {
#pragma omp simd aligned(o, a: 32)
	for(int i = 0; i < N; i++) {
	    o[i] /= a[i];
	}  
    }
    /*
    template <int N>
    inline void xrdiv(double& o, const double* a)
    {
#pragma omp simd reduction(/: o) aligned(a: 32)
	for(int i = 0; i < N; i++) {
	    o /= a[i];
	}
    }
    */
    template <int N>
    inline void x1div(double* o, const double* a, const double& b)
    {
#pragma omp simd aligned(o, a: 32)
	for(int i = 0; i < N; i++) {
	    o[i] = a[i] / b;
	}  
    }

    template <int N>
    inline void x2div(double* o, const double& a, const double* b)
    {
#pragma omp simd aligned(o, b: 32)
	for(int i = 0; i < N; i++) {
	    o[i] = a / b[i];
	}  
    }

    inline v256 operator+(const v256& a, const v256& b)
    {
	v256 o;
	xadd<4>( o._x, a._x, b._x );
	return o;
    }

    inline v256 operator+(const v256& a, const double& b)
    {
	v256 o;
	x1add<4>( o._x, a._x, b );
	return o;
    }

    inline v256 operator+(const double& a, const v256& b)
    {
	v256 o;
	x2add<4>( o._x, a, b._x );
	return o;
    }

    inline double& operator+=(double& o, const v256& a)
    {
        xradd<4>( o, a._x );
	return o;
    }

    inline v256& operator+=(v256& o, const v256& a)
    {
        xsadd<4>( o._x, a._x );
	return o;
    }

    inline v256 operator-(const v256& a, const v256& b)
    {
	v256 o;
	xsub<4>( o._x, a._x, b._x );
	return o;
    }

    inline v256 operator-(const v256& a, const double& b)
    {
	v256 o;
	x1sub<4>( o._x, a._x, b );
	return o;
    }

    inline v256 operator-(const double& a, const v256& b)
    {
	v256 o;
	x2sub<4>( o._x, a, b._x );
	return o;
    }

    inline double& operator-=(double& o, const v256& a)
    {
        xrsub<4>( o, a._x );
	return o;
    }

    inline v256& operator-=(v256& o, const v256& a)
    {
        xssub<4>( o._x, a._x );
	return o;
    }

    inline v256 operator*(const v256& a, const v256& b)
    {
	v256 o;
	xmul<4>( o._x, a._x, b._x );
	return o;
    }

    inline v256& operator*=(v256& o, const v256& a)
    {
        xsmul<4>( o._x, a._x );
	return o;
    }

    inline v256 operator*(const v256& a, const double& b)
    {
	v256 o;
	x1mul<4>( o._x, a._x, b );
	return o;
    }

    inline v256 operator*(const double& a, const v256& b)
    {
	v256 o;
	x2mul<4>( o._x, a, b._x );
	return o;
    }

    inline double& operator*=(double& o, const v256& a)
    {
        xrmul<4>( o, a._x );
	return o;
    }

    inline v256 operator/(const v256& a, const v256& b)
    {
	v256 o;
	xdiv<4>( o._x, a._x, b._x );
	return o;
    }

    inline v256 operator/(const v256& a, const double& b)
    {
	v256 o;
	x1div<4>( o._x, a._x, b );
	return o;
    }

    inline v256 operator/(const double& a, const v256& b)
    {
	v256 o;
	x2div<4>( o._x, a, b._x );
	return o;
    }
    /*
    inline double& operator/=(double& o, const v256& a)
    {
        xrdiv<4>( o, a._x );
	return o;
    }
    */
    inline v256& operator/=(v256& o, const v256& a)
    {
        xsdiv<4>( o._x, a._x );
	return o;
    }

    inline double xpow(const double& b, const double& e)
    {
	return pow(b,e);
    }

    inline v256 pow(const v256& b, const v256& e)
    {
	v256 out;
	for(int i = 0; i < 4; i++){
	    out._x[i] = xpow(b._x[i],e._x[i]);
	}
	return out;
    }

    inline double xexp(const double& a)
    {
	return exp(a);
    }

    inline v256 exp(const v256& a)
    {
	v256 out;
	for(int i = 0; i < 4; i++){
	    out._x[i] = xexp(a._x[i]);
	}
	return out;
    }

    inline double xsqrt(const double& a)
    {
	return sqrt(a);
    }

    inline v256 sqrt(const v256& a)
    {
	v256 out;
	for(int i = 0; i < 4; i++){
	    out._x[i] = xsqrt(a._x[i]);
	}
	return out;
    }

} // namespace MultiArrayTools

#endif
