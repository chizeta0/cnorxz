
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

    typedef std::pair<size_t const*,size_t> DExt;
    
    class ExpressionBase
    {
    public:

	ExpressionBase() = default;
	ExpressionBase(const ExpressionBase& in) = default;
	ExpressionBase(ExpressionBase&& in) = default;
	ExpressionBase& operator=(const ExpressionBase& in) = default;
	ExpressionBase& operator=(ExpressionBase&& in) = default;

	virtual void operator()(size_t mlast, DExt last) const = 0;
	virtual void operator()(size_t mlast = 0) const = 0;

        virtual DExt dRootSteps(std::intptr_t iPtrNum = 0) const = 0;
        virtual DExt dExtension() const = 0;
        
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
    class SingleExpression : public ExpressionBase
    {
    private:
	SingleExpression() = default;

	const IndexClass* mIndPtr;
	size_t mSPos;
	size_t mMax;

        Expr mExpr;
	typedef decltype(mExpr.rootSteps()) ExtType;
	ExtType mExt;

        mutable ExtType mRootSteps;

    public:
	typedef ExpressionBase EB;
	
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

	
	inline void operator()(size_t mlast, DExt last) const override final;
	inline void operator()(size_t mlast, ExtType last) const;
	inline void operator()(size_t mlast = 0) const override final;

        DExt dRootSteps(std::intptr_t iPtrNum = 0) const override final;
        DExt dExtension() const override final;

        auto rootSteps(std::intptr_t iPtrNum = 0) const -> ExtType;
	auto extension() const -> ExtType;
    };
    
    template <class IndexClass, class Expr, ForType FT>
    class For : public ExpressionBase
    {
    private:
	For() = default;
		
	const IndexClass* mIndPtr;
	size_t mSPos;
	size_t mMax;
	size_t mStep;

	Expr mExpr;
	typedef decltype(mExpr.rootSteps()) ExtType;
	ExtType mExt;

        mutable ExtType mRootSteps;
        
    public:
	typedef ExpressionBase EB;
	
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

	inline void operator()(size_t mlast, DExt last) const override final;
	inline void operator()(size_t mlast, ExtType last) const;
	inline void operator()(size_t mlast = 0) const override final;

        DExt dRootSteps(std::intptr_t iPtrNum = 0) const override final;
        DExt dExtension() const override final;

        auto rootSteps(std::intptr_t iPtrNum = 0) const -> ExtType;
	auto extension() const -> ExtType;

    };

    template <size_t N>
    inline size_t exceptMax(size_t max) { return max; }

    template <>
    inline size_t exceptMax<1>(size_t max) { return 1; }

    class DynamicExpression : public ExpressionBase
    {
    private:
	DynamicExpression() = default;

	std::shared_ptr<ExpressionBase> mNext;

    public:
	
	DynamicExpression(const DynamicExpression& in) = default;
	DynamicExpression(DynamicExpression&& in) = default;
	DynamicExpression& operator=(const DynamicExpression& in) = default;
	DynamicExpression& operator=(DynamicExpression&& in) = default;
	
	DynamicExpression(const std::shared_ptr<ExpressionBase>& next) :
	    mNext(next)
	{}

        template <class Expr>
        DynamicExpression(Expr ex) : mNext( std::make_shared<Expr>(ex) ) {}
        
	inline void operator()(size_t mlast, DExt last) const override final;
	inline void operator()(size_t mlast = 0) const override final;

        inline DExt dRootSteps(std::intptr_t iPtrNum = 0) const override final;
        inline DExt dExtension() const override final;

    };


    template <class Expr>
    class ExpressionHolder : public ExpressionBase
    {
    private:
	ExpressionHolder() = default;
	
        DynamicExpression mExpr;
	typedef decltype(std::declval<Expr>().rootSteps()) ExtType;
	ExtType mExt;

        mutable ExtType mRootSteps;

    public:
	typedef ExpressionBase EB;
	
	static constexpr size_t LAYER = Expr::LAYER + 1;
	static constexpr size_t SIZE = Expr::SIZE;

	ExpressionHolder(const ExpressionHolder& in) = default;
	ExpressionHolder(ExpressionHolder&& in) = default;
	ExpressionHolder& operator=(const ExpressionHolder& in) = default;
	ExpressionHolder& operator=(ExpressionHolder&& in) = default;

	ExpressionHolder(DynamicExpression expr);
	
	inline void operator()(size_t mlast, DExt last) const override final;
	inline void operator()(size_t mlast, ExtType last) const;
	inline void operator()(size_t mlast = 0) const override final;

        DExt dRootSteps(std::intptr_t iPtrNum = 0) const override final;
        DExt dExtension() const override final;

        auto rootSteps(std::intptr_t iPtrNum = 0) const -> ExtType;
	auto extension() const -> ExtType;

    };

} // namespace MultiArrayHelper

