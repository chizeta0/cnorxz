
#ifndef __xfor_h__
#define __xfor_h__

#include <cstdlib>
#include <memory>
#include <tuple>
#include "xfor/for_type.h"
#include "xfor/for_utils.h"
#include "xfor/exttype.h"

namespace MultiArrayHelper
{

    // 'HIDDEN FOR' CLASS for nested for loops in contractions a.s.o.
    // (NO COUNTING OF MASTER POSITION !!!!!)

    template <class Expr>
    class ExpressionBase
    {
    public:
	static constexpr size_t LAYER = Expr::LAYER + 1;
	static constexpr size_t SIZE = Expr::SIZE;

    protected:
	Expr mExpr;

    public:
	typedef decltype(mExpr.rootSteps()) ExtType;

    protected:
	ExtType mExt;

    public:

	ExpressionBase() = default;
	ExpressionBase(const ExpressionBase& in) = default;
	ExpressionBase(ExpressionBase&& in) = default;
	ExpressionBase& operator=(const ExpressionBase& in) = default;
	ExpressionBase& operator=(ExpressionBase&& in) = default;

	ExpressionBase(ExtType ext, Expr&& expr);
	
	virtual void operator()(size_t mlast, ExtType last) const = 0;
	virtual void operator()(size_t mlast = 0) const = 0;

	auto rootSteps(std::intptr_t iPtrNum = 0) const -> ExtType;
	auto extension() const -> ExtType;
    };

    
    template <ForType FT = ForType::DEFAULT>
    struct PosForward
    {
	static inline size_t valuex(size_t last, size_t step, size_t pos)
	{
	    return last + pos * step;
	}

	static inline size_t value(size_t last, size_t max, size_t pos)
	{
	    return last * max + pos;
	}
    };

    template <>
    struct PosForward<ForType::HIDDEN>
    {
	static inline size_t valuex(size_t last, size_t step, size_t pos)
	{
	    return last;
	}

	static inline size_t value(size_t last, size_t max, size_t pos)
	{
	    return last;
	}
    };

    template <size_t ISSTATIC>
    struct ForBound
    {
	template <size_t BOUND>
	static inline size_t bound(size_t bound)
	{
	    return bound;
	}
    };

    template <>
    struct ForBound<1>
    {
	template <size_t BOUND>
	static constexpr size_t bound(size_t bound)
	{
	    return BOUND;
	}
    };

    template <class IndexClass, class Expr>
    class SingleExpression : public ExpressionBase<Expr>
    {
    private:
	SingleExpression() = default;

	const IndexClass* mIndPtr;
	size_t mSPos;
	size_t mMax;

    public:
	typedef ExpressionBase<Expr> EB;
	using EB::rootSteps;
	typedef typename ExpressionBase<Expr>::ExtType ExtType;
	
	static constexpr size_t LAYER = Expr::LAYER + 1;
	static constexpr size_t SIZE = Expr::SIZE;

	SingleExpression(const SingleExpression& in) = default;
	SingleExpression& operator=(const SingleExpression& in) = default;
	SingleExpression(SingleExpression&& in) = default;
	SingleExpression& operator=(SingleExpression&& in) = default;
	
	SingleExpression(const std::shared_ptr<IndexClass>& indPtr,
			 Expr expr);

	SingleExpression(const IndexClass* indPtr,
			 Expr expr);

	
	inline void operator()(size_t mlast, ExtType last) const override final;
	inline void operator()(size_t mlast = 0) const override final;

    };
    
    template <class IndexClass, class Expr, ForType FT>
    class For : public ExpressionBase<Expr>
    {
    private:
	For() = default;
		
	const IndexClass* mIndPtr;
	size_t mSPos;
	size_t mMax;
	size_t mStep;

    public:
	typedef ExpressionBase<Expr> EB;
	using EB::rootSteps;
	typedef typename ExpressionBase<Expr>::ExtType ExtType;
	
	static constexpr size_t LAYER = Expr::LAYER + 1;
	static constexpr size_t SIZE = Expr::SIZE;

