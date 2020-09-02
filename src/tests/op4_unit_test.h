
#include "test_header.h"
#include <ctime>

typedef OperationRoot<double,CR,DR> OpCD;
typedef OperationRoot<double,DR> OpD;
typedef CR::IndexType CI;
typedef std::shared_ptr<CI> CIP;

template <typename T, class Op>
DynamicO<T> mkDynOp1(const Op& op)
{
    return DynamicO<T>(op);
}

template <class ROP>
class HighLevelOpBase
{
public:
    
    typedef OperationRoot<double,CR,CR> OpCC;
    typedef OperationRoot<double,CR,DR> OpCD;
    typedef OperationRoot<double,DR> OpD;

    template <class... Indices>
    struct RetT
    {
	DynamicO<OpH<OperationRoot<double,typename Indices::RangeType...>>> op;
	DynamicO<size_t> outer;

	template <class Op, class... Ops>
	void appendOuterM(const Op& op, const Ops&... ops)
	{
	    // does not check anything regarding input !!!
	    if(outer.init()){
		outer = mkDynOp1<size_t>(mkMOp<size_t>(outer,op,ops...));
	    }
	    else {
		outer = mkDynOp1<size_t>(mkMOp<size_t>(op,ops...));
	    }
	}

	void appendOuterM()
	{}

	void appendOuter(const DynamicO<size_t>& in)
	{
	    if(in.init()){
		if(outer.init()){
		    outer = mkDynOp1<size_t>(mkMOp<size_t>(outer,in));
		}
		else {
		    outer = in;
		}
	    }
	}

	void appendOuter(const RetT& in)
	{
	    appendOuter(in.outer);
	}

    };
    
    virtual bool root() const = 0;
    
    virtual RetT<CI,CI> create(const std::shared_ptr<CI> ind1,
			       const std::shared_ptr<CI> ind2) = 0;

    //virtual const OperationRoot<double,CR,DR>* get2() const = 0;
    virtual const ROP* get() const = 0;

};

template <class ROP>
class HighLevelOpRoot : public HighLevelOpBase<ROP>
{
private:
    typedef HighLevelOpBase<ROP> B;
    
    ROP mOp;
public:

    HighLevelOpRoot(const ROP& op) : mOp(op) {}
    
    virtual bool root() const override final
    {
	return true;
    }
    
    virtual typename B::RetT<CI,CI> create(const std::shared_ptr<CI> ind1,
					   const std::shared_ptr<CI> ind2) override final
    {
	assert(0);
	return typename B::RetT<CI,CI>();
    }

    virtual const ROP* get() const override final
    {
	return &mOp;
    }

   
};

template <class OpF, class... Ops>
auto mkFOp(const Ops&... ops)
{
    return Operation<double,OpF,Ops...>(ops...);
}


template <size_t N>
struct Create
{
    template <class... Indices>
    struct cx
    {
	template <class ROP, class OpF, class... OPs>
	struct ccx
	{
	    template <size_t M, class... DOPs>
	    static inline void
	    cccx(typename HighLevelOpBase<ROP>::RetT<Indices...>& res,
		 const std::array<std::shared_ptr<HighLevelOpBase<ROP>>,M>& in,
		 const std::shared_ptr<Indices>&... inds,
		 const OPs&... ops,
		 const DOPs&... dops)
	    {
		static_assert(N > 0, "N > 0 failed");
		auto& inn = std::get<N>(in);
		if(not inn->root()){
		    auto dop = inn->create(inds...);
		    auto op = *dop.op.data()->mOp;
		    typedef decltype(op) OP;
		    res.appendOuter(dop);
		    Create<N-1>::template cx<Indices...>::template ccx<ROP,OpF,OP,OPs...>::template cccx<M>
			(res, in, inds..., op, ops..., dop, dops...);
		}
		else {
		    auto& op = *inn->get();
		    typedef typename std::remove_reference<decltype(op)>::type OP;
		    Create<N-1>::template cx<Indices...>::template ccx<ROP,OpF,OP,OPs...>::template cccx<M>
			(res, in, inds..., op, ops..., dops...);
		}
	    }
	};
    };
};

template <>
struct Create<0>
{
    template <class... Indices>
    struct cx
    {
	template <class ROP, class OpF, class... OPs>
	struct ccx
	{
	    template <size_t M, class... DOPs>
	    static inline void
	    cccx(typename HighLevelOpBase<ROP>::RetT<Indices...>& res,
		 const std::array<std::shared_ptr<HighLevelOpBase<ROP>>,M>& in,
		 const std::shared_ptr<Indices>&... inds,
		 const OPs&... ops,
		 const DOPs&... dops)
	    {
		auto& inn = std::get<0>(in);
		if(not inn->root()){
		    auto dop = inn->create(inds...);
		    auto op = *dop.op.data()->mOp;
		    res.appendOuter(dop);
		    res.op = mkDynOutOp(mkFOp<OpF>(op,ops...), inds...);
		    res.appendOuterM(dop.op,dops.op...);
		}
		else {
		    auto& op = *inn->get();
		    res.op = mkDynOutOp(mkFOp<OpF>(op,ops...), inds...);
		    res.appendOuterM(dops.op...);
		}
	    }
	};
    };
};

template <class ROP, class OpF, size_t N>
class HighLevelOp : public HighLevelOpBase<ROP>
{
private:
    std::array<std::shared_ptr<HighLevelOpBase<ROP>>,N> mIn;
    
public:
    typedef HighLevelOpBase<ROP> B;

    HighLevelOp(std::array<std::shared_ptr<HighLevelOpBase<ROP>>,N> in) : mIn(in) {}
    
    virtual bool root() const override final
    {
        return false;
    }
    
