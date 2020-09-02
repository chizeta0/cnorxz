
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


class OpHolderBase
{
public:
    virtual std::string stype() const = 0;
    virtual size_t ntype() const = 0;
};

template <class Op>
struct OpName
{
    static std::string sget()
    {
        return "none";
    }

    static size_t nget()
    {
        return 0;
    }
};

template <class Op>
class OpHolder : public OpHolderBase
{
private:
    Op mOp;
public:

    OpHolder(const Op& op) : mOp(op) {}

    virtual std::string stype() const override final
    {
        return OpName<Op>::sget();
    }

    virtual size_t ntype() const override final
    {
        return OpName<Op>::nget();
    }

};



class HighLevelOpBase
{
public:
    
    typedef OperationRoot<double,CR,CR> OpCC;
    typedef OperationRoot<double,CR,DR> OpCD;
    typedef OperationRoot<double,DR> OpD;

    struct RetT
    {
	DynamicO<OpH<OpCC>> op;
	DynamicO<size_t> outer;

	template <class... Ops>
	void appendOuterM(const Ops&... ops)
	{
	    // does not check anything regarding input !!!
	    if(outer.init()){
		outer = mkDynOp1<size_t>(mkMOp<size_t>(outer,ops...));
	    }
	    else {
		outer = mkDynOp1<size_t>(mkMOp<size_t>(ops...));
	    }
	}
	
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
    
    virtual RetT create(const std::shared_ptr<CI> ind1,
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
    
    virtual B::RetT create(const std::shared_ptr<CI> ind1,
			   const std::shared_ptr<CI> ind2) override final
    {
	assert(0);
	return B::RetT();
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


template <class OpF>
class HighLevelOp2 : public HighLevelOpBase
{
private:
    std::shared_ptr<HighLevelOpBase> mIn1;
    std::shared_ptr<HighLevelOpBase> mIn2;
    
public:
    typedef HighLevelOpBase B;
    typedef Operation<double,OpF,B::OpCC,B::OpCC> OxCCxCC;
    typedef Operation<double,OpF,B::OpCD,B::OpCD> OxCDxCD;
    typedef Operation<double,OpF,B::OpCC,B::OpCD> OxCCxCD;
    typedef Operation<double,OpF,B::OpCD,B::OpCC> OxCDxCC;

    HighLevelOp2(const std::shared_ptr<HighLevelOpBase>& in1,
		 const std::shared_ptr<HighLevelOpBase>& in2) : mIn1(in1), mIn2(in2) {}
    
    virtual bool root() const override final
    {
        return false;
    }
    virtual const OperationRoot<double,DR>* get1() const override final
    { assert(0); return nullptr; }
    virtual const OperationRoot<double,CR,DR>* get2() const override final
    { assert(0); return nullptr; }
    
    virtual B::RetT create(const std::shared_ptr<CI> ind1,
			   const std::shared_ptr<CI> ind2) override final
    {
	B::RetT res;
	if(not mIn2->root()){
	    auto dop2 = mIn2->create(ind1,ind2);
            auto op2 = *dop2.op.data()->mOp;
	    res.outer = dop2.outer;
            if(not mIn1->root()){
		auto dop1 = mIn1->create(ind1,ind2);
                auto op1 = *dop1.op.data()->mOp;
                res.op = mkDynOutOp(OxCCxCC(op1,op2), ind1, ind2);
		res.appendOuter(dop1);
		res.appendOuterM(dop1.op,dop2.op);
		return res;
            }
            else {
                const OperationRoot<double,CR,DR>& op1 = *mIn1->get2();
		res.op = mkDynOutOp(OxCDxCC(op1,op2), ind1, ind2);
		res.appendOuterM(dop2.op);
		return res;
            }
        }
	assert(0);
        return res;
    }

};

template <class OpF>
class HighLevelOp1 : public HighLevelOpBase
{
private:
    std::shared_ptr<HighLevelOpBase> mIn1;
public:
    typedef HighLevelOpBase B;
    typedef Operation<double,OpF,B::OpCC> OxCC;
    typedef Operation<double,OpF,B::OpCD> OxCD;

    HighLevelOp1(const std::shared_ptr<HighLevelOpBase>& in1) : mIn1(in1) {}
    
    virtual bool root() const override final
    {
        return false;
    }
    virtual const OperationRoot<double,DR>* get1() const override final
    { assert(0); return nullptr; }
    virtual const OperationRoot<double,CR,DR>* get2() const override final
    { assert(0); return nullptr; }
    
    virtual B::RetT create(const std::shared_ptr<CI> ind1,
			   const std::shared_ptr<CI> ind2) override final
    {
	if(not mIn1->root()){
	    auto dop1 = mIn1->create(ind1,ind2);
	    auto op1 = *dop1.op.data()->mOp;
	    dop1.appendOuterM(dop1.op);
	    dop1.op = mkDynOutOp(OxCC(op1), ind1, ind2);
	    return dop1;
	}
	else {
	    const OperationRoot<double,CR,DR>& op1 = *mIn1->get2();
	    B::RetT res;
	    res.op = mkDynOutOp(OxCD(op1), ind1, ind2); 
	    return res;
	}
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
	    ( std::make_shared<HighLevelOp2<multipliesx<double,double>>>
	      ( mOp, in.mOp ) );
    }
};

template <class OR>
HighLevelOpHolder mkHLO(const OR& op)
{
    return HighLevelOpHolder(std::make_shared<HighLevelOpRoot<OR>>( op ) );
}

HighLevelOpHolder exp(const HighLevelOpHolder& in)
{
    return HighLevelOpHolder( std::make_shared<HighLevelOp1<x_exp<double>>>( in.op() ) );
}