	For(const For& in) = default;
	For& operator=(const For& in) = default;
	For(For&& in) = default;
	For& operator=(For&& in) = default;
	
	For(const std::shared_ptr<IndexClass>& indPtr,
	    size_t step, Expr expr);

	For(const IndexClass* indPtr,
	    size_t step, Expr expr);

	inline void operator()(size_t mlast, ExtType last) const override final;
	inline void operator()(size_t mlast = 0) const override final;
	    
    };

    template <size_t N>
    inline size_t exceptMax(size_t max) { return max; }

    template <>
    inline size_t exceptMax<1>(size_t max) { return 1; }

    template <class Expr>
    class DynamicalExpression : public ExpressionBase<Expr>
    {
    private:
	DynamicalExpression() = default;

	std::shared_ptr<ExpressionBase<Expr>> mNext;

    public:
	typedef ExpressionBase<Expr> EB;
	using EB::rootSteps;
	typedef typename ExpressionBase<Expr>::ExtType ExtType;
	
	static constexpr size_t LAYER = Expr::LAYER + 1;
	static constexpr size_t SIZE = Expr::SIZE;

	DynamicalExpression(const DynamicalExpression& in) = default;
	DynamicalExpression(DynamicalExpression&& in) = default;
	DynamicalExpression& operator=(const DynamicalExpression& in) = default;
	DynamicalExpression& operator=(DynamicalExpression&& in) = default;
	
	DynamicalExpression(const std::shared_ptr<ExpressionBase<Expr>>& next,
			    Expr expr);

	inline void operator()(size_t mlast, ExtType last) const override final;
	inline void operator()(size_t mlast = 0) const override final;
	
    };
    
} // namespace MultiArrayHelper

/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */

#include <iostream>

namespace MultiArrayHelper
{

    /**********************
     *   ExpressionBase   *
     **********************/
    
    template <class Expr>
    ExpressionBase<Expr>::ExpressionBase(ExtType ext, Expr&& expr) :
	mExpr(std::forward<Expr>(expr)),
	mExt(ext)
    {}
    
    template <class Expr>
    auto ExpressionBase<Expr>::rootSteps(std::intptr_t iPtrNum) const
	-> ExtType
    {
	return mExpr.rootSteps(iPtrNum);
    }

    template <class Expr>
    auto ExpressionBase<Expr>::extension() const
	-> ExtType
    {
	return mExt;
    }
    
    /*****************
     *     F o r     *
     *****************/
    
    template <class IndexClass, class Expr, ForType FT>
    For<IndexClass,Expr,FT>::For(const std::shared_ptr<IndexClass>& indPtr,
				 size_t step, Expr expr) :
	ExpressionBase<Expr>(expr.rootSteps( reinterpret_cast<std::intptr_t>( indPtr.get() )),
			     std::forward<Expr>(expr)),
	mIndPtr(indPtr.get()), mSPos(mIndPtr->pos()), mMax(mIndPtr->max()), mStep(step)
    {
	assert(mIndPtr != nullptr);
	//VCHECK(mIndPtr->id());
	//VCHECK(mIndPtr->max());
    }

    template <class IndexClass, class Expr, ForType FT>
    For<IndexClass,Expr,FT>::For(const IndexClass* indPtr,
				 size_t step, Expr expr) :
	ExpressionBase<Expr>(expr.rootSteps( reinterpret_cast<std::intptr_t>( indPtr ) ),
			     std::forward<Expr>(expr)),
	mIndPtr(indPtr), mSPos(mIndPtr->pos()), mMax(mIndPtr->max()),
	mStep(step)
    {
	assert(mIndPtr != nullptr);
	//VCHECK(mIndPtr->id());
	//VCHECK(mIndPtr->max());
    }
    
    template <class IndexClass, class Expr, ForType FT>
    inline void For<IndexClass,Expr,FT>::operator()(size_t mlast,
						    ExtType last) const
    {
	typedef typename IndexClass::RangeType RangeType;
	for(size_t pos = 0u; pos != ForBound<RangeType::ISSTATIC>::template bound<RangeType::SIZE>(mMax); ++pos){
	//for(size_t pos = mSPos; pos != mMax; ++pos){
	    //const size_t mnpos = PosForward<FT>::value(mlast, mMax, pos);
	    const size_t mnpos = PosForward<FT>::valuex(mlast, mStep, pos);
	    const ExtType npos = last + EB::mExt*pos;
	    EB::mExpr(mnpos, npos);
	}
    }

