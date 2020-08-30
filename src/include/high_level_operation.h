
#ifndef __high_level_operation_h__
#define __high_level_operation_h__

#include "base_def.h"
#include "dynamic_operation.h"

namespace MultiArrayTools
{

    template <class DOp>
    class HLOBuilderBase
    {

    public:
	virtual DOp mkDOp() const = 0;
	virtual DynamicExpression mkGetExpr() const = 0;
    };

    template <class DOp, class Op>
    class HLOBuilder : public HLOBuilderBase<DOp>
    {
    private:
	Op mOp;
    public:
	// ...
	HLOBuilder(const Op& op) : mOp(op) {}
	
	auto operator*(const HLOBuilderBase<DOp>& in)
	{
	}
    }
    
}

#endif
