
#include "multi_range.h"
#include "pack_num.h"

namespace MultiArrayTools
{

    namespace
    {
	using namespace MultiArrayHelper;
    }

    /**********************
     *   MultiIndexBase   *	     
     **********************/
    
    template <class... Indices>
    MultiIndex<Indices...>::MultiIndex(const MultiIndex<Indices...>& in) :
	IndexInterface<std::tuple<decltype(Indices().meta())...> >(in)
    {
	PackNum<sizeof...(Indices)-1>::copy(mIPack, in);
	mPos = PackNum<sizeof...(Indices)-1>::makePos(mIPack);
    }
	
    template <class... Indices>
    MultiIndex<Indices...>& MultiIndex<Indices...>::operator=(const MultiIndex<Indices...>& in)
    {
	IndexI::operator=(in);
	PackNum<sizeof...(Indices)-1>::copy(mIPack, in);
	mPos = PackNum<sizeof...(Indices)-1>::makePos(mIPack);
	return *this;
    }
    
    template <class... Indices>
    template <class MRange>
    MultiIndex<Indices...>::MultiIndex(const std::shared_ptr<MRange>& range) :
	IndexInterface<std::tuple<decltype(Indices().meta())...> >(range, 0)
    {
	PackNum<sizeof...(Indices)-1>::construct(mIPack, *range);
	mPos = PackNum<sizeof...(Indices)-1>::makePos(mIPack);
    }
    
    template <class... Indices>
    MultiIndex<Indices...>& MultiIndex<Indices...>::operator++()
    {
	PackNum<sizeof...(Indices)-1>::pp( mIPack );
	++mPos;
	return *this;
    }

    template <class... Indices>
    MultiIndex<Indices...>& MultiIndex<Indices...>::operator--()
    {
	PackNum<sizeof...(Indices)-1>::mm( mIPack );
	--mPos;
	return *this;
    }
    
    template <class... Indices>
    MultiIndex<Indices...>& MultiIndex<Indices...>::operator=(size_t pos)
    {
	mPos = pos;
	PackNum<sizeof...(Indices)-1>::setIndexPack(mIPack, pos);
	return *this;
    }

    template <class... Indices>
    template <size_t DIR>
    MultiIndex<Indices...>& MultiIndex<Indices...>::up()
    {
	static_assert(DIR < sizeof...(Indices), "DIR exceeds number of sub-indices");
	mPos += PackNum<sizeof...(Indices)-DIR-1>::blockSize( mIPack );
	PackNum<DIR>::pp( mIPack );
	return *this;
    }

    template <class... Indices>
    template <size_t DIR>
    MultiIndex<Indices...>& MultiIndex<Indices...>::down()
    {
	static_assert(DIR < sizeof...(Indices), "DIR exceeds number of sub-indices");
	mPos -= PackNum<sizeof...(Indices)-DIR-1>::blockSize( mIPack );
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
    auto MultiIndex<Indices...>::get() const -> const decltype(*std::get<N>(mIPack))&
    {
	return *std::get<N>(mIPack);
    }
    
    template <class... Indices>
    const IndexBase& MultiIndex<Indices...>::get(size_t n) const
    {
	if(n >= sizeof...(Indices)){
	    assert(0);
	    // throw !!
	}
	MultiIndex<Indices...> const* t = this;
	return IndexGetter<sizeof...(Indices)-1>::getIndex(*t, n);
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
	return *this;
    }

    /*************************
     *   MultiRangeFactory   *
     *************************/

    template <class... Ranges>
    MultiRangeFactory<Ranges...>::MultiRangeFactory(const std::shared_ptr<Ranges>&... rs)
    {
	mProd = std::make_shared< MultiRange<Ranges...> >( rs... );
    }
    
    template <class... Ranges>
    MultiRangeFactory<Ranges...>::MultiRangeFactory(const MultiRange<Ranges...>::SpaceType& st)
    {
	mProd = std::make_shared< MultiRange<Ranges...> >( st );
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
    MultiRange<Ranges...>::MultiRange(const SpaceType& space) : mSpace(space) {}

    template <class... Ranges>
    template <size_t N>
    auto getRange() const -> const decltype(*std::get<N>(mSpace))&
    //typename std::tuple_element<N, std::tuple<Ranges...> >::type const& MultiRange<Ranges...>::getRange() const
    {
	return *std::get<N>(mSpace);
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
    MultiIndex<typename Ranges::IndexType...> MultiRange<Ranges...>::begin() const
    {
	MultiIndex<typename Ranges::IndexType...> i( std::dynamic_pointer_cast<MultiRange<Ranges...> >( mThis ) );
	return i = 0;
    }

    template <class... Ranges>
    MultiIndex<typename Ranges::IndexType...> MultiRange<Ranges...>::end() const
    {
	MultiIndex<typename Ranges::IndexType...> i( std::dynamic_pointer_cast<MultiRange<Ranges...> >( mThis ) );
	return i = size();
    }

    template <class... Ranges>
    std::shared_ptr<IndexBase> MultiRange<Ranges...>::index() const
    {
	return std::make_shared<MultiIndex<typename Ranges::IndexType...> >
	    ( std::dynamic_pointer_cast<MultiRange<Ranges...> >( mThis ) );
    }
}
