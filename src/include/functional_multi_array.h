
#ifndef __functional_multi_array__
#define __functional_multi_array__

#include "multi_array_base.h"
#include "slice.h"

namespace MultiArrayTools
{

    template <bool HASMETACONT>
    struct ToMAObject
    {
	template <class Index>
	static auto mk(const std::shared_ptr<Index>& i)
	    -> MultiArray<typename Index::MetaType, typename Index::RangeType>
	{
	    std::vector<typename Index::MetaType> vv(i->range()->size());
	    for(Index j = (*i); j.pos() != j.max(); ++j){
		vv[j.pos()] = j.meta();
	    }
	    return MultiArray<typename Index::MetaType, typename Index::RangeType>( i->range(), std::move( vv ) );
	}
    };

    template <>
    struct ToMAObject<true>
    {
	template <class Index>
	static auto mk(const std::shared_ptr<Index>& i)
	    -> ConstSlice<typename Index::MetaType, typename Index::RangeType>
	{
	    return ConstSlice<typename Index::MetaType, typename Index::RangeType>( i->range(), i->metaPtr() );
	}
    };
    
    template <class Index>
    auto mkMAObject(const std::shared_ptr<Index>& i)
	-> decltype(ToMAObject<Index::RangeType::HASMETACONT>::mk(i))
    {
	return ToMAObject<Index::RangeType::HASMETACONT>::mk(i);
    }

    
    template <typename T, class Function, class... SRanges>
    class FunctionalMultiArray : public MultiArrayBase<T,SRanges...>
    {
    public:

	typedef ContainerRange<T,SRanges...> CRange;
	typedef MultiArrayBase<T,SRanges...> MAB;
	//typedef typename MultiArrayBase<T,CRange>::const_iterator const_iterator;
	typedef ContainerIndex<T,typename SRanges::IndexType...> IndexType;
	//typedef typename CRange::IndexType IndexType;
	typedef MultiArray<T,SRanges...> MAType;
	
    private:
	mutable T mVal;
	Function mFunc;

	mutable std::shared_ptr<MAType> mMaPtr;

    public:
	
	DEFAULT_MEMBERS(FunctionalMultiArray);
	FunctionalMultiArray(const std::shared_ptr<SRanges>&... ranges, const Function& func);
	FunctionalMultiArray(const std::shared_ptr<SRanges>&... ranges);
	
	virtual const T& operator[](const IndexType& i) const override;
	virtual const T& at(const typename CRange::IndexType::MetaType& meta) const override;
	virtual const T* data() const override;
	
	virtual bool isConst() const override;	
	virtual bool isSlice() const override;

	virtual std::shared_ptr<MultiArrayBase<T,AnonymousRange> > anonymous() const override;
	
	// EVALUTAION CLASS ??!!!!

	auto exec(const std::shared_ptr<typename SRanges::IndexType>&... inds) const
	    -> decltype( mkOperation( mFunc, ConstOperationRoot<typename SRanges::IndexType::MetaType,SRanges>( mkMAObject( inds ), inds) ... ) );

	virtual ConstOperationRoot<T,SRanges...>
	operator()(const std::shared_ptr<typename SRanges::IndexType>&... inds) const override;
	
    };

  
} // namespace MultiArrayTools

/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */

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
    std::shared_ptr<MultiArrayBase<T,AnonymousRange> > FunctionalMultiArray<T,Function,SRanges...>::anonymous() const
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

    
    template <typename T, class Function, class... SRanges>
    auto FunctionalMultiArray<T,Function,SRanges...>::
    exec(const std::shared_ptr<typename SRanges::IndexType>&... inds) const
	-> decltype( mkOperation( mFunc, ConstOperationRoot<typename SRanges::IndexType::MetaType,SRanges>( mkMAObject( inds ), inds) ... ) )
    {
 	return mkOperation( mFunc, ConstOperationRoot<typename SRanges::IndexType::MetaType,SRanges>( mkMAObject( inds ), inds ) ... );
    }

    
} // namespace MultiArrayTools

#endif
