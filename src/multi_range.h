// -*- C++ -*-

#ifndef __multi_range_h__
#define __multi_range_h__

#include <cstdlib>
#include <tuple>
#include <memory>

#include "base_def.h"
#include "range_base.h"
#include "index_base.h"

#include "pack_num.h"

namespace MultiArrayTools
{
    namespace
    {
	using namespace MultiArrayHelper;
    }
    
    template <class... Indices>
    class MultiIndex : public IndexInterface<MultiIndex<Indices...>,
					     std::tuple<typename Indices::MetaType...> >
    {
    public:
	
	typedef IndexInterface<MultiIndex<Indices...>,
			       std::tuple<typename Indices::MetaType...> > IB;
	typedef std::tuple<std::shared_ptr<Indices>...> IndexPack;
	typedef std::tuple<typename Indices::MetaType...> MetaType;
	typedef MultiRange<typename Indices::RangeType...> RangeType;
	
    private:
	
	IndexPack mIPack;
	std::array<size_t,sizeof...(Indices)+1> mBlockSizes;

    public:

	MultiIndex() = delete;

	MultiIndex& operator=(size_t pos) { IB::operator=(pos) ; return *this; }
	
	// NO DEFAULT HERE !!!
	// ( have to assign sub-indices (ptr!) correctly )
	//MultiIndex(const MultiIndex& in);
	//MultiIndex& operator=(const MultiIndex& in);
	MultiIndex& operator=(ContainerIndex<Indices...>& ci);

	template <class MRange>
	MultiIndex(const std::shared_ptr<MRange>& range);

	template <size_t DIR>
	MultiIndex& up();

	template <size_t DIR>
	MultiIndex& down();

	template <size_t N>
	auto get() const -> decltype( *std::get<N>( mIPack ) )&;

	template <size_t N>
	auto getPtr() const -> decltype( std::get<N>( mIPack ) )&;
	
	// raplace instances (in contrast to its analogon in ContainerIndex
	// MultiIndices CANNOT be influences be its subindices, so there is
	// NO foreign/external controll)
	// Do NOT share index instances between two or more MultiIndex instances
	MultiIndex& operator()(std::shared_ptr<Indices>&... indices);

	std::shared_ptr<RangeType> range() const { return std::dynamic_pointer_cast<RangeType>( IB::mRangePtr ); }
	
    private:

	friend IB;
	// ==== >>>>> STATIC POLYMORPHISM <<<<< ====
	
	static IndexType S_type(MultiIndex const* i) { return IndexType::MULTI; }

	static MultiIndex& S_ass_op(MultiIndex* i, size_t pos)
	{
	    i->mPos = pos;
	    PackNum<sizeof...(Indices)-1>::setIndexPack(i->mIPack, pos);
	    return *i;
	}

	static MultiIndex& S_pp_op(MultiIndex* i)
	{
	    PackNum<sizeof...(Indices)-1>::pp( i->mIPack );
	    ++i->mPos;
	    return *i;
	}

	static MultiIndex& S_mm_op(MultiIndex* i)
	{
	    PackNum<sizeof...(Indices)-1>::mm( i->mIPack );
	    --i->mPos;
	    return *i;
	}

	static int S_pp(MultiIndex* i, std::intptr_t idxPtrNum)
	{
	    int tmp = PackNum<sizeof...(Indices)-1>::pp(i->mIPack, i->mBlockSizes, idxPtrNum);
	    i->mPos += tmp;
	    return tmp;
	}

	static int S_mm(MultiIndex* i, std::intptr_t idxPtrNum)
	{
	    int tmp = PackNum<sizeof...(Indices)-1>::mm(i->mIPack, i->mBlockSizes, idxPtrNum);
	    i->mPos -= tmp;
	    return tmp;
	}

	static MetaType S_meta(MultiIndex const* i)
	{
	    MetaType metaTuple;
	    PackNum<sizeof...(Indices)-1>::getMetaPos(metaTuple, i->mIPack);
	    return metaTuple;
	}

	static MultiIndex& S_at(MultiIndex* i, const MetaType& metaPos)
	{
	    PackNum<sizeof...(Indices)-1>::setMeta(i->mIPack, metaPos);
	    i->mPos = PackNum<sizeof...(Indices)-1>::makePos(i->mIPack);
	    return *i;
	}
	
	static size_t S_dim(MultiIndex const* i)
	{
	    return sizeof...(Indices);
	}

	static bool S_first(MultiIndex const* i)
	{
	    return i->mPos == 0;
	}

	static bool S_last(MultiIndex const* i)
	{
	    return i->mPos == i->mMax - 1;
	}

	static std::shared_ptr<RangeType> S_range(MultiIndex const* i)
	{
	    return std::dynamic_pointer_cast<RangeType>( i->mRangePtr );
	}

	template <size_t N>
	static auto S_getPtr(MultiIndex const* i) -> decltype( std::get<N>( mIPack ) )&
	{
	    return std::get<N>(i->mIPack);
	}
	
	//const IndexBase& get(size_t n);
	static std::shared_ptr<VIWB> S_getVPtr(MultiIndex const* i, size_t n)
	{
	    if(n >= sizeof...(Indices)){
		assert(0);
		// throw !!
	    }
	    MultiIndex<Indices...> const* t = i;
	    return PackNum<sizeof...(Indices)-1>::getIndexPtr(*t, n);
	}
	
	static size_t S_getStepSize(MultiIndex const* i, size_t n)
	{
	    if(n >= sizeof...(Indices)){
		assert(0);
		// throw !!
	    }
	    return i->mBlockSizes[n+1];
	}

	static std::string S_id(MultiIndex const* i) { return std::string("mul") + std::to_string(i->mId); }
    };

