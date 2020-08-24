
#include "dynamic_operation.h"

namespace MultiArrayTools
{
    template <typename T, class Operation>
    T DynamicOperation<T,Operation>::get(const DExtT& pos) const
    {
	return mOp.get(pos.expl<ET>());
    }
    
    template <typename T, class Operation>
    DynamicOperationBase<T>& DynamicOperation<T,Operation>::set(const DExtT& pos)
    {
	mOp.set(pos.expl<ET>());
	return *this;
    }
    
    template <typename T, class Operation>
    DExtT DynamicOperation<T,Operation>::rootSteps(std::intptr_t iPtrNum) const
    {
	return DExtT(mkDExt(mkExtT(mOp.rootSteps(iPtrNum))),None(0));
    }
    
    template <typename T, class Operation>
    DynamicExpression DynamicOperation<T,Operation>::loop(const DynamicExpression& exp) const
    {
	return mOp.loop(exp);
    }
    
    template <typename T, class Operation>
    const T* DynamicOperation<T,Operation>::data() const
    {
	return mOp.data();
    }

    template <typename T, class Operation>
    std::shared_ptr<DynamicOperationBase<T>> DynamicOperation<T,Operation>::deepCopy() const
    {
        return std::make_shared<DynamicOperation<T,Operation>>(*this);
    }


} // namespace MultiArrayTools
