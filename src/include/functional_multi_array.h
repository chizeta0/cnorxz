
#ifndef __functional_multi_array__
#define __functional_multi_array__

#include "multi_array_base.h"
#include "slice.h"

namespace MultiArrayTools
{

    template <class Index>
    auto indexToSlice(const std::shared_ptr<Index>& i)
	-> Slice<typename Index::MetaType,typename Index::RangeType>;

    
    template <typename T, class Function, class... SRanges>
    class FunctionalMultiArray : public MultiArrayBase<T,SRanges...>
    {
    public:

	typedef ContainerRange<T,SRanges...> CRange;
	typedef MultiArrayBase<T,CRange> MAB;
	//typedef typename MultiArrayBase<T,CRange>::const_iterator const_iterator;
	typedef ContainerIndex<T,typename SRanges::IndexType...> IndexType;
	//typedef typename CRange::IndexType IndexType;

    private:
	mutable T mVal;
	Function mFunc;

    public:
	
	DEFAULT_MEMBERS(FunctionalMultiArray);
	FunctionalMultiArray(const std::shared_ptr<SRanges>&... ranges, const Function& func);
	FunctionalMultiArray(const std::shared_ptr<SRanges>&... ranges);
	
	virtual const T& operator[](const IndexType& i) const override;
	
	virtual bool isConst() const override;	
	virtual bool isSlice() const override;

	// EVALUTAION CLASS ??!!!!

	virtual auto operator()(std::shared_ptr<typename SRanges::IndexType>&... inds) const
	    -> decltype( mkOperation( mFunc, ConstOperationRoot<T,SRanges>( indexToSlice( inds ), inds) ... ) ) override;
	
    };

  
} // namespace MultiArrayTools

/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */

namespace MultiArrayTools
{
    
    template <class Index>
    auto indexToSlice(const std::shared_ptr<Index>& i)
	-> Slice<typename Index::MetaType, typename Index::RangeType>
    {
	return Slice<typename Index::MetaType, typename Index::RangeType>( i->range(), i->metaPtr() );
    }


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
	template <typename T, class Function, class Index>
	static inline T apply(const Function& f, const Index& i)
	{
	    return f(i.meta());
	}
    };

    template <>
    struct Application<true>
    {
	template <typename T, class Function, class Index>
	static inline T apply(const Function& f, const Index& i)
	{
	    return Function::apply(i.meta());
	}
    };
	
    template <typename T, class Function, class... SRanges>
    FunctionalMultiArray<T,Function,SRanges...>::FunctionalMultiArray(const std::shared_ptr<SRanges>&... ranges,
								      const Function& func) :
	MultiArrayBase<T,SRanges...>(ranges...), mFunc(func) {}

    template <typename T, class Function, class... SRanges>
    FunctionalMultiArray<T,Function,SRanges...>::FunctionalMultiArray(const std::shared_ptr<SRanges>&... ranges) :
	MultiArrayBase<T,SRanges...>(ranges...) {}

    template <typename T, class Function, class... SRanges>
    const T& FunctionalMultiArray<T,Function,SRanges...>::operator[](const IndexType& i) const
    {
	mVal = Application<Function::FISSTATIC>::template apply<T,Function,IndexType>(mFunc, i.meta());
	return mVal;
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
    auto FunctionalMultiArray<T,Function,SRanges...>::
    operator()(std::shared_ptr<typename SRanges::IndexType>&... inds) const
	-> decltype( mkOperation( mFunc, ConstOperationRoot<T,SRanges>( indexToSlice( inds ), inds) ... ) )
    {
 	return mkOperation( mFunc, ConstOperationRoot<T,SRanges>( indexToSlice( inds ), inds ) ... );
    }

    
} // namespace MultiArrayTools

#endif