/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */

#include <iostream>

namespace MultiArrayHelper
{

    /*****************
     *     F o r     *
     *****************/
    
    template <class IndexClass, class Expr, ForType FT>
    For<IndexClass,Expr,FT>::For(const std::shared_ptr<IndexClass>& indPtr,
				 size_t step, Expr expr) :
	mIndPtr(indPtr), mSPos(mIndPtr->pos()), mMax(mIndPtr->max()), mStep(step),
        mExpr(expr), mExt(mExpr.rootSteps( reinterpret_cast<std::intptr_t>( mIndPtr )))
    {
	assert(mIndPtr != nullptr);
	//VCHECK(mIndPtr->id());
	//VCHECK(mIndPtr->max());
    }

    template <class IndexClass, class Expr, ForType FT>
    For<IndexClass,Expr,FT>::For(const IndexClass* indPtr,
				 size_t step, Expr expr) :
	mIndPtr(indPtr), mSPos(mIndPtr->pos()), mMax(mIndPtr->max()), mStep(step),
        mExpr(expr), mExt(mExpr.rootSteps( reinterpret_cast<std::intptr_t>( mIndPtr )))
    {
	assert(mIndPtr != nullptr);
	//VCHECK(mIndPtr->id());
	//VCHECK(mIndPtr->max());
    }