    template <class IndexClass, class Expr, ForType FT>
    inline void For<IndexClass,Expr,FT>::operator()(size_t mlast) const
    {
	typedef typename IndexClass::RangeType RangeType;
	const ExtType last;
	for(size_t pos = 0u; pos != ForBound<RangeType::ISSTATIC>::template bound<RangeType::SIZE>(mMax); ++pos){
	//for(size_t pos = mSPos; pos != mMax; ++pos){
	    //const size_t mnpos = PosForward<FT>::value(mlast, mMax, pos);
	    const size_t mnpos = PosForward<FT>::valuex(mlast, mStep, pos);
	    const ExtType npos = last + EB::mExt*pos;
	    EB::mExpr(mnpos, npos);
	}
    }
    

    /************************
     *   SingleExpression   *
     ************************/
    
    template <class IndexClass, class Expr>
    SingleExpression<IndexClass,Expr>::SingleExpression(const std::shared_ptr<IndexClass>& indPtr,
							Expr expr) :
	ExpressionBase<Expr>(expr.rootSteps( reinterpret_cast<std::intptr_t>( indPtr.get() )),
			     std::forward<Expr>(expr)),
	mIndPtr(indPtr.get()), mSPos(mIndPtr->pos()), mMax(mIndPtr->max())
    {
	assert(mIndPtr != nullptr);
	//VCHECK(mIndPtr->id());
	//VCHECK(mIndPtr->max());
    }

    template <class IndexClass, class Expr>
    SingleExpression<IndexClass,Expr>::SingleExpression(const IndexClass* indPtr,
				 Expr expr) :
	ExpressionBase<Expr>(expr.rootSteps( reinterpret_cast<std::intptr_t>( indPtr ) ),
			     std::forward<Expr>(expr)),
	mIndPtr(indPtr), mSPos(mIndPtr->pos()), mMax(mIndPtr->max())
    {
	assert(mIndPtr != nullptr);
	//VCHECK(mIndPtr->id());
	//VCHECK(mIndPtr->max());
    }
    
    template <class IndexClass, class Expr>
    inline void SingleExpression<IndexClass,Expr>::operator()(size_t mlast,
							      ExtType last) const
    {
	//typedef typename IndexClass::RangeType RangeType;
	const size_t pos = mIndPtr->pos();
	const size_t mnpos = PosForward<ForType::DEFAULT>::value(mlast, mMax, pos);
	const ExtType npos = last + EB::mExt*pos;
	EB::mExpr(mnpos, npos);
    }

    template <class IndexClass, class Expr>
    inline void SingleExpression<IndexClass,Expr>::operator()(size_t mlast) const
    {
	//typedef typename IndexClass::RangeType RangeType;
	const ExtType last;
	const size_t pos = mIndPtr->pos();
	const size_t mnpos = PosForward<ForType::DEFAULT>::value(mlast, mMax, pos);
	const ExtType npos = last + EB::mExt*pos;
	EB::mExpr(mlast, last);
    }

    
    /***************************
     *   DynamicalExpression   *
     ***************************/

    template <class Expr>
    DynamicalExpression<Expr>::
    DynamicalExpression(const std::shared_ptr<ExpressionBase<Expr>>& next,
			Expr expr) :
	ExpressionBase<Expr>(next->extension(), std::forward<Expr>(expr)),
	mNext(next)
    {}

    template <class Expr>
    inline void DynamicalExpression<Expr>::operator()(size_t mlast, ExtType last) const
    {
	(*mNext)(mlast,last);
    }

    template <class Expr>
    inline void DynamicalExpression<Expr>::operator()(size_t mlast) const
    {
	(*mNext)(mlast);
    }

    
} // namespace MultiArrayHelper

#endif
