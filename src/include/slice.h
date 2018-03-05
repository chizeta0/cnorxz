
#ifndef __slice_h__
#define __slice_h__

#include "multi_array_base.h"
#include "multi_array_operation.h"

namespace MultiArrayTools
{
    template <typename T, class... SRanges>
    class Slice : public MutableMultiArrayBase<T,SRanges...>
    {
    public:

	typedef ContainerRange<T,SRanges...> CRange;
	typedef MultiArrayBase<T,SRanges...> MAB;
	typedef ContainerIndex<T,typename SRanges::IndexType...> IType;
	
	DEFAULT_MEMBERS(Slice);

	Slice(T* data, const std::shared_ptr<SRanges>&... ranges);
	    
	virtual const T& operator[](const IType& i) const override;
	virtual T& operator[](const IType& i) override;
	virtual const T& at(const typename IType::MetaType& meta) const override;
	virtual T& at(const typename IType::MetaType& meta) override;

	virtual const T* data() const override;
	virtual T* data() override;

	virtual bool isSlice() const override;

	virtual auto begin() const -> IType override;
	virtual auto end() const -> IType override;

	template <class... MARanges>
	auto define(const std::shared_ptr<typename SRanges::IType>&... inds)
	    -> SliceDef<T,MARanges...>;
	
    private:
	T* mData;
    };


    template <typename T, class... SRanges>
    class SliceDef
    {
    private:
	SliceDef() = default;
	
    public:
	SliceDef(Slice<T,SRanges...>& sl,
		 const std::shared_ptr<typename SRanges::IndexType>&... inds);

	template <class... ORanges>
	SliceDef& operator=(OperationRoot<T,ORanges...>& op);
    };
    
} // end namespace MultiArrayTools

/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */

namespace MultiArrayTools
{
    
    /*************
     *  Slice    *
     *************/

    template <typename T, class... SRanges>
    Slice<T,SRanges...>::Slice(T* data, const std::shared_ptr<SRanges>&... ranges) :
	MutableMultiArrayBase<T,SRanges...>(ranges...),
	mData(data) {}
    
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
    /*
    SliceDef<T,SRanges...>::
    SliceDef(Slice<T,SRanges...>& sl,
	     const std::shared_ptr<typename SRanges::IndexType>&... inds);
    
    template <class... ORanges>
    SliceDef<T,SRanges...>& SliceDef<T,SRanges...>::operator=(OperationRoot<T,ORanges...>& op);
    */

} // end namespace MultiArrayTools

#endif
