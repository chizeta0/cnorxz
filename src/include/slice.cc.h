
#include "slice.h"

namespace MultiArrayTools
{

    /*******************
     *   ConstSlice    *
     *******************/

    template <typename T, class... SRanges>
    void ConstSlice<T,SRanges...>::format(const std::array<size_t,sizeof...(SRanges)+1>& blocks)
    {
	MAB::mProtoI->format(blocks);
    }

    template <typename T, class... SRanges>
    ConstSlice<T,SRanges...>::ConstSlice(const std::tuple<std::shared_ptr<SRanges>...>& ranges,
					 const T* data) :
	MultiArrayBase<T,SRanges...>(ranges),
	mData(data)
    {
	MAB::mInit = true;
    }
    
    template <typename T, class... SRanges>
    ConstSlice<T,SRanges...>::ConstSlice(const std::shared_ptr<SRanges>&... ranges, const T* data) :
	MultiArrayBase<T,SRanges...>(ranges...),
	mData(data)
    {
	MAB::mInit = true;
    }

    template <typename T, class... SRanges>
    ConstSlice<T,SRanges...>::ConstSlice(const MultiArrayBase<T,AnonymousRange>& ma, SIZET<SRanges>... sizes) :
	MultiArrayBase<T,SRanges...>
	( ma.range()->template get<0>().template scast<SRanges...>(sizes...)->space() ),
	mData( ma.data() )
    {
	MAB::mInit = true;
    }
    
    template <typename T, class... SRanges>
    const T& ConstSlice<T,SRanges...>::operator[](const IType& i) const
    {
	//assert(i.sliceMode()); // -> compare objects !!!!!
	assert(i.container() == reinterpret_cast<std::intptr_t>(this));
	return mData[ i.pos() ];
    }
    
    template <typename T, class... SRanges>
    const T& ConstSlice<T,SRanges...>::at(const typename IType::MetaType& meta) const
    {
	//auto x = begin().at(meta);
	//VCHECK(x.pos());
	return mData[ begin().at(meta).pos() ];
    }
    
    template <typename T, class... SRanges>
    const T* ConstSlice<T,SRanges...>::data() const
    {
	return mData;
    }

    template <typename T, class... SRanges>
    bool ConstSlice<T,SRanges...>::isSlice() const
    {
	return true;
    }
    
    template <typename T, class... SRanges>
    auto ConstSlice<T,SRanges...>::begin() const -> ConstSlice<T,SRanges...>::IType 
    {
	IType i(*MAB::mProtoI,true);
	i = 0;
	//i = mStartPos;
	return i.setData(data());
    }

    template <typename T, class... SRanges>
    auto ConstSlice<T,SRanges...>::end() const -> ConstSlice<T,SRanges...>::IType 
    {
	IType i(*MAB::mProtoI,true);
	i = i.max(); // CHECK !!!
	//i = std::get<sizeof...(SRanges)>(mBlockSizes);
	return i.setData(data());
    }
    
    template <typename T, class... SRanges>
    std::shared_ptr<MultiArrayBase<T,AnonymousRange> > ConstSlice<T,SRanges...>::anonymous(bool slice) const
    {
	assert(0); // think about carefully!!!!
	return nullptr;
    }
    
    template <typename T, class... SRanges>
    auto ConstSlice<T,SRanges...>::define(const std::shared_ptr<typename SRanges::IndexType>&... inds)
	-> ConstSliceDef<T,SRanges...>
    {
	return ConstSliceDef<T,SRanges...>(*this, inds...);
    }

    
    /**************
     *   Slice    *
     **************/

    template <typename T, class... SRanges>
    void Slice<T,SRanges...>::format(const std::array<size_t,sizeof...(SRanges)+1>& blocks)
    {
	MAB::mProtoI->format(blocks);
    }
    
    template <typename T, class... SRanges>
    Slice<T,SRanges...>::Slice(const std::shared_ptr<SRanges>&... ranges, T* data) :
	MutableMultiArrayBase<T,SRanges...>(ranges...),
	mData(data) {}
    
    template <typename T, class... SRanges>
    const T& Slice<T,SRanges...>::operator[](const IType& i) const
    {
	//assert(i.sliceMode()); // -> compare objects !!!!!
	assert(i.container() == reinterpret_cast<std::intptr_t>(this));
	return mData[ i.pos() ];
    }
    
