
#ifndef __cxz_xfor_h__
#define __cxz_xfor_h__

#include <omp.h>

#include "base/base.h"
#include "for_type.h"
#include "for_utils.h"
#include "exttype.h"

namespace CNORXZ
{
    // 'HIDDEN FOR' CLASS for nested for loops in contractions a.s.o.
    // (NO COUNTING OF MASTER POSITION !!!!!)

    template <class Xpr, class PosT>
    class ExprInterface
    {
    public:
	DEFAULT_MEMBERS(ExprInterface);

	Xpr& THIS() { return static_cast<Xpr&>(*this); }
	const Xpr& THIS() const { return static_cast<const Xpr&>(*this); }

	//Sptr<Expr> copy() const { THIS().copy(); }
	
	void operator()(SizeT mlast, PosT last) { THIS()(mlast, last); }
	void operator()(SizeT mlast = 0) { THIS()(mlast); }

	PosT rootSteps(PtrId ptrId = 0) const { return THIS().rootSteps(ptrId); }
	PosT extension() const { return THIS().extenrion(); }
    };

    class VExprBase
    {
    public:
	DEFAULT_MEMBERS(VExprBase);

	virtual Uptr<VExprBase> copy() const = 0;
	
	virtual void vexec(SizeT mlast, DPos last) = 0;
	virtual void vexec(SizeT mlast) = 0;

	virtual DPos vrootSteps(PtrId ptrId) const = 0;
	virtual DPos vextension() const = 0;
    };

    template <class Xpr, class PosT>
    class VExpr : public VExprBase, public Xpr
    {
    public:
	typedef ExprInterface<Xpr,PosT> EI;
	DEFAULT_MEMBERS(VExpr);
	VExpr(const ExprInterface<Xpr,PosT>& a) : Xpr(a.THIS()) {}

	virtual Uptr<VExprBase> copy() const override final { return std::make_unique<VExpr>(*this); }

	virtual void vexec(SizeT mlast, DPos last) override final { EI::THIS()(mlast,last); }
	virtual void vexec(SizeT mlast) override final { EI::THIS()(mlast); }

	virtual DPos vrootSteps(PtrId ptrId) const override final { return EI::THIS().rootSteps(ptrId); }
	virtual DPos vextension() const override final { return EI::THIS().extension(); }
    };
    
