
#ifndef __slice_h__
#define __slice_h__

#include "multi_array_base.h"

namespace MultiArrayTools
{

    template <typename T, class... SRanges>
    class Slice : public MutableMultiArrayBase<T,SRanges...>
    {
    public:

	typedef ContainerRange<T,SRanges...> CRange;
	typedef MultiArrayBase<T,SRanges...> MAB;
	typedef typename CRange::IndexType IType;
	
	DEFAULT_MEMBERS(Slice);

	// RITypes = Range XOR Index depending on whether const XOR opern
	template <class... RITypes>
	Slice(MutableMultiArrayBase<T,typename RITypes::RangeType...>& ma,
	      const std::shared_ptr<RITypes>&... ris);
	
	virtual const T& operator[](const IType& i) const override;
	virtual T& operator[](const IType& i) override;
	virtual const T& at(const typename IType::MetaType& meta) const override;
	virtual T& at(const typename IType::MetaType& meta) override;

	virtual const T* data() const override;
	virtual T* data() override;

	virtual bool isSlice() const override;

	virtual auto begin() const -> IType override;
	virtual auto end() const -> IType override;
	
    private:
	T* mData;
    };
    
} // end namespace MultiArrayTools

/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */

namespace MultiArrayTools
{

    namespace
    {

	//size_t sum(size_t arg)
	//{
	//    return arg;
	//}
	
	template <typename... SizeTypes>
	size_t sum(size_t arg, SizeTypes... args)
	{
	    return arg + sum(args...);
	}
	
	template <bool ISINDEX>
	struct XX
	{
	    template <class RI>
	    static auto ri_to_tuple(const std::shared_ptr<RI>& ri)
		-> std::tuple<RI>
	    {
		return std::make_tuple(ri);
	    }

	    template <class RI>
	    static size_t ri_to_start_pos(const std::shared_ptr<RI>& ri)
	    {
		return 0;
	    }
	};

	template <>
	struct XX<true>
	{
	    template <class RI>
	    static auto ri_to_tuple(const std::shared_ptr<RI>& ri)
		-> std::tuple<>
	    {
		return std::make_tuple();
	    }

	    template <class RI>
	    static size_t ri_to_start_pos(const std::shared_ptr<RI>& ri)
	    {
		return ri->pos();
	    }

	};
	
	template <class... RITypes>
	auto mkSliceRange(const std::shared_ptr<RITypes>&... ris)
	    -> decltype(std::tuple_cat(XX<RITypes::ISINDEX>::ri_to_tuple(ris)...))
	{
	    return std::tuple_cat(XX<RITypes::ISINDEX>::ri_to_tuple(ris)...);
	}

	template <class... RITypes>
	size_t mkStartPos(const std::shared_ptr<RITypes>&... ris)
	{
	    return sum(ri_to_start_pos(ris)...);
	}
	
    }
    
    /*************
     *  Slice    *	     
     *************/
    
    template <typename T, class... SRanges>
    template <class... RITypes>
    Slice<T,SRanges...>::Slice(MutableMultiArrayBase<T,typename RITypes::RangeType...>& ma,
			       const std::shared_ptr<RITypes>&... ris) :
	MutableMultiArrayBase<T,SRanges...>( mkSliceRange(ris...) ),
	mData(ma.data() + mkStartPos(ris...))
    {
	MAB::mProtoI.format( mBlockSizes(mkSliceBlocks(ris...)) );
    }
    
    template <typename T, class... SRanges>
    const T& Slice<T,SRanges...>::operator[](const IType& i) const
    {
	assert(i.sliceMode()); // -> compare objects !!!!!
	assert(i.container() == reinterpret_cast<std::intptr_t>(this));
	return mData[ i.pos() ];
    }

    template <typename T, class... SRanges>
    T& Slice<T,SRanges...>::operator[](const IType& i)
    {
	assert(i.sliceMode());
	assert(i.container() == reinterpret_cast<std::intptr_t>(this));
	return mData[ i.pos() ];
    }

    template <typename T, class... SRanges>
    const T& Slice<T,SRanges...>::at(const typename IType::MetaType& meta) const
    {
	return mData[ begin().at(meta).pos() ];
    }

    template <typename T, class... SRanges>
    T& Slice<T,SRanges...>::at(const typename IType::MetaType& meta)
    {
	return mData[ begin().at(meta).pos() ];
    }

    template <typename T, class... SRanges>
    const T* Slice<T,SRanges...>::data() const
    {
	return mData;
    }

    template <typename T, class... SRanges>
    T* Slice<T,SRanges...>::data()
    {
	return mData;
    }

    template <typename T, class... SRanges>
    bool Slice<T,SRanges...>::isSlice() const
    {
	return true;
    }

    template <typename T, class... SRanges>
    auto Slice<T,SRanges...>::begin() const -> Slice<T,SRanges...>::IType 
    {
	IType i = MAB::mProtoI;
	i = 0;
	//i = mStartPos;
	return i.setData(data());
    }

    template <typename T, class... SRanges>
    auto Slice<T,SRanges...>::end() const -> Slice<T,SRanges...>::IType 
    {
	IType i = MAB::mProtoI;
	i = i.max(); // CHECK !!!
	//i = std::get<sizeof...(SRanges)>(mBlockSizes);
	return i.setData(data());
    }

    

} // end namespace MultiArrayTools

#endif
