
#include "functional_array.h"

namespace CNORXZ
{


    /****************************
     *   FunctionalArray   *	     
     ****************************/

    template <typename T, class Function, class... SRanges>
    FunctionalArray<T,Function,SRanges...>::FunctionalArray(const std::shared_ptr<SRanges>&... ranges,
								      const std::shared_ptr<Function>& func) :
	ArrayBase<T,SRanges...>(ranges...), mFunc(func) {}

    template <typename T, class Function, class... SRanges>
    FunctionalArray<T,Function,SRanges...>::FunctionalArray(const std::shared_ptr<SRanges>&... ranges) :
	ArrayBase<T,SRanges...>(ranges...) {}

    template <typename T, class Function, class... SRanges>
    FunctionalArray<T,Function,SRanges...>::FunctionalArray(const typename CRange::Space& space) :
	ArrayBase<T,SRanges...>(space) {}

    template <typename T, class Function, class... SRanges>
    FunctionalArray<T,Function,SRanges...>::FunctionalArray(const typename CRange::Space& space,
								      const std::shared_ptr<Function>& func) :
	ArrayBase<T,SRanges...>(space), mFunc(func) {}

    
    template <typename T, class Function, class... SRanges>
    const T& FunctionalArray<T,Function,SRanges...>::operator[](const IndexType& i) const
    {
	if constexpr(Function::FISSTATIC){
	    mVal = Function::apply(i.meta());
	}
	else {
	    mVal = (*mFunc)(i.meta());
	}
	return mVal;
    }

    template <typename T, class Function, class... SRanges>
    const T& FunctionalArray<T,Function,SRanges...>::at(const typename CRange::IndexType::MetaType& meta) const
    {
	if constexpr(Function::FISSTATIC){
	    mVal = Function::apply(meta);
	}
	else {
	    mVal = (*mFunc)(meta);
	}
	return mVal;
    }

    template <typename T, class Function, class... SRanges>
    const T* FunctionalArray<T,Function,SRanges...>::data() const
    {
	return &mVal;
    }
    
    template <typename T, class Function, class... SRanges>
    bool FunctionalArray<T,Function,SRanges...>::isConst() const
    {
	return true;
    }

    template <typename T, class Function, class... SRanges>
    bool FunctionalArray<T,Function,SRanges...>::isSlice() const
    {
	return false;
    }

    template <typename T, class Function, class... SRanges>
    std::shared_ptr<ArrayBase<T,AnonymousRange> > FunctionalArray<T,Function,SRanges...>::anonymous(bool slice) const
    {
	assert(0); // think about it carefully
	return nullptr;
    }
	
    template <typename T, class Function, class... SRanges>
    ConstOperationRoot<T,SRanges...> FunctionalArray<T,Function,SRanges...>::
    operator()(const std::shared_ptr<typename SRanges::IndexType>&... inds) const
    {
	if(not mMaPtr){
	    mMaPtr = std::make_shared<MAType>( MAB::mRange->space() );
	    (*mMaPtr)(inds...) = exec(inds...);
	}
	return ConstOperationRoot<T,SRanges...>( *mMaPtr, inds... );
    }

    template <typename T, class Function, class... SRanges>
    auto FunctionalArray<T,Function,SRanges...>::
    exec(const std::shared_ptr<typename SRanges::IndexType>&... inds) const
	-> Operation<T,Function,MetaOperationRoot<SRanges>...>
    {
        return mkOperation( mFunc, MetaOperationRoot<SRanges>( inds ) ... );
    }
    
} // namespace CNORXZ
