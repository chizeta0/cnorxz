// -*- C++ -*-

#include "container_range.h"
#include "pack_num.h"

namespace MultiArrayTools
{
    namespace
    {
	using namespace MultiArrayHelper;
    }

    
    /**********************
     *   ContainerIndex   *
     **********************/

    template <class... Indices>
    ContainerIndex<Indices...>::ContainerIndex(const ContainerIndex& in) :
	IndexInterface<std::tuple<decltype(Indices().meta())...> >(in)
    {
	PackNum<sizeof...(Indices)-1>::copy(mIPack, in);
	IB::mPos = PackNum<sizeof...(Indices)-1>::makePos(mIPack);
    }

    template <class... Indices>
    ContainerIndex<Indices...>& ContainerIndex<Indices...>::operator=(const ContainerIndex& in)
    {
	IndexI::operator=(in);
	PackNum<sizeof...(Indices)-1>::copy(mIPack, in);
	IB::mPos = PackNum<sizeof...(Indices)-1>::makePos(mIPack);
	return *this;
    }

    template <class... Indices>
    template <class MRange>
    ContainerIndex<Indices...>::ContainerIndex(const std::shared_ptr<MRange>& range)
    {
	PackNum<sizeof...(Indices)-1>::construct(mIPack, *range);
	IB::mPos = PackNum<sizeof...(Indices)-1>::makePos(mIPack);
    }

    template <class... Indices>
    ContainerIndex<Indices...>& ContainerIndex<Indices...>::operator++()
    {
	if(mExternControl){
	    IB::mPos = PackNum<sizeof...(Indices)-1>::makePos(mIPack);
	}
	PackNum<sizeof...(Indices)-1>::pp( mIPack );
	++IB::mPos;
	return *this;
    }

    template <class... Indices>
    ContainerIndex<Indices...>& ContainerIndex<Indices...>::operator--()
    {
	if(mExternControl){
	    IB::mPos = PackNum<sizeof...(Indices)-1>::makePos(mIPack);
	}
	PackNum<sizeof...(Indices)-1>::mm( mIPack );
	--IB::mPos;
	return *this;

    }

    template <class... Indices>
    ContainerIndex<Indices...>& ContainerIndex<Indices...>::operator=(size_t pos)
    {
	IB::mPos = pos;
	PackNum<sizeof...(Indices)-1>::setIndexPack(mIPack, pos);
	return *this;
    }

    template <class... Indices>
    typename ContainerIndex<Indices...>::MetaType ContainerIndex<Indices...>::meta() const
    {
	MetaType metaTuple;
	PackNum<sizeof...(Indices)-1>::getMetaPos(metaTuple, mIPack);
	return metaTuple;
    }

    template <class... Indices>
    ContainerIndex<Indices...>& ContainerIndex<Indices...>::at(const MetaType& metaPos)
    {
	PackNum<sizeof...(Indices)-1>::setMeta(mIPack, metaPos);
	return *this;
    }

    template <class... Indices>
    size_t ContainerIndex<Indices...>::dim() const
    {
	return sizeof...(Indices);
    }

    template <class... Indices>
    size_t ContainerIndex<Indices...>::pos() const
    {
	if(mExternControl){
	    IB::mPos = PackNum<sizeof...(Indices)-1>::makePos(mIPack);
	}
	return IB::mPos;
    }

    template <class... Indices>
    bool ContainerIndex<Indices...>::first() const
    {
	return pos() == 0;
    }

    template <class... Indices>
    bool ContainerIndex<Indices...>::last() const
    {
	return pos() == IB::mRangePtr->size() - 1;
    }

    
    template <class... Indices>
    ContainerIndex<Indices...>& ContainerIndex<Indices...>::operator()(const std::shared_ptr<Indices>&... inds)
    {
	PackNum<sizeof...(Indices)-1>::swapIndices(mIPack, inds...);
	mExternControl = true;
	return *this;
    }    
    
    /*****************************
     *   ContainerRangeFactory   *
     *****************************/

    template <class... Ranges>
    ContainerRangeFactory<Ranges...>::ContainerRangeFactory(const std::shared_ptr<Ranges>&... rs)
    {
	mProd = std::shared_ptr<ContainerRange<Ranges...> >( new ContainerRange<Ranges...>( rs... ) );
    }
    
    template <class... Ranges>
    ContainerRangeFactory<Ranges...>::
    ContainerRangeFactory(const typename ContainerRange<Ranges...>::SpaceType& space)
    {
	mProd = std::shared_ptr<ContainerRange<Ranges...> >( new ContainerRange<Ranges...>( space ) );
    }

    template <class... Ranges>
    std::shared_ptr<RangeBase> ContainerRangeFactory<Ranges...>::create()
    {
	setSelf();
	return mProd;
    }
    
    /**********************
     *   ContainerRange   *
     **********************/

    template <class... Ranges>
    ContainerRange<Ranges...>::ContainerRange(const std::shared_ptr<Ranges>&... rs) :
	mSpace( std::make_tuple( rs... ) ) {}

    template <class... Ranges>
    ContainerRange<Ranges...>::ContainerRange(const SpaceType& space) : mSpace( space ) {}

    template <class... Ranges>
    size_t ContainerRange<Ranges...>::dim() const
    {
	return sizeof...(Ranges);
    }

    template <class... Ranges>
    size_t ContainerRange<Ranges...>::size() const
    {
	return PackNum<sizeof...(Ranges)-1>::getSize(mSpace);
    }

    template <class... Ranges>
    typename ContainerRange<Ranges...>::IndexType ContainerRange<Ranges...>::begin() const
    {
	ContainerIndex<typename Ranges::IndexType...>
	    i( std::dynamic_pointer_cast<ContainerRange<Ranges...> >
	       ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
	return i = 0;
    }

    template <class... Ranges>
    typename ContainerRange<Ranges...>::IndexType ContainerRange<Ranges...>::end() const
    {
	ContainerIndex<typename Ranges::IndexType...>
	    i( std::dynamic_pointer_cast<ContainerRange<Ranges...> >
	       ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
	return i = size();
    }

    template <class... Ranges>
    std::shared_ptr<IndexBase> ContainerRange<Ranges...>::index() const
    {
	return std::make_shared<ContainerIndex<typename Ranges::IndexType...> >
	    ( std::dynamic_pointer_cast<ContainerRange<Ranges...> >
	      ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
    }
    
} // end namespace MultiArrayTools
