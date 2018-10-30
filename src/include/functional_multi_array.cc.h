
#include "functional_multi_array.h"

namespace MultiArrayTools
{


    /****************************
     *   FunctionalMultiArray   *	     
     ****************************/

    /*
    template <typename T, class Range, class Function>
    FunctionalMultiArray<T,Range,Function>::FunctionalMultiArray(const Range& range) :
	MultiArrayBase<T,SRanges...>(range), mFunc() {}
    */
    template <bool FISSTATIC>
    struct Application
    {
	template <typename T, class Function, typename Meta>
	static inline T apply(const std::shared_ptr<Function>& f, const Meta& m)
	{
	    return (*f)(m);
	}
    };

    template <>
    struct Application<true>
    {
	template <typename T, class Function, typename Meta>
	static inline T apply(const std::shared_ptr<Function>& f, const Meta& m)
	{
	    return Function::apply(m);
	}
    };
	
    template <typename T, class Function, class... SRanges>
    FunctionalMultiArray<T,Function,SRanges...>::FunctionalMultiArray(const std::shared_ptr<SRanges>&... ranges,
								      const std::shared_ptr<Function>& func) :
	MultiArrayBase<T,SRanges...>(ranges...), mFunc(func) {}

    template <typename T, class Function, class... SRanges>
    FunctionalMultiArray<T,Function,SRanges...>::FunctionalMultiArray(const std::shared_ptr<SRanges>&... ranges) :
	MultiArrayBase<T,SRanges...>(ranges...) {}

    template <typename T, class Function, class... SRanges>
    FunctionalMultiArray<T,Function,SRanges...>::FunctionalMultiArray(const typename CRange::Space& space) :
	MultiArrayBase<T,SRanges...>(space) {}

    template <typename T, class Function, class... SRanges>
    FunctionalMultiArray<T,Function,SRanges...>::FunctionalMultiArray(const typename CRange::Space& space,
								      const std::shared_ptr<Function>& func) :
	MultiArrayBase<T,SRanges...>(space), mFunc(func) {}

    
    template <typename T, class Function, class... SRanges>
    const T& FunctionalMultiArray<T,Function,SRanges...>::operator[](const IndexType& i) const
    {
	mVal = Application<Function::FISSTATIC>::template apply<T,Function,typename IndexType::MetaType>(mFunc, i.meta());
	return mVal;
    }

    template <typename T, class Function, class... SRanges>
    const T& FunctionalMultiArray<T,Function,SRanges...>::at(const typename CRange::IndexType::MetaType& meta) const
    {
	mVal = Application<Function::FISSTATIC>::template apply<T,Function,typename IndexType::MetaType>(mFunc,meta);
	return mVal;
    }

    template <typename T, class Function, class... SRanges>
    const T* FunctionalMultiArray<T,Function,SRanges...>::data() const
    {
	return &mVal;
    }
    
    template <typename T, class Function, class... SRanges>
    bool FunctionalMultiArray<T,Function,SRanges...>::isConst() const
    {
	return true;
    }

    template <typename T, class Function, class... SRanges>
    bool FunctionalMultiArray<T,Function,SRanges...>::isSlice() const
    {
	return false;
    }

    template <typename T, class Function, class... SRanges>
    std::shared_ptr<MultiArrayBase<T,AnonymousRange> > FunctionalMultiArray<T,Function,SRanges...>::anonymous(bool slice) const
    {
	assert(0); // think about it carefully
	return nullptr;
    }
	
    template <typename T, class Function, class... SRanges>
    ConstOperationRoot<T,SRanges...> FunctionalMultiArray<T,Function,SRanges...>::
    operator()(const std::shared_ptr<typename SRanges::IndexType>&... inds) const
    {
	if(not mMaPtr){
	    mMaPtr = std::make_shared<MAType>( MAB::mRange->space() );
	    (*mMaPtr)(inds...) = exec(inds...);
	}
	return ConstOperationRoot<T,SRanges...>( *mMaPtr, inds... );
    }

    /*
    template <typename T, class Function, class... SRanges>
    auto FunctionalMultiArray<T,Function,SRanges...>::
    exec(const std::shared_ptr<typename SRanges::IndexType>&... inds) const
	-> decltype( mkOperation( mFunc, ConstOperationRoot<typename SRanges::IndexType::MetaType,SRanges>( mkMAObject( inds ), inds) ... ) )
    {
 	return mkOperation( mFunc, ConstOperationRoot<typename SRanges::IndexType::MetaType,SRanges>( mkMAObject( inds ), inds ) ... );
    }
    */

    template <typename T, class Function, class... SRanges>
    auto FunctionalMultiArray<T,Function,SRanges...>::
    exec(const std::shared_ptr<typename SRanges::IndexType>&... inds) const
	-> decltype( mkOperation( mFunc, mkOpObject(inds) ... ) )
    {
 	return mkOperation( mFunc, mkOpObject(inds) ... );
    }

} // namespace MultiArrayTools
