
#ifndef __high_level_operation_h__
#define __high_level_operation_h__

#include "base_def.h"
#include "dynamic_operation.h"
#include "multi_array_operation.h"

namespace MultiArrayTools
{

    template <class DOp, class... Indices>
    class HLOBuilderBase
    {
    protected:
	//typedef typename DOp::value_type IOp;
	DOp mDOp;
	typedef decltype(mDOp.data()->mOp) IOpP;
	IOpP mIOpP;
	typedef std::shared_ptr<HLOBuilderBase<DOp>> HLOBP;
	HLOBP mPrev;
    public:

	HLOBuilderBase(const HLOBP& prev) : mPrev(prev) {}
	
	virtual void create(const std::shared_ptr<Indices>&... inds) = 0;

	virtual std::shared_ptr<HLOBuilderBase<DOp,Indices...>>
	operator*(const std::shared_ptr<HLOBuilderBase<DOp,Indices...>>& in) const = 0;

	const IOpP& get() const
	{
	    return mIOpP;
	}

	const DOp& dget() const
	{
	    return mDOp;
	}

	const HLOBP& prev() const
	{
	    return mPrev;
	}

	void appendPrev(const HLOBP& prev)
	{
	    if(mPrev){
		mPrev->append(prev);
	    }
	    else {
		mPrev = prev;
	    }
	}
	
	template <class Expr>
	DynamicExpression mkGetExpr(const Expr& expr) const
	{
	    if(not mPrev){
		return DynamicExpression( mkGetExpr(mDOp, expr) );
	    }
	    else {
		return DynamicExpression( mkGetExpr(mDOp, mPrev->mkGetExpr(expr) ) );
	    }
	}
    };

    
    template <class DOp, class CF, class... Indices>
    class HLOBuilder : public HLOBuilderBase<DOp,Indices...>
    {
    private:
	typedef HLOBuilderBase<DOp,Indices...> B;
	typedef typename B::HLOBP HLOBP;
	CF mCF;
    public:

	HLOBuilder(const HLOBP& prev,
		   const CF& cf) : B(prev), mCF(cf) {}

	HLOBuilder(const CF& cf) : B(nullptr), mCF(cf) {}
	
	virtual void create(const std::shared_ptr<Indices>&... inds) override final
	{
	    B::mPrev->create(inds...);
	    auto op = mCF();
	    B::mDOp = mkDynOutOp(op, inds...);
	    B::mIOpP = B::mDOp.data()->mOp;
	}

	virtual std::shared_ptr<B>
	operator*(const std::shared_ptr<HLOBuilderBase<DOp,Indices...>>& in) const override final
	{
	    HLOBP xprev = B::mPrev;
	    xprev->appendPrev(in->prev());
	    xprev->append(in);
	    xprev->appendPrev(std::make_shared<HLOBuilder<DOp,CF,Indices...>>(*this));
	    return std::make_shared<HLOBuilder<DOp,decltype(*B::mIOpP*in->mkDOp().data()->mOp),Indices...>>
		(xprev, [&in,this]() { this->create(); return *B::mIOpP*in->mkDOp().data()->mOp; });
	}
    };


    template <class DOp, class Op, class... Indices>
    class HLOBuilderRoot : public HLOBuilderBase<DOp,Indices...>
    {
    private:
	typedef HLOBuilderBase<DOp,Indices...> B;
	typedef typename B::HLOBP HLOBP;
	Op mOp;
    public:

	HLOBuilderRoot(const Op& op) : B(nullptr), mOp(op) {}

	virtual void create(const std::shared_ptr<Indices>&... inds) override final
	{
	    //B::mPrev->create(inds...);
	    //mOp = mCF();
	    //B::mDOp = mkDynOutOp(op, inds...);
	    //B::mIOpP = B::mDOp.data()->mOp;
	}

	virtual std::shared_ptr<B>
	operator*(const std::shared_ptr<HLOBuilderBase<DOp,Indices...>>& in) const override final
	{
	    HLOBP xprev = in->prev();
	    xprev->append(in);
	    //xprev->appendPrev(in.prev());
	    //xprev->appendPrev(std::make_shared<HLOBuilder<DOp,CF>>(*this));
	    return std::make_shared<HLOBuilder<DOp,decltype(mOp*in->mkDOp().data()->mOp),Indices...>>
		(xprev, [&in,this]() { this->create(); return mOp*in->mkDOp().data()->mOp; });
	}
    };

    template <class DOp, class CF,class... Indices>
    std::shared_ptr<HLOBuilderBase<DOp,Indices...>>
    mkHLOBuilder( const CF& cf, const std::shared_ptr<Indices>&... inds)
    {
	return std::make_shared<HLOBuilder<DOp,CF,Indices...>>(cf);
    }

    template <class DOp, class Op ,class... Indices>
    std::shared_ptr<HLOBuilderBase<DOp,Indices...>>
    mkHLOBuilderRoot( const Op& op, const std::shared_ptr<Indices>&... inds )
    {
	return std::make_shared<HLOBuilderRoot<DOp,Op,Indices...>>(op);
    }
}

#endif
