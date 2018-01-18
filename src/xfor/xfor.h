
#ifndef __xfor_h__
#define __xfor_h__

#include <cstdlib>
#include <memory>
#include <tuple>
#include "xfor/for_utils.h"
#include "xfor/for_type.h"
#include "xfor/for_params.h"

namespace MultiArrayHelper
{

    // 'HIDDEN FOR' CLASS for nested for loops in contractions a.s.o.
    // (NO COUNTING OF MASTER POSITION !!!!!)

    template <ForType FT = ForType::DEFAULT>
    struct PosForward
    {
	static inline size_t value(size_t last, size_t max, size_t pos)
	{
	    return last * max + pos;
	}
    };

    template <>
    struct PosForward<ForType::HIDDEN>
    {
	static inline size_t value(size_t last, size_t max, size_t pos)
	{
	    return last;
	}
    };
    
    template <class IndexClass, class Expr, ForType FT = ForType::DEFAULT>
    class For
    {
    private:
	For() = default;
	//For(const For& in) = default;
	//For& operator=(const For& in) = default;
		
	const IndexClass* mIndPtr;
	size_t mSPos;
	size_t mMax;
	Expr mExpr;

	typedef decltype(mExpr.rootSteps()) FParType;
	FParType mExt;

    public:

	static size_t layer() { return Expr::layer() + 1; }
	static const size_t LAYER = Expr::LAYER + 1;
	static const size_t SIZE = std::remove_reference<Expr>::type::SIZE;
	
	For(For&& in) = default;
	For& operator=(For&& in) = default;
	
	For(const std::shared_ptr<IndexClass>& indPtr,
	    Expr&& expr);

	For(const IndexClass* indPtr,
	    Expr&& expr);

	
	inline void operator()(size_t mlast, const FParType& last) const;
	inline void operator()(size_t mlast = 0) const;

	FParType rootSteps(std::intptr_t iPtrNum = 0) const;
	    
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

    template <class IndexClass, class Expr, ForType FT>
    For<IndexClass,Expr,FT>::For(const std::shared_ptr<IndexClass>& indPtr,
			      Expr&& expr) :
	mIndPtr(indPtr.get()), mSPos(mIndPtr->pos()), mMax(mIndPtr->max()), mExpr(expr),
	mExt(expr.rootSteps( reinterpret_cast<std::intptr_t>( mIndPtr.get() )))
    {
	assert(mIndPtr != nullptr);
	//VCHECK(mIndPtr->id());
	//VCHECK(mIndPtr->max());
    }

    template <class IndexClass, class Expr, ForType FT>
    For<IndexClass,Expr,FT>::For(const IndexClass* indPtr,
			      Expr&& expr) :
	mIndPtr(indPtr), mSPos(mIndPtr->pos()), mMax(mIndPtr->max()),
	mExpr(std::forward<Expr>( expr )),
	mExt(expr.rootSteps( reinterpret_cast<std::intptr_t>( mIndPtr ) ))
    {
	assert(mIndPtr != nullptr);
	//VCHECK(mIndPtr->id());
	//VCHECK(mIndPtr->max());
    }
    
    template <class IndexClass, class Expr, ForType FT>
    inline void For<IndexClass,Expr,FT>::operator()(size_t mlast,
						    const FParType& last) const
    {
	for(size_t pos = mSPos; pos != mMax; ++pos){
	    //const size_t mnpos = mlast * max + pos;
	    const size_t mnpos = PosForward<FT>::value(mlast, mMax, pos);
	    //const FParType npos = std::move( XFPackNum<SIZE-1>::mkPos(pos, mExt, last) );
	    const FParType npos(mlast, mExt, pos);
	    mExpr(mnpos, npos);
	}
    }

    template <class IndexClass, class Expr, ForType FT>
    inline void For<IndexClass,Expr,FT>::operator()(size_t mlast) const
    {
	const FParType last;
	for(size_t pos = mSPos; pos != mMax; ++pos){
	    //const size_t mnpos = mlast * max + pos;
	    const size_t mnpos = PosForward<FT>::value(mlast, mMax, pos);
	    //const FParType npos = std::move( XFPackNum<SIZE-1>::mkPos(pos, mExt, last) );
	    const FParType npos(mlast, mExt, pos);
	    mExpr(mnpos, npos);
	}
    }
    
    template <class IndexClass, class Expr, ForType FT>
    typename For<IndexClass,Expr,FT>::FParType For<IndexClass,Expr,FT>::rootSteps(std::intptr_t iPtrNum) const
    {
	return mExpr.rootSteps(iPtrNum);
    }

    
} // namespace MultiArrayHelper

#endif