    /*************************
     *   MultiRangeFactory   *
     *************************/

    template <class... Ranges>
    class MultiRangeFactory : public RangeFactoryBase
    {
    public:
	typedef MultiRange<Ranges...> oType;
	
	MultiRangeFactory() = delete;
	MultiRangeFactory(const std::shared_ptr<Ranges>&... rs);
	MultiRangeFactory(const typename MultiRange<Ranges...>::SpaceType& space);
	MultiRangeFactory(const std::shared_ptr<ContainerRange<Ranges...> >& cr);
	
	virtual std::shared_ptr<RangeBase> create() override;
    };
    
    /******************
     *   MultiRange   *
     ******************/
      
    template <class... Ranges>
    class MultiRange : public RangeInterface<MultiIndex<typename Ranges::IndexType...> >
    {
    public:
	typedef RangeBase RB;
	typedef std::tuple<std::shared_ptr<Ranges>...> SpaceType;
	typedef MultiIndex<typename Ranges::IndexType...> IndexType;	
	//typedef typename RangeInterface<MultiIndex<typename Ranges::IndexType...> >::IndexType IndexType;

    protected:
	MultiRange() = delete;
	MultiRange(const MultiRange& in) = delete;
	MultiRange& operator=(const MultiRange& in) = delete;
	
	MultiRange(const std::shared_ptr<Ranges>&... rs);
	MultiRange(const SpaceType& space);
	
	SpaceType mSpace;
	
    public:
	
	static const size_t sdim = sizeof...(Ranges);

	template <size_t N>
	auto get() const -> decltype( *std::get<N>( mSpace ) )&;

	template <size_t N>
	auto getPtr() const -> decltype( std::get<N>( mSpace ) )&;

	virtual size_t dim() const override;
	virtual size_t size() const override;
	
	const SpaceType& space() const;
	
	virtual IndexType begin() const override;
	virtual IndexType end() const override;

	virtual std::shared_ptr<VIWB> index() const override;

	friend MultiRangeFactory<Ranges...>;
	
    };
    
}

/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */

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
	IndexInterface<MultiIndex<Indices...>,std::tuple<typename Indices::MetaType...> >(range, 0)
    {
	PackNum<sizeof...(Indices)-1>::construct(mIPack, *range);
	IB::mPos = PackNum<sizeof...(Indices)-1>::makePos(mIPack);
	std::get<sizeof...(Indices)>(mBlockSizes) = 1;
	PackNum<sizeof...(Indices)-1>::initBlockSizes(mBlockSizes, mIPack); // has one more element!
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
    std::shared_ptr<VIWB> MultiRange<Ranges...>::index() const
    {
	typedef IndexWrapper<IndexType> IW;
	return std::make_shared<IW>
	    ( std::make_shared<IndexType>
	      ( std::dynamic_pointer_cast<MultiRange<Ranges...> >
		( std::shared_ptr<RangeBase>( RB::mThis ) ) ) );
    }
}

#endif
