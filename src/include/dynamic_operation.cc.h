
#include "dynamic_operation.h"
#include "helper_tools.h"

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
    DynamicOuterOp<T,Operation,Ranges...>::DynamicOuterOp(const DynamicOuterOp& in) :
	mThreadId(omp_get_thread_num()), mOp(in.mOp),
	mIndices(in.mIndices),
	mMa((mThreadId != in.mThreadId) ? std::make_shared<MultiArray<T,Ranges...>>(*in.mMa) : in.mMa),
	mProto((mThreadId != in.mThreadId) ? OperationRoot<T,Ranges...>(*mMa,mIndices) : in.mProto),
	mL((mThreadId != in.mThreadId) ?
	   mkILoop(std::make_tuple(*mProto.mOp,mOp), mIndices,
		   std::make_tuple(mMa),
		   std::make_tuple(mProto.mOp->assign( mOp, mkMIndex(mIndices) )),
		   std::array<size_t,1>({1}), std::array<size_t,1>({0})) :
	   in.mL)
	   
    {*mMa = 0;}

    template <typename T, class Operation, class... Ranges>
    DynamicOuterOp<T,Operation,Ranges...>::DynamicOuterOp(DynamicOuterOp&& in) :
	mThreadId(omp_get_thread_num()), mOp(in.mOp),
	mIndices(in.mIndices),
	mMa((mThreadId != in.mThreadId) ? std::make_shared<MultiArray<T,Ranges...>>(*in.mMa) : in.mMa),
	mProto((mThreadId != in.mThreadId) ? OperationRoot<T,Ranges...>(*mMa,mIndices) : in.mProto),
	mL((mThreadId != in.mThreadId) ?
	   mkILoop(std::make_tuple(*mProto.mOp,mOp), mIndices,
		   std::make_tuple(mMa),
		   std::make_tuple(mProto.mOp->assign( mOp, mkMIndex(mIndices) )),
		   std::array<size_t,1>({1}), std::array<size_t,1>({0})) :
	   in.mL)
	   
    {*mMa = 0;}

    template <typename T, class Operation, class... Ranges>
    DynamicOuterOp<T,Operation,Ranges...>&
    DynamicOuterOp<T,Operation,Ranges...>::operator=(const DynamicOuterOp& in)
    {
	mThreadId = omp_get_thread_num();
	mOp = in.mOp;
	mIndices = in.mIndices;
	if(mThreadId != in.mThreadId){
	    mMa = std::make_shared<MultiArray<T,Ranges...>>(in.mMa);
	    mProto = OperationRoot<T,Ranges...>(*mMa,mIndices);
	    mL = mkILoop(std::make_tuple(*mProto.mOp,mOp), mIndices,
			 std::make_tuple(mMa),
			 std::make_tuple(mProto.mOp->assign( mOp, mkMIndex(mIndices) )),
			 std::array<size_t,1>({1}), std::array<size_t,1>({0}));
	}
	else {
	    mMa = in.mMa;
	    mProto = in.mProto;
	    mL = in.mL;
	}
	*mMa = 0;
	return *this;
    }

    template <typename T, class Operation, class... Ranges>
    DynamicOuterOp<T,Operation,Ranges...>&
    DynamicOuterOp<T,Operation,Ranges...>::operator=(DynamicOuterOp&& in)
    {
	mThreadId = omp_get_thread_num();
	mOp = in.mOp;
	mIndices = in.mIndices;
	if(mThreadId != in.mThreadId){
	    mMa = std::make_shared<MultiArray<T,Ranges...>>(in.mMa);
	    mProto = OperationRoot<T,Ranges...>(*mMa,mIndices);
	    mL = mkILoop(std::make_tuple(*mProto.mOp,mOp), mIndices,
			 std::make_tuple(mMa),
			 std::make_tuple(mProto.mOp->assign( mOp, mkMIndex(mIndices) )),
			 std::array<size_t,1>({1}), std::array<size_t,1>({0}));
	}
	else {
	    mMa = in.mMa;
	    mProto = in.mProto;
	    mL = in.mL;
	}
	*mMa = 0;
	return *this;
    }

    template <typename T, class Operation, class... Ranges>
    DynamicOuterOp<T,Operation,Ranges...>::DynamicOuterOp(const Operation& op,
                                                          const std::shared_ptr<typename Ranges::IndexType>&... inds)
        : mThreadId(omp_get_thread_num()), mOp(op),
	  mIndices(inds...),
	  mMa(std::make_shared<MultiArray<T,Ranges...>>(mkArray<T>(inds->range()...))),
	  mProto(OperationRoot<T,Ranges...>(*mMa,inds...)),
          mL(std::make_tuple(*mProto.mOp,mOp), std::make_tuple(inds...),
             std::make_tuple(mMa), std::make_tuple(mProto.mOp->assign( mOp, mkMIndex(inds...) )),
             std::array<size_t,1>({1}), std::array<size_t,1>({0}))
    {}

    template <typename T, class Operation, class... Ranges>
    OpH<OperationRoot<T,Ranges...>> DynamicOuterOp<T,Operation,Ranges...>::get(const DExtT& pos) const
    {
        mL(0,pos.expl<ET>());
        return mProto; // empty
    }
    
    template <typename T, class Operation, class... Ranges>
    DynamicOperationBase<OpH<OperationRoot<T,Ranges...>>>&
    DynamicOuterOp<T,Operation,Ranges...>::set(const DExtT& pos)
    {
	mOp.set(pos.expl<ET>());
	return *this;
    }
    
    template <typename T, class Operation, class... Ranges>
    DExtT DynamicOuterOp<T,Operation,Ranges...>::rootSteps(std::intptr_t iPtrNum) const
    {
        return DExtT(mkDExt(mkExtT(mL.rootSteps(iPtrNum))),None(0));
    }
    
    template <typename T, class Operation, class... Ranges>
    DynamicExpression DynamicOuterOp<T,Operation,Ranges...>::loop(const DynamicExpression& exp) const
    {
        return mOp.loop(exp); // ???!!
    }
    
    template <typename T, class Operation, class... Ranges>
    const OpH<OperationRoot<T,Ranges...>>* DynamicOuterOp<T,Operation,Ranges...>::data() const
    {
        return &mProto;
    }
    
    template <typename T, class Operation, class... Ranges>
    std::shared_ptr<DynamicOperationBase<OpH<OperationRoot<T,Ranges...>>>>
    DynamicOuterOp<T,Operation,Ranges...>::deepCopy() const
    {
        return std::make_shared<DynamicOuterOp<T,Operation,Ranges...>>(*this);
    }

} // namespace MultiArrayTools