    template <typename T, class... SRanges>
    T& Slice<T,SRanges...>::operator[](const IType& i)
    {
	//assert(i.sliceMode());
	assert(i.container() == reinterpret_cast<std::intptr_t>(this));
	return mData[ i.pos() ];
    }
    
    template <typename T, class... SRanges>
    const T& Slice<T,SRanges...>::at(const typename IType::MetaType& meta) const
    {
	//auto x = begin().at(meta);
	//VCHECK(x.pos());
	return mData[ begin().at(meta).pos() ];
    }
    
    template <typename T, class... SRanges>
    T& Slice<T,SRanges...>::at(const typename IType::MetaType& meta)
    {
	//auto x = begin().at(meta);
	//VCHECK(x.pos());
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
	IType i(*MAB::mProtoI,true);
	i = 0;
	//i = mStartPos;
	return i.setData(data());
    }

    template <typename T, class... SRanges>
    auto Slice<T,SRanges...>::end() const -> Slice<T,SRanges...>::IType 
    {
	IType i(*MAB::mProtoI,true);
	i = i.max(); // CHECK !!!
	//i = std::get<sizeof...(SRanges)>(mBlockSizes);
	return i.setData(data());
    }

    template <typename T, class... SRanges>
    std::shared_ptr<MultiArrayBase<T,AnonymousRange> > Slice<T,SRanges...>::anonymous(bool slice) const
    {
	assert(0); // think about carefully!!!!
	return nullptr;
    }
    
    template <typename T, class... SRanges>
    std::shared_ptr<MultiArrayBase<T,AnonymousRange> > Slice<T,SRanges...>::anonymousMove()
    {
	assert(0); // think about carefully!!!!
	return nullptr;
    }
    
    template <typename T, class... SRanges>
    auto Slice<T,SRanges...>::define(const std::shared_ptr<typename SRanges::IndexType>&... inds)
	-> SliceDef<T,SRanges...>
    {
	return SliceDef<T,SRanges...>(*this, inds...);
    }
    

    template <typename T, class... SRanges>
    SliceDef<T,SRanges...>::SliceDef(Slice<T,SRanges...>& sl,
				     const std::shared_ptr<typename SRanges::IndexType>&... inds) :
	mIndex(sl.begin()),
	mSl(sl)
    {
	mIndex(inds...);
    }

    template <typename T, class... SRanges>
    template <class... ORanges>
    SliceDef<T,SRanges...>& SliceDef<T,SRanges...>::operator=(const OperationRoot<T,ORanges...>& op)
    {
	std::array<size_t,sizeof...(SRanges)+1> blocks;
	PackNum<sizeof...(SRanges)-1>::
	    template mkSliceBlocks<T,OperationRoot<T,ORanges...>,SRanges...>(blocks, mIndex, op);
	mSl.format(blocks);
	mSl.mData = op.data();
	return *this;
    }

    template <typename T, class... SRanges>
    ConstSliceDef<T,SRanges...>::ConstSliceDef(ConstSlice<T,SRanges...>& sl,
					       const std::shared_ptr<typename SRanges::IndexType>&... inds) :
	mIndex(sl.begin()),
	mSl(sl)
    {
	mIndex(inds...);
    }

    template <typename T, class... SRanges>
    template <class... ORanges>
    ConstSliceDef<T,SRanges...>& ConstSliceDef<T,SRanges...>::operator=(const ConstOperationRoot<T,ORanges...>& op)
    {
	std::array<size_t,sizeof...(SRanges)+1> blocks;
	PackNum<sizeof...(SRanges)-1>::
	    template mkSliceBlocks<T,ConstOperationRoot<T,ORanges...>,SRanges...>(blocks, mIndex, op);
	mSl.format(blocks);
	mSl.mData = op.data();
	return *this;
    }

    template <typename T, class... SRanges>
    template <class... ORanges>
    ConstSliceDef<T,SRanges...>& ConstSliceDef<T,SRanges...>::operator=(const OperationRoot<T,ORanges...>& op)
    {
	std::array<size_t,sizeof...(SRanges)+1> blocks;
	PackNum<sizeof...(SRanges)-1>::
	    template mkSliceBlocks<T,OperationRoot<T,ORanges...>,SRanges...>(blocks, mIndex, op);
	mSl.format(blocks);
	mSl.mData = op.data();
	return *this;
    }


} // end namespace MultiArrayTools