    class DExpr : public ObjHandle<VExprBase>,
		  public ExprInterface<DExpr,DPos>
    {
    public:
	DEFAULT_MEMBERS(DExpr);

	inline void operator()(SizeT mlast, DPos last) { mC->vexec(mlast, last); }
	inline void operator()(SizeT mlast) { mC->vexec(mlast); }

	inline DPos rootSteps(PtrId ptrId) const { return mC->vrootSteps(ptrId); }
	inline DPos extension() const { return mC->vextension(); }
    };
    /*
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

    template <class IndexClass, class Expr>
    class SubExpr : public ExpressionBase
    {
    private:
	SubExpr() = default;

	const IndexClass* mIndPtr;
	PtrId mSIPtr;
	size_t mSPos;
	size_t mMax;

        Expr mExpr;
	typedef decltype(mkExt(0).extend(mExpr.rootSteps())) ExtType;
	ExtType mExt;

        const Vector<SizeT>* mSubSet;
        
        mutable ExtType mRootSteps;

    public:
	typedef ExpressionBase EB;
	
	static constexpr size_t LAYER = Expr::LAYER + 1;
	static constexpr size_t SIZE = Expr::SIZE + 1;
	//static constexpr size_t NHLAYER = Expr::NHLAYER + 1;

	DEFAULT_MEMBERS_X(SubExpr);
	
	SubExpr(const Sptr<IndexClass>& indPtr,
		std::intptr_t siptr,
                const vector<size_t>* subset, Expr expr);

	SubExpr(const IndexClass* indPtr, std::intptr_t siptr,
                const vector<size_t>* subset, Expr expr);

	inline void operator()(size_t mlast, DExt last) override final;
	inline void operator()(size_t mlast, ExtType last) ;
	inline void operator()(size_t mlast = 0) override final;

        DExt dRootSteps(std::intptr_t iPtrNum = 0) const override final;
        DExt dExtension() const override final;

        auto rootSteps(std::intptr_t iPtrNum = 0) const -> ExtType;
	auto extension() const -> ExtType;
    };

    template <ForType FT, size_t LAYER>
    struct NHLayer
    {
	template <class Expr>
	static constexpr size_t get()
	{
	    return Expr::NHLAYER + 1;
	}
    };

    template <size_t LAYER>
    struct NHLayer<ForType::HIDDEN,LAYER>
    {
	template <class Expr>
	static constexpr size_t get()
	{
	    return 0;
	}
    };

    template <>
    struct NHLayer<ForType::DEFAULT,1>
    {
	template <class Expr>
	static constexpr size_t get()
	{
	    return Expr::LAYER;
	}
    };

    template <class IndexClass, class Expr, ForType FT, size_t DIV>
    class For : public ExpressionBase
    {
    private:
	For() = default;
		
	typedef typename IndexClass::RangeType RangeType;
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
	//static constexpr size_t MAX = RangeType::SIZE / DIV;
	//static constexpr size_t NHLAYER = (FT == ForType::HIDDEN) ? 0 : Expr::NHLAYER + 1;

	DEFAULT_MEMBERS(For);
	
	For(const Sptr<IndexClass>& indPtr,
	    size_t step, Expr expr);

	For(const IndexClass* indPtr,
	    size_t step, Expr expr);

	inline void operator()(size_t mlast, DExt last) override final;
	inline void operator()(size_t mlast, ExtType last) ;
	inline void operator()(size_t mlast = 0) override final;

        PFor<IndexClass,Expr> parallel() const;
        
        DExt dRootSteps(std::intptr_t iPtrNum = 0) const override final;
        DExt dExtension() const override final;

        auto rootSteps(std::intptr_t iPtrNum = 0) const -> ExtType;
	auto extension() const -> ExtType;

    };

    class DynamicExpression : public ExpressionBase
    {
    private:

	size_t mThreadId = 0;
	Sptr<ExpressionBase> mNext;

	DynamicExpression() : mThreadId(omp_get_thread_num()) {}
    public:
	
	static constexpr size_t LAYER = 0;
	static constexpr size_t SIZE = 0;
	static constexpr size_t NHLAYER = 0;

	DynamicExpression(const DynamicExpression& in) :
	    mThreadId(omp_get_thread_num()),
	    mNext( (static_cast<int>(in.mThreadId) == omp_get_thread_num()) ?
		   in.mNext : in.mNext->deepCopy()) {}
	DynamicExpression(DynamicExpression&& in) :
	    mThreadId(omp_get_thread_num()),
	    mNext( (static_cast<int>(in.mThreadId) == omp_get_thread_num()) ?
		   in.mNext : in.mNext->deepCopy()) {}
	DynamicExpression& operator=(const DynamicExpression& in)
	{
	    mThreadId = omp_get_thread_num();
	    mNext = (static_cast<int>(in.mThreadId) == omp_get_thread_num()) ?
		in.mNext : in.mNext->deepCopy();
	    return *this;
	}
	
	DynamicExpression& operator=(DynamicExpression&& in)
	{
	    mThreadId = omp_get_thread_num();
	    mNext = (static_cast<int>(in.mThreadId) == omp_get_thread_num()) ?
		in.mNext : in.mNext->deepCopy();
	    return *this;
	}
	
	DynamicExpression(const Sptr<ExpressionBase>& next) :
	    mNext(next)
	{}

        template <class Expr>
        DynamicExpression(const ExpressionBase& next) :
	    mNext(std::make_shared<Expr>(next))
	{}

        template <class Expr>
        DynamicExpression(Expr ex) : mNext( std::make_shared<Expr>(ex) ) {}
        
	virtual Sptr<ExpressionBase> deepCopy() const override final
	{
	    return std::make_shared<DynamicExpression>(*this);
	}

	inline void operator()(size_t mlast, DExt last) override final;
        inline void operator()(size_t mlast, DExtT last) { (*this)(mlast,last.get()); }
	inline void operator()(size_t mlast = 0) override final;

        inline DExt dRootSteps(std::intptr_t iPtrNum = 0) const override final;
        inline DExt dExtension() const override final;

        inline DExtT rootSteps(std::intptr_t iPtrNum = 0) const { return DExtT(dRootSteps(iPtrNum)); }
        inline DExtT extension() const { return DExtT(dExtension()); }

    };

    */
} 

/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */


namespace CNORXZ
{
    
