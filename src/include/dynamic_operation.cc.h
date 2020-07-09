
#include "dynamic_operation.h"

namespace MultiArrayTools
{
    template <typename T, class Operation>
    const T& DynamicOperation<T,Operation>::get(const DExt& pos) const
    {
	return mOp.get(pos.expl<ET>());
    }
    
    template <typename T, class Operation>
    DynamicOperationBase& DynamicOperation<T,Operation>::set(const DExt& pos)
    {
	mOp.set(pos.expl<ET>());
	return *this;
    }
    
    template <typename T, class Operation>
    DExt DynamicOperation<T,Operation>::rootSteps(std::intptr_t iPtrNum = 0) const
    {
	return mOp.rootSteps(iPtrNum);
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

} // namespace MultiArrayTools
