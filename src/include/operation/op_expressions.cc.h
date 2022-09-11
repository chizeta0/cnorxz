
#include "op_expressions.h"

namespace CNORXZ
{
    namespace
    {
	using namespace CNORXZInternal;
    }

    /************************
     *   AssignmentExpr     *
     ************************/

    template <OpIndexAff OIA, class ExtType>
    inline size_t opIndexResolve(size_t start, ExtType last)
    {
        if constexpr(OIA == OpIndexAff::EXTERN){
            return last.val();
        }
        if constexpr(OIA == OpIndexAff::TARGET){
            return start;
        }
        return 0;
    }

    template <typename T, class Func, class AT, class Target, class OpClass, OpIndexAff OIA>
    AssignmentExpr<T,Func,AT,Target,OpClass,OIA>::AssignmentExpr(const AccessTemplate<AT>& dataAcc, const Target& tar, const OpClass& sec) :
        mTar(tar), mSec(sec), mDataAcc(static_cast<const AT&>(dataAcc)) {}

    template <typename T, class Func, class AT, class Target, class OpClass, OpIndexAff OIA>
    inline void AssignmentExpr<T,Func,AT,Target,OpClass,OIA>::operator()(size_t start)
    {
        ExtType last = rootSteps();
	last.zero();
	// TODO: ask MA container for data (ptr)!!!
	mDataAcc.template exec<Func>(opIndexResolve<OIA>(start,last),mSec,last.next());
    }

    template <typename T, class Func, class AT, class Target, class OpClass, OpIndexAff OIA>
    inline void AssignmentExpr<T,Func,AT,Target,OpClass,OIA>::operator()(size_t start, ExtType last)
    {
	mDataAcc.template exec<Func>(opIndexResolve<OIA>(start,last),mSec,last.next());
    }
    
    template <typename T, class Func, class AT, class Target, class OpClass, OpIndexAff OIA>
    typename AssignmentExpr<T,Func,AT,Target,OpClass,OIA>::ExtType
    AssignmentExpr<T,Func,AT,Target,OpClass,OIA>::rootSteps(std::intptr_t iPtrNum) const
    {
	return mTar.rootSteps(iPtrNum).extend( mSec.rootSteps(iPtrNum) );
    }

    template <typename T, class Func, class AT, class Target, class OpClass, OpIndexAff OIA>
    inline void AssignmentExpr<T,Func,AT,Target,OpClass,OIA>::operator()(size_t mlast, DExt last)
    {
        (*this)(mlast, std::dynamic_pointer_cast<ExtT<ExtType>>(last)->ext());
    }

    template <typename T, class Func, class AT, class Target, class OpClass, OpIndexAff OIA>
    inline DExt AssignmentExpr<T,Func,AT,Target,OpClass,OIA>::dRootSteps(std::intptr_t iPtrNum) const
    {
        return std::make_shared<ExtT<ExtType>>(rootSteps(iPtrNum));
    }

    template <typename T, class Func, class AT, class Target, class OpClass, OpIndexAff OIA>
    inline DExt AssignmentExpr<T,Func,AT,Target,OpClass,OIA>::dExtension() const
    {
        return nullptr; //???!!!
    }

    
} // namespace CNORXZ