    /*****************
     *     F o r     *
     *****************/
    /*
    template <class IndexClass, class Expr, ForType FT, size_t DIV>
    For<IndexClass,Expr,FT,DIV>::For(const Sptr<IndexClass>& indPtr,
				 size_t step, Expr expr) :
	mIndPtr(indPtr.get()), mSPos(mIndPtr->pos()), mMax(mIndPtr->max()), mStep(step),
        mExpr(expr), mExt(mExpr.rootSteps( reinterpret_cast<std::intptr_t>( mIndPtr )))
    {
	assert(mMax % DIV == 0);
	assert(mIndPtr != nullptr);
    }

    template <class IndexClass, class Expr, ForType FT, size_t DIV>
    For<IndexClass,Expr,FT,DIV>::For(const IndexClass* indPtr,
				 size_t step, Expr expr) :
	mIndPtr(indPtr), mSPos(mIndPtr->pos()), mMax(mIndPtr->max()), mStep(step),
        mExpr(expr), mExt(mExpr.rootSteps( reinterpret_cast<std::intptr_t>( mIndPtr )))
    {
	//VCHECK(mMax);
	//VCHECK(DIV);
	//assert(mMax % DIV == 0);
	assert(mIndPtr != nullptr);
    }

    template <class IndexClass, class Expr, ForType FT, size_t DIV>
    inline void For<IndexClass,Expr,FT,DIV>::operator()(size_t mlast, DExt last)
    {
        operator()(mlast, std::dynamic_pointer_cast<ExtT<ExtType>>(last)->ext());
        //operator()(mlast, *reinterpret_cast<ExtType const*>(last.first));
    }
    
    template <class IndexClass, class Expr, ForType FT, size_t DIV>
    inline void For<IndexClass,Expr,FT,DIV>::operator()(size_t mlast,
						    ExtType last)
    {
	typedef typename IndexClass::RangeType RangeType;
	for(size_t pos = 0u; pos != ForBound<RangeType::ISSTATIC>::template bound<RangeType::SIZE,DIV>(mMax); ++pos){
	    const size_t mnpos = PosForward<FT>::valuex(mlast, mStep, pos);
	    const ExtType npos = last + mExt*pos;
	    mExpr(mnpos, npos);
	}
    }

    template <class IndexClass, class Expr, ForType FT, size_t DIV>
    inline void For<IndexClass,Expr,FT,DIV>::operator()(size_t mlast)
    {
	typedef typename IndexClass::RangeType RangeType;
	ExtType last = rootSteps();
	last.zero();
	for(size_t pos = 0u; pos != ForBound<RangeType::ISSTATIC>::template bound<RangeType::SIZE,DIV>(mMax); ++pos){
	    const size_t mnpos = PosForward<FT>::valuex(mlast, mStep, pos);
	    const ExtType npos = last + mExt*pos;
	    mExpr(mnpos, npos);
	}
    }
    
    
    template <class IndexClass, class Expr, ForType FT, size_t DIV>
    auto For<IndexClass,Expr,FT,DIV>::rootSteps(std::intptr_t iPtrNum) const
	-> ExtType
    {
	return mExpr.rootSteps(iPtrNum);
    }

    template <class IndexClass, class Expr, ForType FT, size_t DIV>
    auto For<IndexClass,Expr,FT,DIV>::extension() const
	-> ExtType
    {
	return mExt;
    }

    template <class IndexClass, class Expr, ForType FT, size_t DIV>
    DExt For<IndexClass,Expr,FT,DIV>::dRootSteps(std::intptr_t iPtrNum) const
    {
        return std::make_shared<ExtT<ExtType>>(rootSteps(iPtrNum));
        //mRootSteps = rootSteps(iPtrNum);
        //return std::make_pair<size_t const*,size_t>(reinterpret_cast<size_t const*>(&mRootSteps),
	//					    sizeof(ExtType)/sizeof(size_t));
    }

    template <class IndexClass, class Expr, ForType FT, size_t DIV>
    DExt For<IndexClass,Expr,FT,DIV>::dExtension() const
    {
        return std::make_shared<ExtT<ExtType>>(mExt);
        //return std::make_pair<size_t const*,size_t>(reinterpret_cast<size_t const*>(&mExt),
	//					    sizeof(ExtType)/sizeof(size_t));
    }

    template <class IndexClass, class Expr, ForType FT, size_t DIV>
    PFor<IndexClass,Expr,DIV> For<IndexClass,Expr,FT,DIV>::parallel() const
    {
        static_assert(FT == ForType::DEFAULT, "hidden for not parallelizable");
        return PFor<IndexClass,Expr,DIV>(mIndPtr, mStep, mExpr);
    }
    */
    /******************
     *    P F o r     *
     ******************/
    /*
    template <class IndexClass, class Expr, size_t DIV>
    PFor<IndexClass,Expr,DIV>::PFor(const Sptr<IndexClass>& indPtr,
				 size_t step, Expr expr) :
	mIndPtr(indPtr.get()), mSPos(mIndPtr->pos()), mMax(mIndPtr->max()), mStep(step),
        mExpr(expr), mExt(mExpr.rootSteps( reinterpret_cast<std::intptr_t>( mIndPtr )))
    {
	//assert(mMax % DIV == 0);
	assert(mIndPtr != nullptr);
    }

    template <class IndexClass, class Expr, size_t DIV>
    PFor<IndexClass,Expr,DIV>::PFor(const IndexClass* indPtr,
				 size_t step, Expr expr) :
	mIndPtr(indPtr), mSPos(mIndPtr->pos()), mMax(mIndPtr->max()), mStep(step),
        mExpr(expr), mExt(mExpr.rootSteps( reinterpret_cast<std::intptr_t>( mIndPtr )))
    {
	assert(mMax % DIV == 0);
	assert(mIndPtr != nullptr);
    }

    template <class IndexClass, class Expr, size_t DIV>
    inline void PFor<IndexClass,Expr,DIV>::operator()(size_t mlast, DExt last)
    {
        operator()(mlast, std::dynamic_pointer_cast<ExtT<ExtType>>(last)->ext());
        //operator()(mlast, *reinterpret_cast<ExtType const*>(last.first));
    }
    
    template <class IndexClass, class Expr, size_t DIV>
    inline void PFor<IndexClass,Expr,DIV>::operator()(size_t mlast,
						    ExtType last)
    {
        CHECK;
	typedef typename IndexClass::RangeType RangeType;
        int pos = 0;
        size_t mnpos = 0;
        ExtType npos;
#pragma omp parallel shared(mExpr) private(pos,mnpos,npos)
        {
            auto expr = mExpr;
#pragma omp for nowait
            for(pos = 0; pos < static_cast<int>(ForBound<RangeType::ISSTATIC>::template bound<RangeType::SIZE,DIV>(mMax)); pos++){
                mnpos = PosForward<ForType::DEFAULT>::valuex(mlast, mStep, pos);
                npos = last + mExt*static_cast<size_t>(pos);
                expr(mnpos, npos);
            }
        }
    }

    template <class IndexClass, class Expr, size_t DIV>
    inline void PFor<IndexClass,Expr,DIV>::operator()(size_t mlast)
    {
        CHECK;
	ExtType last = rootSteps();
	last.zero();
        int pos = 0;
        size_t mnpos = 0;
        ExtType npos = rootSteps();
	npos.zero();
#pragma omp parallel shared(mExpr) private(pos,mnpos,npos)
        {
            auto expr = mExpr;
#pragma omp for nowait
            for(pos = 0; pos < static_cast<int>(ForBound<RangeType::ISSTATIC>::template bound<RangeType::SIZE,DIV>(mMax)); pos++){
                mnpos = PosForward<ForType::DEFAULT>::valuex(mlast, mStep, pos);
                npos = last + mExt*static_cast<size_t>(pos);
                expr(mnpos, npos);
            }
        }
    }
    
    
    template <class IndexClass, class Expr, size_t DIV>
    auto PFor<IndexClass,Expr,DIV>::rootSteps(std::intptr_t iPtrNum) const
	-> ExtType
    {
	return mExpr.rootSteps(iPtrNum);
    }

    template <class IndexClass, class Expr, size_t DIV>
    auto PFor<IndexClass,Expr,DIV>::extension() const
	-> ExtType
    {
	return mExt;
    }

    template <class IndexClass, class Expr, size_t DIV>
    DExt PFor<IndexClass,Expr,DIV>::dRootSteps(std::intptr_t iPtrNum) const
    {
        return std::make_shared<ExtT<ExtType>>(rootSteps(iPtrNum));
        //mRootSteps = rootSteps(iPtrNum);
        //return std::make_pair<size_t const*,size_t>(reinterpret_cast<size_t const*>(&mRootSteps),
	//					    sizeof(ExtType)/sizeof(size_t));
    }

    template <class IndexClass, class Expr, size_t DIV>
    DExt PFor<IndexClass,Expr,DIV>::dExtension() const
    {
        return std::make_shared<ExtT<ExtType>>(mExt);
        //return std::make_pair<size_t const*,size_t>(reinterpret_cast<size_t const*>(&mExt),
	//					    sizeof(ExtType)/sizeof(size_t));
    }
    */
    
