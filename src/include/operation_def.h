
#ifndef __operation_def_h__
#define __operation_def_h__

#include "cxz_operation.h"

namespace MultiArrayTools
{

    template <typename T, class OperationClass>
    auto operator+(const T& a, const OperationBase<T,OperationClass>& b)
	-> Operation<T,plus<T>,OperationValue<T>,OperationClass>
    {
	OperationValue<T> v(a);
	return Operation<T,plus<T>,OperationValue<T>,OperationClass>(v, b.THIS());
    }

    template <typename T, class OperationClass>
    auto operator-(const T& a, const OperationBase<T,OperationClass>& b)
	-> Operation<T,minus<T>,OperationValue<T>,OperationClass>
    {
	OperationValue<T> v(a);
	return Operation<T,minus<T>,OperationValue<T>,OperationClass>(v, b.THIS());
    }

    template <typename T, class OperationClass>
    auto operator*(const T& a, const OperationBase<T,OperationClass>& b)
	-> Operation<T,multiplies<T>,OperationValue<T>,OperationClass>
    {
	OperationValue<T> v(a);
	return Operation<T,multiplies<T>,OperationValue<T>,OperationClass>(v, b.THIS());
    }

    template <typename T, class OperationClass>
    auto operator/(const T& a, const OperationBase<T,OperationClass>& b)
	-> Operation<T,divides<T>,OperationValue<T>,OperationClass>
    {
	OperationValue<T> v(a);
	return Operation<T,divides<T>,OperationValue<T>,OperationClass>(v, b.THIS());
    }

    template <typename T, class OperationClass>
    auto operator+(const OperationBase<T,OperationClass>& a, const T& b)
	-> Operation<T,plus<T>,OperationClass,OperationValue<T> >
    {
	OperationValue<T> v(b);
	return Operation<T,plus<T>,OperationClass,OperationValue<T> >(a.THIS(), v);
    }

    template <typename T, class OperationClass>
    auto operator-(const OperationBase<T,OperationClass>& a, const T& b)
	-> Operation<T,minus<T>,OperationClass,OperationValue<T> >
    {
	OperationValue<T> v(b);
	return Operation<T,minus<T>,OperationClass,OperationValue<T> >(a.THIS(), v);
    }

    template <typename T, class OperationClass>
    auto operator*(const OperationBase<T,OperationClass>& a, const T& b)
	-> Operation<T,multiplies<T>,OperationClass,OperationValue<T> >
    {
	OperationValue<T> v(b);
	return Operation<T,multiplies<T>,OperationClass,OperationValue<T> >(a.THIS(), v);
    }

    template <typename T, class OperationClass>
    auto operator/(const OperationBase<T,OperationClass>& a, const T& b)
	-> Operation<T,divides<T>,OperationClass,OperationValue<T> >
    {
	OperationValue<T> v(b);
	return Operation<T,divides<T>,OperationClass,OperationValue<T> >(a.THIS(), v);
    }
    
#define regFunc1(fff) template <typename T, class OperationClass>	\
    auto fff(const OperationBase<T,OperationClass>& a)	\
	-> Operation<T,x_##fff<T>,OperationClass> {			\
	return Operation<T,x_##fff<T>,OperationClass>(a.THIS()); }

    
#include "extensions/math.h"
#undef regFunc1


    template <size_t N, typename T, class OperationClass>
    auto ipow(const OperationBase<T,OperationClass>& a)
	-> Operation<T,x_ipow<N-1>,OperationClass>
    {
	return Operation<T,x_ipow<N-1>,OperationClass>(a.THIS());
    }
    

}

#endif
