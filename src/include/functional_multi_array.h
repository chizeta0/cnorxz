
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
            assert(0); // deprecated
	    vector<typename Index::MetaType> vv(i->range()->size());
	    for(Index j = (*i); j.pos() != j.max(); ++j){
		vv[j.pos()] = j.meta();
	    }	    
	    return MultiArray<typename Index::MetaType, typename Index::RangeType>( i->range(), vv );
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
	-> std::shared_ptr<decltype(ToMAObject<Index::RangeType::HASMETACONT>::mk(i))>
    {
	return std::make_shared<decltype(ToMAObject<Index::RangeType::HASMETACONT>::mk(i))>
	    (ToMAObject<Index::RangeType::HASMETACONT>::mk(i));
    }

    template <bool HASMETACONT>
    struct ToOpObject
    {
	template <class Index>
	static auto mk(const std::shared_ptr<Index>& ind)
	    -> ConstOperationRoot<typename Index::MetaType,
				  typename Index::RangeType>
	{
	    return ConstOperationRoot<typename Index::MetaType,
				      typename Index::RangeType>( mkMAObject(ind), ind);
	}
    };

    template <>
    struct ToOpObject<false>
    {
        template <class Index>
	static auto mk(const std::shared_ptr<Index>& ind)
	    -> MetaOperationRoot<typename Index::RangeType>
        {
            return MetaOperationRoot<typename Index::RangeType>( ind );
        }
    };
    
    template <class Index>
    auto mkOpObject(const std::shared_ptr<Index>& i)
	-> decltype(ToOpObject<Index::RangeType::HASMETACONT>::mk(i))
    {
	return ToOpObject<Index::RangeType::HASMETACONT>::mk(i);
    }
    
    template <typename T, class Function, class... SRanges>
    class FunctionalMultiArray : public MultiArrayBase<T,SRanges...>
    {
    public:

	typedef ContainerRange<SRanges...> CRange;
	typedef MultiArrayBase<T,SRanges...> MAB;
	typedef ConstContainerIndex<T,typename SRanges::IndexType...> IndexType;
	typedef MultiArray<T,SRanges...> MAType;
	typedef T value_type;
	
    private:
	mutable T mVal;
	std::shared_ptr<Function> mFunc;

	mutable std::shared_ptr<MAType> mMaPtr;

    public:
	
	DEFAULT_MEMBERS(FunctionalMultiArray);
	FunctionalMultiArray(const std::shared_ptr<SRanges>&... ranges, const std::shared_ptr<Function>& func);
	FunctionalMultiArray(const std::shared_ptr<SRanges>&... ranges);
	FunctionalMultiArray(const typename CRange::Space& space);
	FunctionalMultiArray(const typename CRange::Space& space, const std::shared_ptr<Function>& func);
	
	virtual const T& operator[](const IndexType& i) const override;
	virtual const T& at(const typename CRange::IndexType::MetaType& meta) const override;
	virtual const T* data() const override;
	
	virtual bool isConst() const override;	
	virtual bool isSlice() const override;

	virtual std::shared_ptr<MultiArrayBase<T,AnonymousRange> > anonymous(bool slice = false) const override;

	auto exec(const std::shared_ptr<typename SRanges::IndexType>&... inds) const
            -> Operation<T,Function,MetaOperationRoot<SRanges>...>;
            
	virtual ConstOperationRoot<T,SRanges...>
	operator()(const std::shared_ptr<typename SRanges::IndexType>&... inds) const override;
	
    };

  
} // namespace MultiArrayTools

/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */


#endif
