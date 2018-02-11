
#ifndef __xfor_h__
#define __xfor_h__

#include <cstdlib>
#include <memory>
#include <tuple>
#include "xfor/for_utils.h"
#include "xfor/for_type.h"
#include "xfor/exttype.h"

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

	typedef decltype(mExpr.rootSteps()) ExtType;
	ExtType mExt;

    public:

	static constexpr size_t LAYER = Expr::LAYER + 1;
	static constexpr size_t SIZE = Expr::SIZE;

	For(const For& in) = default;
	For& operator=(const For& in) = default;
	For(For&& in) = default;
	For& operator=(For&& in) = default;
	
	For(const std::shared_ptr<IndexClass>& indPtr,
	    Expr expr);

	For(const IndexClass* indPtr,
	    Expr expr);

	
	inline void operator()(size_t mlast, ExtType last) const;
	inline void operator()(size_t mlast = 0) const;

	auto rootSteps(std::intptr_t iPtrNum = 0) const -> ExtType;
	    
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
				 Expr expr) :
	mIndPtr(indPtr.get()), mSPos(mIndPtr->pos()), mMax(mIndPtr->max()), mExpr(expr),
	mExt(expr.rootSteps( reinterpret_cast<std::intptr_t>( mIndPtr.get() )))
    {
	assert(mIndPtr != nullptr);
	//VCHECK(mIndPtr->id());
	//VCHECK(mIndPtr->max());
    }

    template <class IndexClass, class Expr, ForType FT>
    For<IndexClass,Expr,FT>::For(const IndexClass* indPtr,
				 Expr expr) :
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
						    const ExtType last) const
    {
	for(size_t pos = mSPos; pos != mMax; ++pos){
	    const size_t mnpos = PosForward<FT>::value(mlast, mMax, pos);
	    const ExtType npos = last + mExt*pos;
	    mExpr(mnpos, npos);
	}
    }

    template <class IndexClass, class Expr, ForType FT>
    inline void For<IndexClass,Expr,FT>::operator()(size_t mlast) const
    {
	const ExtType last;
	for(size_t pos = mSPos; pos != mMax; ++pos){
	    const size_t mnpos = PosForward<FT>::value(mlast, mMax, pos);
	    const ExtType npos = last + mExt*pos;
	    mExpr(mnpos, npos);
	}
    }
    
    template <class IndexClass, class Expr, ForType FT>
    auto For<IndexClass,Expr,FT>::rootSteps(std::intptr_t iPtrNum) const
	-> ExtType
    {
	return mExpr.rootSteps(iPtrNum);
    }

    
} // namespace MultiArrayHelper

#endif
