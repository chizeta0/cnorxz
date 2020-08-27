
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

    template <typename T, class Operation, class... Ranges>
    DynamicOuterOp<T,Operation,Ranges...>::DynamicOuterOp(const Operation& op,
                                                          const std::shared_ptr<typename Ranges::IndexType>&... inds)
        : mOp(op)
    {
        //...!!!
        // setup operations
    }

    template <typename T, class Operation, class... Ranges>
    OpH<ConstOperationRoot<T,Ranges...>> DynamicOuterOp<T,Operation,Ranges...>::get(const DExtT& pos) const
    {
        mOp.get(pos.expl<ET>());
        //...
        // execute assignment... care about threads!!!
        return OpH<ConstOperationRoot<T,Ranges...>>(); // empty
    }
    
    template <typename T, class Operation, class... Ranges>
    DynamicOperationBase<OpH<ConstOperationRoot<T,Ranges...>>>&
    DynamicOuterOp<T,Operation,Ranges...>::set(const DExtT& pos)
    {
	mOp.set(pos.expl<ET>());
	return *this;
    }
    
    template <typename T, class Operation, class... Ranges>
    DExtT DynamicOuterOp<T,Operation,Ranges...>::rootSteps(std::intptr_t iPtrNum) const
    {
        return DExtT(mkDExt(mkExtT(mOp.rootSteps(iPtrNum))),None(0));
    }
    
    template <typename T, class Operation, class... Ranges>
    DynamicExpression DynamicOuterOp<T,Operation,Ranges...>::loop(const DynamicExpression& exp) const
    {
        return mOp.loop(exp);
    }
    
    template <typename T, class Operation, class... Ranges>
    const OpH<ConstOperationRoot<T,Ranges...>>* DynamicOuterOp<T,Operation,Ranges...>::data() const
    {
        return &mProto;
    }
    
    template <typename T, class Operation, class... Ranges>
    std::shared_ptr<DynamicOperationBase<OpH<ConstOperationRoot<T,Ranges...>>>>
    DynamicOuterOp<T,Operation,Ranges...>::deepCopy() const
    {
        return std::make_shared<DynamicOuterOp<T,Operation,Ranges...>>(*this);
    }

} // namespace MultiArrayTools
