
#ifndef __xfor_h__
#define __xfor_h__

#include <cstdlib>
#include <memory>
#include <tuple>
#include "xfor/for_utils.h"

namespace MultiArrayHelper
{

    // 'HIDDEN FOR' CLASS for nested for loops in contractions a.s.o.
    // (NO COUNTING OF MASTER POSITION !!!!!)
    
    template <class IndexClass, class Expr>
    class For
    {
    private:
	For() = default;
	//For(const For& in) = default;
	//For& operator=(const For& in) = default;
		
	const IndexClass* mIndPtr;
	Expr mExpr;
	decltype(mExpr.rootSteps()) mExt;

    public:

	static size_t layer() { return Expr::layer() + 1; }
	static const size_t LAYER = Expr::LAYER + 1;
	static const size_t SIZE = std::remove_reference<Expr>::type::SIZE;

	typedef decltype(mExpr.rootSteps()) ETuple;
	
	For(For&& in) = default;
	For& operator=(For&& in) = default;
	
	For(const std::shared_ptr<IndexClass>& indPtr,
	    Expr&& expr);

	For(const IndexClass* indPtr,
	    Expr&& expr);

	
	inline void operator()(size_t mlast, const ETuple& last) const;
	inline void operator()(size_t mlast = 0) const;

	ETuple rootSteps(std::intptr_t iPtrNum = 0) const;
	    
    };

    template <size_t N>
    size_t exceptMax(size_t max) { return max; }

    template <>
    size_t exceptMax<1>(size_t max) { return 1; }

    
} // namespace MultiArrayHelper

/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */

#include <iostream>

namespace MultiArrayHelper
{

    template <class IndexClass, class Expr>
    For<IndexClass,Expr>::For(const std::shared_ptr<IndexClass>& indPtr,
			      Expr&& expr) :
	mIndPtr(indPtr.get()), mExpr(expr),
	mExt(expr.rootSteps( reinterpret_cast<std::intptr_t>( mIndPtr.get() ))) { assert(mIndPtr != nullptr); }

    template <class IndexClass, class Expr>
    For<IndexClass,Expr>::For(const IndexClass* indPtr,
			      Expr&& expr) :
	mIndPtr(indPtr), mExpr(std::forward<Expr>( expr )),
	mExt(expr.rootSteps( reinterpret_cast<std::intptr_t>( mIndPtr ) )) { assert(mIndPtr != nullptr); }
    
    template <class IndexClass, class Expr>
    inline void For<IndexClass,Expr>::operator()(size_t mlast,
						 const ETuple& last) const
    {
	auto& ind = *mIndPtr;
	std::cout << mIndPtr << std::endl;
	const size_t max = ind.max(); // blocking
	for(size_t pos = ind.pos(); pos != max; ++pos){
	    const size_t mnpos = mlast * max + pos;
	    const ETuple npos = std::move( XFPackNum<SIZE-1>::mkPos(pos, mExt, last) );
	    mExpr(mnpos, npos);
	}
    }

    template <class IndexClass, class Expr>
    inline void For<IndexClass,Expr>::operator()(size_t mlast) const
    {
	const ETuple last;
	auto& ind = *mIndPtr;
	std::cout << mIndPtr << std::endl;
	const size_t max = ind.max(); // blocking
	for(size_t pos = ind.pos(); pos != max; ++pos){
	    const size_t mnpos = mlast * max + pos;
	    const ETuple npos = std::move( XFPackNum<SIZE-1>::mkPos(pos, mExt, last) );
	    mExpr(mnpos, npos);
	}
    }
    
    template <class IndexClass, class Expr>
    typename For<IndexClass,Expr>::ETuple For<IndexClass,Expr>::rootSteps(std::intptr_t iPtrNum) const
    {
	return mExpr.rootSteps(iPtrNum);
    }

    
} // namespace MultiArrayHelper

#endif
