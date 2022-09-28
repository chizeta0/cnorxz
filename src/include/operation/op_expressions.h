
#ifndef __cxz_op_expressions__
#define __cxz_op_expressions__

#include "access.h"

namespace CNORXZ
{
    namespace
    {
	using namespace CNORXZInternal;
    }

    enum class OpIndexAff {
	EXTERN = 0,
	TARGET = 1
    };

    template <typename T, class Func, class AT, class Target, class OpClass, OpIndexAff OIA=OpIndexAff::EXTERN>
    class AssignmentExpr : public ExpressionBase
    {
    private:
        AssignmentExpr() = default;

        Target mTar;
        OpClass mSec;
	AT mDataAcc;
            
    public:

        static constexpr size_t LAYER = 0;
        static constexpr size_t NHLAYER = 0;
        static constexpr size_t SIZE = Target::SIZE + OpClass::SIZE;
        typedef decltype(mTar.rootSteps(0).extend( mSec.rootSteps(0) )) ExtType;
	    
	AssignmentExpr(const AccessTemplate<AT>& dataAcc, const Target& tar, const OpClass& sec);
        AssignmentExpr(const AssignmentExpr& in) = default;
        AssignmentExpr(AssignmentExpr&& in) = default;
        AssignmentExpr& operator=(const AssignmentExpr& in) = default;
        AssignmentExpr& operator=(AssignmentExpr&& in) = default;

	virtual std::shared_ptr<ExpressionBase> deepCopy() const override final
	{
	    return std::make_shared<AssignmentExpr<T,Func,AT,Target,OpClass,OIA>>(*this);
	}
	
        inline void operator()(size_t start = 0); 
        inline void operator()(size_t start, ExtType last);
        auto rootSteps(std::intptr_t iPtrNum = 0) const -> ExtType;

        inline void operator()(size_t mlast, DExt last) override final;

        inline DExt dRootSteps(std::intptr_t iPtrNum = 0) const override final;
        inline DExt dExtension() const override final;
    };

} // namespace CNORXZ

#endif
