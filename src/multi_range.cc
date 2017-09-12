
#include "multi_range.h"
#include "pack_num.h"

namespace MultiArrayTools
{

    namespace
    {
	using namespace MultiArrayHelper;
    }

    /******************
     *   MultiIndex   *
     ******************/

    /*
    template <class... Indices>
    MultiIndex<Indices...>::MultiIndex(const MultiIndex<Indices...>& in) :
	IndexInterface<std::tuple<typename Indices::MetaType...> >(in)
    {
	PackNum<sizeof...(Indices)-1>::copy(mIPack, in);
	IB::mPos = PackNum<sizeof...(Indices)-1>::makePos(mIPack);
    }
	
    template <class... Indices>
    MultiIndex<Indices...>& MultiIndex<Indices...>::operator=(const MultiIndex<Indices...>& in)
    {
	IndexI::operator=(in);
	PackNum<sizeof...(Indices)-1>::copy(mIPack, in);
	IB::mPos = PackNum<sizeof...(Indices)-1>::makePos(mIPack);
	return *this;
    }
    */
    template <class... Indices>
    MultiIndex<Indices...>& MultiIndex<Indices...>::operator=(ContainerIndex<Indices...>& ci)
    {	
	PackNum<sizeof...(Indices)-1>::copyInst(mIPack, ci);
	IB::mPos = PackNum<sizeof...(Indices)-1>::makePos(mIPack);
	return *this;
    }
    
    template <class... Indices>
    template <class MRange>
    MultiIndex<Indices...>::MultiIndex(const std::shared_ptr<MRange>& range) :
	IndexInterface<std::tuple<typename Indices::MetaType...> >(range, 0)
    {
	PackNum<sizeof...(Indices)-1>::construct(mIPack, *range);
	IB::mPos = PackNum<sizeof...(Indices)-1>::makePos(mIPack);
	std::get<sizeof...(Indices)>(mBlockSizes) = 1;
	PackNum<sizeof...(Indices)-1>::initBlockSizes(mBlockSizes, mIPack); // has one more element!
    }

    template <class... Indices>
    IndexType MultiIndex<Indices...>::type() const
    {
	return IndexType::MULTI;
    }
    
    template <class... Indices>
    MultiIndex<Indices...>& MultiIndex<Indices...>::operator++()
    {
	PackNum<sizeof...(Indices)-1>::pp( mIPack );
	++IB::mPos;
	return *this;
    }

    template <class... Indices>
    MultiIndex<Indices...>& MultiIndex<Indices...>::operator--()
    {
	PackNum<sizeof...(Indices)-1>::mm( mIPack );
	--IB::mPos;
	return *this;
    }
    
    template <class... Indices>
    MultiIndex<Indices...>& MultiIndex<Indices...>::operator=(size_t pos)
    {
	IB::mPos = pos;
	PackNum<sizeof...(Indices)-1>::setIndexPack(mIPack, pos);
	return *this;
    }

    template <class... Indices>
    int MultiIndex<Indices...>::pp(std::shared_ptr<IndexBase>& idxPtr)
    {
	int tmp = PackNum<sizeof...(Indices)-1>::pp(mIPack, mBlockSizes, idxPtr);
	IB::mPos += tmp;
	return tmp;
    }

    template <class... Indices>
    int MultiIndex<Indices...>::mm(std::shared_ptr<IndexBase>& idxPtr)
    {
	int tmp = PackNum<sizeof...(Indices)-1>::mm(mIPack, mBlockSizes, idxPtr);
	IB::mPos -= tmp;
	return tmp;
    }

    template <class... Indices>
    template <size_t DIR>
    MultiIndex<Indices...>& MultiIndex<Indices...>::up()
    {
	static_assert(DIR < sizeof...(Indices), "DIR exceeds number of sub-indices");
	IB::mPos += PackNum<sizeof...(Indices)-DIR-1>::blockSize( mIPack );
	PackNum<DIR>::pp( mIPack );
	return *this;
    }

    template <class... Indices>
    template <size_t DIR>
    MultiIndex<Indices...>& MultiIndex<Indices...>::down()
    {
	static_assert(DIR < sizeof...(Indices), "DIR exceeds number of sub-indices");
	IB::mPos -= PackNum<sizeof...(Indices)-DIR-1>::blockSize( mIPack );
	PackNum<DIR>::mm( mIPack );
	return *this;
    }
    
    template <class... Indices>
    size_t MultiIndex<Indices...>::dim() const
    {
	return sizeof...(Indices);
    }
    
    template <class... Indices>
    template <size_t N>
    auto MultiIndex<Indices...>::get() const -> decltype( *std::get<N>( mIPack ) )&
    {
	return *std::get<N>(mIPack);
    }

    template <class... Indices>
    template <size_t N>
    auto MultiIndex<Indices...>::getPtr() const -> decltype( std::get<N>( mIPack ) )&
    {
	return std::get<N>(mIPack);
    }
    
    template <class... Indices>
    const IndexBase& MultiIndex<Indices...>::get(size_t n) const
    {
	if(n >= sizeof...(Indices)){
	    assert(0);
	    // throw !!
	}
	MultiIndex<Indices...> const* t = this;
	return PackNum<sizeof...(Indices)-1>::getIndex(*t, n);
    }

    template <class... Indices>
    std::shared_ptr<IndexBase> MultiIndex<Indices...>::getPtr(size_t n) const
    {
	if(n >= sizeof...(Indices)){
	    assert(0);
	    // throw !!
	}
	MultiIndex<Indices...> const* t = this;
	return PackNum<sizeof...(Indices)-1>::getIndexPtr(*t, n);
    }