    /****************
     *   SubExpr    *
     ****************/
    /*
    template <class IndexClass, class Expr>
    SubExpr<IndexClass,Expr>::SubExpr(const Sptr<IndexClass>& indPtr,
				      std::intptr_t siptr,
                                      const vector<size_t>* subset, Expr expr) :
	mIndPtr(indPtr.get()), mSIPtr(siptr), mSPos(mIndPtr->pos()), mMax(mIndPtr->max()),
        mExpr(expr),
	mExt( mkExt(0).extend( mExpr.rootSteps( reinterpret_cast<std::intptr_t>( mIndPtr )) ) ),
        mSubSet(subset)
    {
	assert(mIndPtr != nullptr);
    }

    template <class IndexClass, class Expr>
    SubExpr<IndexClass,Expr>::SubExpr(const IndexClass* indPtr, std::intptr_t siptr,
                                      const vector<size_t>* subset, Expr expr) :
	mIndPtr(indPtr), mSIPtr(siptr), mSPos(mIndPtr->pos()), mMax(mIndPtr->max()),
        mExpr(expr),
	mExt( mkExt(0).extend( mExpr.rootSteps( reinterpret_cast<std::intptr_t>( mIndPtr )) ) ),
        mSubSet(subset)
    {
	assert(mIndPtr != nullptr);
    }

    template <class IndexClass, class Expr>
    inline void SubExpr<IndexClass,Expr>::operator()(size_t mlast, DExt last)
    {
        operator()(mlast, std::dynamic_pointer_cast<ExtT<ExtType>>(last)->ext());
        //operator()(mlast, *reinterpret_cast<ExtType const*>(last.first));
    }
    
    template <class IndexClass, class Expr>
    inline void SubExpr<IndexClass,Expr>::operator()(size_t mlast,
                                                     ExtType last)
    {
        const size_t pos = (*mSubSet)[last.val()];
        const size_t mnpos = mlast;
        const ExtType npos = last + mExt*pos;
        mExpr(mnpos, getX<1>( npos ));
    }

    template <class IndexClass, class Expr>
    inline void SubExpr<IndexClass,Expr>::operator()(size_t mlast)
    {
	ExtType last = rootSteps();
	last.zero();
        const size_t pos = (*mSubSet)[last.val()];
        const size_t mnpos = mlast;
        const ExtType npos = last + mExt*pos;
        mExpr(mnpos, getX<1>( npos ));
    }
    
    
    template <class IndexClass, class Expr>
    auto SubExpr<IndexClass,Expr>::rootSteps(std::intptr_t iPtrNum) const
	-> ExtType
    {
	return mkExt(iPtrNum == mSIPtr ? 1 : 0).extend(mExpr.rootSteps(iPtrNum));
    }

    template <class IndexClass, class Expr>
    auto SubExpr<IndexClass,Expr>::extension() const
	-> ExtType
    {
	return mExt;
    }

    template <class IndexClass, class Expr>
    DExt SubExpr<IndexClass,Expr>::dRootSteps(std::intptr_t iPtrNum) const
    {
        return std::make_shared<ExtT<ExtType>>(rootSteps(iPtrNum));
        //mRootSteps = rootSteps(iPtrNum);
        //return std::make_pair<size_t const*,size_t>(reinterpret_cast<size_t const*>(&mRootSteps),
        //sizeof(ExtType)/sizeof(size_t));
    }

    template <class IndexClass, class Expr>
    DExt SubExpr<IndexClass,Expr>::dExtension() const
    {
        return std::make_shared<ExtT<ExtType>>(mExt);
        //return std::make_pair<size_t const*,size_t>(reinterpret_cast<size_t const*>(&mExt),
	//					    sizeof(ExtType)/sizeof(size_t));
    }
    */
    /***************************
     *   DynamicExpression   *
     ***************************/
    /*
    inline void DynamicExpression::operator()(size_t mlast, DExt last)
    {
	(*mNext)(mlast,last);
    }

    inline void DynamicExpression::operator()(size_t mlast)
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

    */
} // namespace CNORXZInternal

#endif