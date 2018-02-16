
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
	typedef typename CRange::IndexType IndexType;
	
	DEFAULT_MEMBERS(Slice);

	template <class... RITypes> // Range / Index <-> open / const 
	Slice(T* data, const RITypes&... ris);
	
	virtual const T& operator[](const IndexType& i) const override;
	virtual T& operator[](const IndexType& i) override;
	virtual const T& at(const typename CRange::IndexType::MetaType& meta) const override;
	virtual T& at(const typename CRange::IndexType::MetaType& meta) override;

	virtual const T* data() const override;
	virtual T* data() override;

	virtual bool isSlice() const override;

	virtual IndexType begin() const override;
	virtual IndexType end() const override;
	
    private:
	T* mData;
	size_t mStartPos;
	std::array<size_t,sizeof...(SRange)+1> mBlockSizes;
    };
    
} // end namespace MultiArrayTools

/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */

namespace MultiArrayTools
{

    namespace
    {

	template <bool ISINDEX>
	struct XX
	{
	    template <class RI>
	    static auto ri_to_tuple(const std::shared_ptr<RI>& ri)
		-> std::tuple<RI>
	    {
		return std::make_tuple(ri);
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
	};
	
	template <class... RITypes>
	auto mkSliceRange(const std::shared_ptr<RITypes>&... ris)
	{
	    return std::tuple_cat(XX<RITypes::ISINDEX>::ri_to_tuple(ris)...);
	}

    }
    
    /*************
     *  Slice    *	     
     *************/
    /*
    template <typename T, class... SRanges>
    Slice<T,SRanges...>::Slice(T* data, const RITypes&... ris) :
	MutableMultiArrayBase<T,SRanges...>( mkSliceRange(ris...) ),
	mData(data),
	mStartPos(mkSliceStart(ris...)),
	mBlockSizes(mkSliceBlocks(ris...)) {}
    */ //!!!!!
    template <typename T, class... SRanges>
    const T& Slice<T,SRanges...>::operator[](const IndexType& i) const
    {
	assert(i.sliceMode()); // -> compare objects !!!!!
	return mData[ i.pos() ];
    }

    template <typename T, class... SRanges>
    T& Slice<T,SRanges...>::operator[](const IndexType& i)
    {
	assert(i.sliceMode());
	return mData[ i.pos() ];
    }

    template <typename T, class... SRanges>
    const T& Slice<T,SRanges...>::at(const typename CRange::IndexType::MetaType& meta) const
    {
	assert(i.sliceMode());
	return mData[ begin().at(meta).pos() ];
    }

    template <typename T, class... SRanges>
    T& Slice<T,SRanges...>::at(const typename CRange::IndexType::MetaType& meta)
    {
	assert(i.sliceMode());
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
    IndexType Slice<T,SRanges...>::begin() const
    {
	IndexType i(mRange, mBlockSizes);
	i = mStartPos;
	return i.setData(data());
    }

    template <typename T, class... SRanges>
    IndexType Slice<T,SRanges...>::end() const
    {
	IndexType i(mRange, mBlockSizes);
	i = std::get<sizeof...(SRanges)>(mBlockSizes);
	return i.setData(data());
    }

    

} // end namespace MultiArrayTools

#endif