    template <class... Indices>
    size_t MultiIndex<Indices...>::getStepSize(size_t n) const
    {
	if(n >= sizeof...(Indices)){
	    assert(0);
	    // throw !!
	}
	return mBlockSizes[n+1];
    }
    
    template <class... Indices>
    typename MultiIndex<Indices...>::MetaType MultiIndex<Indices...>::meta() const
    {
        MetaType metaTuple;
	PackNum<sizeof...(Indices)-1>::getMetaPos(metaTuple, mIPack);
	return metaTuple;
    }

    template <class... Indices>
    MultiIndex<Indices...>& MultiIndex<Indices...>::at(const MultiIndex<Indices...>::MetaType& metaPos)
    {
	PackNum<sizeof...(Indices)-1>::setMeta(mIPack, metaPos);
	IB::mPos = PackNum<sizeof...(Indices)-1>::makePos(mIPack);
	return *this;
    }

    template <class... Indices>
    bool MultiIndex<Indices...>::first() const
    {
	return IB::mPos == 0;
    }

    template <class... Indices>
    bool MultiIndex<Indices...>::last() const
    {
	return IB::mPos == IB::mRangePtr->size() - 1;
    }

    template <class... Indices>
    std::shared_ptr<typename MultiIndex<Indices...>::RangeType> MultiIndex<Indices...>::range() const
    {
	return std::dynamic_pointer_cast<RangeType>( IB::mRangePtr );
    }
    /*
    template <class... Indices>
    MultiIndex<Indices...>& MultiIndex<Indices...>::lock(std::shared_ptr<IndexBase>& idx)
    {
	IB::mLocked = (idx.get() == this);
	PackNum<sizeof...(Indices)-1>::lock(mIPack, idx);
	return *this;
	}*/
    
    template <class... Indices>
    MultiIndex<Indices...>& MultiIndex<Indices...>::operator()(std::shared_ptr<Indices>&... indices)
    {
	PackNum<sizeof...(Indices)-1>::swapIndices(mIPack, indices...);
	PackNum<sizeof...(Indices)-1>::setIndexPack(mIPack, IB::mPos);
	return *this;
    }
    
    /*************************
     *   MultiRangeFactory   *
     *************************/

    template <class... Ranges>
    MultiRangeFactory<Ranges...>::MultiRangeFactory(const std::shared_ptr<Ranges>&... rs)
    {
	mProd = std::shared_ptr< MultiRange<Ranges...> >( new MultiRange<Ranges...>( rs... ) );
    }
    
    template <class... Ranges>
    MultiRangeFactory<Ranges...>::MultiRangeFactory(const typename MultiRange<Ranges...>::SpaceType& st)
    {
	mProd = std::shared_ptr< MultiRange<Ranges...> >( new MultiRange<Ranges...>( st ) );
    }

    template <class... Ranges>
    MultiRangeFactory<Ranges...>::MultiRangeFactory(const std::shared_ptr<ContainerRange<Ranges...> >& cr)
    {
	mProd = std::shared_ptr< MultiRange<Ranges...> >( new MultiRange<Ranges...>( cr->space() ) );
    }
    
    template <class... Ranges>
    std::shared_ptr<RangeBase> MultiRangeFactory<Ranges...>::create()
    {
	setSelf();
	return mProd;
    }
    
    /******************
     *   MultiRange   *
     ******************/

    template <class... Ranges>
    MultiRange<Ranges...>::MultiRange(const std::shared_ptr<Ranges>&... rs) : mSpace(std::make_tuple(rs...)) {}

    template <class... Ranges>
    MultiRange<Ranges...>::MultiRange(const SpaceType& space) : mSpace( space ) {}

    template <class... Ranges>
    template <size_t N>
    auto MultiRange<Ranges...>::get() const -> decltype( *std::get<N>( mSpace ) )&
    {
	return *std::get<N>(mSpace);
    }

    template <class... Ranges>
    template <size_t N>
    auto MultiRange<Ranges...>::getPtr() const -> decltype( std::get<N>( mSpace ) )&
    {
	return std::get<N>(mSpace);
    }

    template <class... Ranges>
    size_t MultiRange<Ranges...>::dim() const
    {
	return sdim;
    }
    
    template <class... Ranges>
    size_t MultiRange<Ranges...>::size() const
    {
	return PackNum<sizeof...(Ranges)-1>::getSize(mSpace);
    }

    template <class... Ranges>
    const typename MultiRange<Ranges...>::SpaceType& MultiRange<Ranges...>::space() const
    {
	return mSpace;
    }
    
    template <class... Ranges>
    typename MultiRange<Ranges...>::IndexType MultiRange<Ranges...>::begin() const
    {
	MultiIndex<typename Ranges::IndexType...>
	    i( std::dynamic_pointer_cast<MultiRange<Ranges...> >
	       ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
	i = 0;
	return i;
    }

    template <class... Ranges>
    typename MultiRange<Ranges...>::IndexType MultiRange<Ranges...>::end() const
    {
	MultiIndex<typename Ranges::IndexType...>
	    i( std::dynamic_pointer_cast<MultiRange<Ranges...> >
	       ( std::shared_ptr<RangeBase>( RB::mThis )) );
	i = size();
	return i;
    }

    template <class... Ranges>
    std::shared_ptr<IndexBase> MultiRange<Ranges...>::index() const
    {
	return std::make_shared<MultiIndex<typename Ranges::IndexType...> >
	    ( std::dynamic_pointer_cast<MultiRange<Ranges...> >
	      ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
    }
}
