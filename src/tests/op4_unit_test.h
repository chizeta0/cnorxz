
#include "test_header.h"
#include <ctime>

typedef OperationRoot<double,CR,DR> OpCD;
typedef OperationRoot<double,DR> OpD;
typedef CR::IndexType CI;
typedef std::shared_ptr<CI> CIP;

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
}

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
protected:
    std::shared_ptr<HighLevelOpBase> mPrev
    
public:
    
    typedef OperationRoot<double,CR,CR> OpCC;
    typedef OperationRoot<double,CR,DR> OpCD;
    typedef OperationRoot<double,DR> OpD;

    virtual bool root() const = 0;
    
    virtual DynamicO<OpH<OpRR>>
    create(const std::shared_ptr<CI> ind1,
           const std::shared_ptr<CI> ind2) = 0;

    virtual OperationRoot<double,DR> get1() = 0;
    virtual OperationRoot<double,CR,DR> get2() = 0;

    void appendPrev(const std::shared_ptr<HighLevelOpBase>& in)
    {
        if(mPrev){
            mPrev->appendPrev(in)
        }
        else {
            mPrev = in;
        }
    }
};

class HighLevelOpRoot : public HighLevelOpBase
{
    
}

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

    virtual bool root() const override final
    {
        return false;
    }
    
    virtual DynamicO<OpH<OperationRoot<double,CR,CR>>>
    create(const std::shared_ptr<CI> ind1,
           const std::shared_ptr<CI> ind2) override final
    {
        if(not mIn1->root()){
            DynamicO<OpH<OperationRoot<double,CR,CR>>> dop1 = mIn1->create(ind1,ind2);
            op1 = *dop1.data()->mOp;
            if(not mIn2->root()){
                DynamicO<OpH<OperationRoot<double,CR,CR>>> dop2 = mIn2->create(ind1,ind2);
                op2 = *dop2.data()->mOp;
                return mkDynOutOp(OxRRRR(op1,op2), ind1, ind2);
            }
            else {
                OperationRoot<double,DR,CR>& op2 = mIn2->get2(ind1,ind2);
                return mkDynOutOp(OxRRRR(op1,op2), ind1, ind2);
            }
        }
        return DynamicO<OpH<OperationRoot<double,CR,CR>>>();
    }

};