    template <class IndexClass, class Expr, ForType FT>
    inline void For<IndexClass,Expr,FT>::operator()(size_t mlast, DExt last) const
    {
        operator()(mlast, *reinterpret_cast<ExtType const*>(last.first));
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
	    const ExtType npos = last + mExt*pos;
	    mExpr(mnpos, npos);
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

    template <class IndexClass, class Expr, ForType FT>
    auto For<IndexClass,Expr,FT>::extension() const
	-> ExtType
    {
	return mExt;
    }

    template <class IndexClass, class Expr, ForType FT>
    DExt For<IndexClass,Expr,FT>::dRootSteps(std::intptr_t iPtrNum) const
    {
        mRootSteps = rootSteps(iPtrNum);
        return std::make_pair<size_t const*,size_t>(reinterpret_cast<size_t const*>(&mRootSteps),
						    sizeof(ExtType)/sizeof(size_t));
    }

    template <class IndexClass, class Expr, ForType FT>
    DExt For<IndexClass,Expr,FT>::dExtension() const
    {
        return std::make_pair<size_t const*,size_t>(reinterpret_cast<size_t const*>(&mExt),
						    sizeof(ExtType)/sizeof(size_t));
    }

    /************************
     *   SingleExpression   *
     ************************/
    
    template <class IndexClass, class Expr>
    SingleExpression<IndexClass,Expr>::SingleExpression(const std::shared_ptr<IndexClass>& indPtr,
							Expr expr) :
	mIndPtr(indPtr.get()), mSPos(mIndPtr->pos()), mMax(mIndPtr->max()),
        mExpr(expr), mExt(mExpr.rootSteps( reinterpret_cast<std::intptr_t>( mIndPtr )))
    {
	assert(mIndPtr != nullptr);
	//VCHECK(mIndPtr->id());
	//VCHECK(mIndPtr->max());
    }

    template <class IndexClass, class Expr>
    SingleExpression<IndexClass,Expr>::SingleExpression(const IndexClass* indPtr,
				 Expr expr) :
	mIndPtr(indPtr), mSPos(mIndPtr->pos()), mMax(mIndPtr->max()),
        mExpr(expr), mExt(mExpr.rootSteps( reinterpret_cast<std::intptr_t>( mIndPtr )))
    {
	assert(mIndPtr != nullptr);
	//VCHECK(mIndPtr->id());
	//VCHECK(mIndPtr->max());
    }

    template <class IndexClass, class Expr>
    inline void SingleExpression<IndexClass,Expr>::operator()(size_t mlast, DExt last) const
    {
        operator()(mlast, *reinterpret_cast<ExtType const*>(last.first));
    }

    template <class IndexClass, class Expr>
    inline void SingleExpression<IndexClass,Expr>::operator()(size_t mlast,
							      ExtType last) const
    {
	//typedef typename IndexClass::RangeType RangeType;
	const size_t pos = mIndPtr->pos();
	const size_t mnpos = PosForward<ForType::DEFAULT>::value(mlast, mMax, pos);
	const ExtType npos = last + mExt*pos;
	mExpr(mnpos, npos);
    }

    template <class IndexClass, class Expr>
    inline void SingleExpression<IndexClass,Expr>::operator()(size_t mlast) const
    {
	//typedef typename IndexClass::RangeType RangeType;
	const ExtType last;
	const size_t pos = mIndPtr->pos();
	const size_t mnpos = PosForward<ForType::DEFAULT>::value(mlast, mMax, pos);
	const ExtType npos = last + mExt*pos;
	mExpr(mlast, last);
    }

    template <class IndexClass, class Expr>
    auto SingleExpression<IndexClass,Expr>::rootSteps(std::intptr_t iPtrNum) const
	-> ExtType
    {
	return mExpr.rootSteps(iPtrNum);
    }

    template <class IndexClass, class Expr>
    auto SingleExpression<IndexClass,Expr>::extension() const
	-> ExtType
    {
	return mExt;
    }

    template <class IndexClass, class Expr>
    DExt SingleExpression<IndexClass,Expr>::dRootSteps(std::intptr_t iPtrNum) const
    {
        mRootSteps = rootSteps(iPtrNum);
        return std::make_pair<size_t const*,size_t>(reinterpret_cast<size_t const*>(&mRootSteps),
						    sizeof(ExtType)/sizeof(size_t));
    }

    template <class IndexClass, class Expr>
    DExt SingleExpression<IndexClass,Expr>::dExtension() const
    {
        return std::make_pair<size_t const*,size_t>(reinterpret_cast<size_t const*>(&mExt),
						    sizeof(ExtType)/sizeof(size_t));
    }
    
    /***************************
     *   DynamicExpression   *
     ***************************/

    inline void DynamicExpression::operator()(size_t mlast, DExt last) const
    {
	(*mNext)(mlast,last);
    }

    inline void DynamicExpression::operator()(size_t mlast) const
    {
	(*mNext)(mlast);
    }

    inline DExt DynamicExpression::dRootSteps(std::intptr_t iPtrNum) const
    {
        return mNext->dRootSteps(iPtrNum);
    }

    inline DExt DynamicExpression::dExtension() const
    {
        return mNext->dExtension();
    }

    /************************
     *   ExpressionHolder   *
     ************************/

    template <class Expr>
    ExpressionHolder<Expr>::ExpressionHolder(DynamicExpression expr) : mExpr(expr) {}

    template <class Expr>
    inline void ExpressionHolder<Expr>::operator()(size_t mlast, DExt last) const
    {
	mExpr(mlast,last);
    }

    template <class Expr>
    inline void ExpressionHolder<Expr>::operator()(size_t mlast, ExtType last) const
    {
	mExpr(mlast,
              std::make_pair<size_t const*,size_t>
              (reinterpret_cast<size_t const*>(&last),
               sizeof(ExtType)/sizeof(size_t)));
    }

    template <class Expr>
    inline void ExpressionHolder<Expr>::operator()(size_t mlast) const
    {
	mExpr(mlast);
    }

    template <class Expr>
    DExt ExpressionHolder<Expr>::dRootSteps(std::intptr_t iPtrNum) const
    {
	return mExpr.dRootSteps(iPtrNum);
    }

    template <class Expr>
    DExt ExpressionHolder<Expr>::dExtension() const
    {
	return mExpr.dExtension();
    }

    template <class Expr>
    auto ExpressionHolder<Expr>::rootSteps(std::intptr_t iPtrNum) const
	-> ExtType
    {
	return *reinterpret_cast<ExtType*>( mExpr.dRootSteps(iPtrNum).first );
    }

    template <class Expr>
    auto ExpressionHolder<Expr>::extension() const
	-> ExtType
    {
	return *reinterpret_cast<ExtType*>( mExpr.dExtension().first );
    }


    
} // namespace MultiArrayHelper

#endif
