
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

    virtual const OperationRoot<double,DR>* get1() const = 0;
    virtual const OperationRoot<double,CR,DR>* get2() const = 0;

};

template <bool SAME>
struct Fwd
{
    template <class O1, class O2>
    static inline const O1* fwd(const O2* in)
    {
	assert(0);
	return nullptr;
    }
};

template <>
struct Fwd<true>
{
    template <class O1, class O2>
    static inline const O1* fwd(const O2* in)
    {
	return in;
    }
};

template <class OR>
class HighLevelOpRoot : public HighLevelOpBase
{
private:
    typedef OperationRoot<double,DR> OType1;
    typedef OperationRoot<double,CR,DR> OType2;
    typedef HighLevelOpBase B;
    
    OR mOp;
public:

    HighLevelOpRoot(const OR& op) : mOp(op) {}
    
    virtual bool root() const override final
    {
	return true;
    }
    
    virtual B::RetT<CI,CI> create(const std::shared_ptr<CI> ind1,
				  const std::shared_ptr<CI> ind2) override final
    {
	assert(0);
	return B::RetT<CI,CI>();
    }

    virtual const OType1* get1() const override final
    {
	return Fwd<std::is_same<OType1,OR>::value>::template fwd<OType1>(&mOp);
    }
    
    virtual const OType2* get2() const override final
    {
	return Fwd<std::is_same<OType2,OR>::value>::template fwd<OType2>(&mOp);
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
	template <class OpF, class... OPs>
	struct ccx
	{
	    template <size_t M, class... DOPs>
	    static inline void
	    cccx(HighLevelOpBase::RetT<Indices...>& res,
		 const std::array<std::shared_ptr<HighLevelOpBase>,M>& in,
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
		    Create<N-1>::template cx<Indices...>::template ccx<OpF,OP,OPs...>::template cccx<M>
			(res, in, inds..., op, ops..., dop, dops...);
		}
		else {
		    auto& op = *inn->get2();
		    typedef typename std::remove_reference<decltype(op)>::type OP;
		    Create<N-1>::template cx<Indices...>::template ccx<OpF,OP,OPs...>::template cccx<M>
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
	template <class OpF, class... OPs>
	struct ccx
	{
	    template <size_t M, class... DOPs>
	    static inline void
	    cccx(HighLevelOpBase::RetT<Indices...>& res,
		 const std::array<std::shared_ptr<HighLevelOpBase>,M>& in,
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
		    auto& op = *inn->get2();
		    res.op = mkDynOutOp(mkFOp<OpF>(op,ops...), inds...);
		    res.appendOuterM(dops.op...);
		}
	    }
	};
    };
};

template <class OpF, size_t N>
class HighLevelOp : public HighLevelOpBase
{
private:
    std::array<std::shared_ptr<HighLevelOpBase>,N> mIn;
    
public:
    typedef HighLevelOpBase B;

    HighLevelOp(std::array<std::shared_ptr<HighLevelOpBase>,N> in) : mIn(in) {}
    
    virtual bool root() const override final
    {
        return false;
    }
    
    virtual const OperationRoot<double,DR>* get1() const override final
    { assert(0); return nullptr; }
    virtual const OperationRoot<double,CR,DR>* get2() const override final
    { assert(0); return nullptr; }
    
    virtual B::RetT<CI,CI> create(const std::shared_ptr<CI> ind1,
			   const std::shared_ptr<CI> ind2) override final
    {
	B::RetT<CI,CI> res;
	Create<N-1>::template cx<CI,CI>::template ccx<OpF>::template cccx<N>(res,mIn,ind1,ind2);
        return res;
    }

};


class HighLevelOpHolder
{
private:
    std::shared_ptr<HighLevelOpBase> mOp;

public:
    HighLevelOpHolder() = default;
    HighLevelOpHolder(const HighLevelOpHolder& in) = default; 
    HighLevelOpHolder(HighLevelOpHolder&& in) = default;
    HighLevelOpHolder& operator=(const HighLevelOpHolder& in) = default;
    HighLevelOpHolder& operator=(HighLevelOpHolder&& in) = default;
    
    HighLevelOpHolder(const std::shared_ptr<HighLevelOpBase>& op) : mOp(op) {}

    bool root() const { return mOp->root(); }
    
    auto create(const std::shared_ptr<CI> ind1,
		const std::shared_ptr<CI> ind2) const { return mOp->create(ind1,ind2); }

    auto get1() const { return mOp->get1(); }
    auto get2() const { return mOp->get2(); }

    std::shared_ptr<HighLevelOpBase> op() const { return mOp; }
    
    HighLevelOpHolder operator*(const HighLevelOpHolder in) const
    {
	return HighLevelOpHolder
	    ( std::make_shared<HighLevelOp<multipliesx<double,double>,2>>
	      ( std::array<std::shared_ptr<HighLevelOpBase>,2>({mOp, in.mOp}) ) );
    }
};

template <class OR>
HighLevelOpHolder mkHLO(const OR& op)
{
    return HighLevelOpHolder(std::make_shared<HighLevelOpRoot<OR>>( op ) );
}

HighLevelOpHolder exp(const HighLevelOpHolder& in)
{
    return HighLevelOpHolder( std::make_shared<HighLevelOp<x_exp<double>,1>>
			      ( std::array<std::shared_ptr<HighLevelOpBase>,1>( {in.op()} ) ) );
}
