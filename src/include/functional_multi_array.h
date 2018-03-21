
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
	virtual const T& at(const typename CRange::IndexType::MetaType& meta) const override;
	virtual const T* data() const override;
	
	virtual bool isConst() const override;	
	virtual bool isSlice() const override;

	// EVALUTAION CLASS ??!!!!

	auto exec(std::shared_ptr<typename SRanges::IndexType>&... inds) const
	    -> decltype( mkOperation( mFunc, ConstOperationRoot<T,SRanges>( indexToSlice( inds ), inds) ... ) );
	
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
	template <typename T, class Function, typename Meta>
	static inline T apply(const Function& f, const Meta& m)
	{
	    return f(m);
	}
    };

    template <>
    struct Application<true>
    {
	template <typename T, class Function, typename Meta>
	static inline T apply(const Function& f, const Meta& m)
	{
	    return Function::apply(m);
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
    auto FunctionalMultiArray<T,Function,SRanges...>::
    exec(std::shared_ptr<typename SRanges::IndexType>&... inds) const
	-> decltype( mkOperation( mFunc, ConstOperationRoot<T,SRanges>( indexToSlice( inds ), inds) ... ) )
    {
 	return mkOperation( mFunc, ConstOperationRoot<T,SRanges>( indexToSlice( inds ), inds ) ... );
    }

    
} // namespace MultiArrayTools

#endif