    virtual const ROP* get() const override final
    { assert(0); return nullptr; }
    
    virtual typename B::RetT<CI,CI> create(const std::shared_ptr<CI> ind1,
			   const std::shared_ptr<CI> ind2) override final
    {
	typename B::RetT<CI,CI> res;
	Create<N-1>::template cx<CI,CI>::template ccx<ROP,OpF>::template cccx<N>(res,mIn,ind1,ind2);
        return res;
    }

};


template <class ROP>
class HighLevelOpHolder
{
private:
    std::shared_ptr<HighLevelOpBase<ROP>> mOp;

public:
    HighLevelOpHolder() = default;
    HighLevelOpHolder(const HighLevelOpHolder& in) = default; 
    HighLevelOpHolder(HighLevelOpHolder&& in) = default;
    HighLevelOpHolder& operator=(const HighLevelOpHolder& in) = default;
    HighLevelOpHolder& operator=(HighLevelOpHolder&& in) = default;
    
    HighLevelOpHolder(const std::shared_ptr<HighLevelOpBase<ROP>>& op) : mOp(op) {}

    bool root() const { return mOp->root(); }
    
    auto create(const std::shared_ptr<CI> ind1,
		const std::shared_ptr<CI> ind2) const { return mOp->create(ind1,ind2); }

    auto get() const { return mOp->get(); }

    std::shared_ptr<HighLevelOpBase<ROP>> op() const { return mOp; }
    
    HighLevelOpHolder operator*(const HighLevelOpHolder& in) const
    {
	return HighLevelOpHolder<ROP>
	    ( std::make_shared<HighLevelOp<ROP,multipliesx<double,double>,2>>
	      ( std::array<std::shared_ptr<HighLevelOpBase<ROP>>,2>({mOp, in.mOp}) ) );
    }

    HighLevelOpHolder operator+(const HighLevelOpHolder& in) const
    {
	return HighLevelOpHolder<ROP>
	    ( std::make_shared<HighLevelOp<ROP,plusx<double,double>,2>>
	      ( std::array<std::shared_ptr<HighLevelOpBase<ROP>>,2>({mOp, in.mOp}) ) );
    }

    HighLevelOpHolder operator-(const HighLevelOpHolder& in) const
    {
	return HighLevelOpHolder<ROP>
	    ( std::make_shared<HighLevelOp<ROP,minusx<double,double>,2>>
	      ( std::array<std::shared_ptr<HighLevelOpBase<ROP>>,2>({mOp, in.mOp}) ) );
    }
    
    HighLevelOpHolder operator/(const HighLevelOpHolder& in) const
    {
	return HighLevelOpHolder<ROP>
	    ( std::make_shared<HighLevelOp<ROP,dividesx<double,double>,2>>
	      ( std::array<std::shared_ptr<HighLevelOpBase<ROP>>,2>({mOp, in.mOp}) ) );
    }

    template <class MIndex, class... Indices>
    HighLevelOpHolder& assign(const HighLevelOpHolder& in,
			      const std::shared_ptr<MIndex>& mi,
			      const std::shared_ptr<Indices>&... inds)
    {
	auto xx = mkArrayPtr<double>(nullr());
	auto& opr = *mOp->get();
	auto loop = mkPILoop
	    ( [&opr,&in,&xx,&inds...,this](){
		auto inx = in;
		auto dop = inx.create(inds...);
		auto gexp = mkDynOp1<size_t>(mkMOp<size_t>(dop.outer,dop.op));
		auto xloop = mkILoop(std::make_tuple(*dop.op.data()->mOp),
				     std::make_tuple(inds...),
				     std::make_tuple(xx),
				     std::make_tuple(opr.assign( *dop.op.data()->mOp,
								 mkMIndex(inds...) )),
				     std::array<size_t,1>({1}), std::array<size_t,1>({0}));
		return mkGetExpr(gexp, xloop); });
        mi->pifor(1,loop)();
	return *this;
    }

    template <class MIndex, class... Indices>
    HighLevelOpHolder& plus(const HighLevelOpHolder& in,
			    const std::shared_ptr<MIndex>& mi,
			    const std::shared_ptr<Indices>&... inds)
    {
	auto xx = mkArrayPtr<double>(nullr());
	auto& opr = *mOp->get();
	auto loop = mkPILoop
	    ( [&opr,&in,&xx,&inds...,this](){
		auto inx = in;
		auto dop = inx.create(inds...);
		auto gexp = mkDynOp1<size_t>(mkMOp<size_t>(dop.outer,dop.op));
		auto xloop = mkILoop(std::make_tuple(*dop.op.data()->mOp),
				     std::make_tuple(inds...),
				     std::make_tuple(xx),
				     std::make_tuple(opr.plus( *dop.op.data()->mOp,
							       mkMIndex(inds...) )),
				     std::array<size_t,1>({1}), std::array<size_t,1>({0}));
		return mkGetExpr(gexp, xloop); });
        mi->pifor(1,loop)();
	return *this;
    }
};

typedef OperationRoot<double,CR,DR> OpType1;

template <class ROP>
HighLevelOpHolder<ROP> mkHLO(const ROP& op)
{
    return HighLevelOpHolder<ROP>(std::make_shared<HighLevelOpRoot<ROP>>( op ) );
}

template <class ROP>
HighLevelOpHolder<ROP> exp(const HighLevelOpHolder<ROP>& in)
{
    return HighLevelOpHolder<ROP>
	( std::make_shared<HighLevelOp<ROP,x_exp<double>,1>>
	  ( std::array<std::shared_ptr<HighLevelOpBase<ROP>>,1>( {in.op()} ) ) );
}
