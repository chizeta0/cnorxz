
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


template <class... Indices>
class XHighLevelOpBase
{
public:
    
    virtual DynamicO<OpH<OperationRoot<double,typename Indices::RangeType>>>
    create(const std::shared_ptr<Indices>&... inds) = 0;
};

class HighLevelOp
{
private:
    std::shared_ptr<OpHolderBase> mOp;
    
public:

    
};


template <class Op, class... Indices>
class XHighLevelOp : public XHighLevelOpBase
{
private:
    Op mOp;
public:
    virtual DynamicO<OpH<OperationRoot<double,typename Indices::RangeType>>>
    create(const std::shared_ptr<Indices>&... inds) override final
    {
        return mkDynOutOp(mOp, inds...);
    }
    
};
