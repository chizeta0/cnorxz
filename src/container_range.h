// -*- C++ -*-

#ifndef __container_range_h__
#define __container_range_h__

#include <cstdlib>
#include <tuple>
#include <memory>

#include "base_def.h"
#include "range_base.h"
#include "index_base.h"

#include "pack_num.h"

namespace MultiArrayTools
{
    
    template <class... Indices>
    class ContainerIndex : public IndexInterface<ContainerIndex<Indices...>,
						 std::tuple<typename Indices::MetaType...> >
    {
    public:

	typedef IndexInterface<ContainerIndex<Indices...>,
			       std::tuple<typename Indices::MetaType...> > IB;
	typedef std::tuple<typename Indices::MetaType...>  MetaType;
	typedef std::tuple<std::shared_ptr<Indices>...> IndexPack;
	typedef ContainerRange<typename Indices::RangeType...> RangeType;

    private:

	friend IB;
	
	bool mExternControl = false;
	IndexPack mIPack;
	std::array<size_t,sizeof...(Indices)+1> mBlockSizes; 
	
    public:
	ContainerIndex() = delete;
	
	template <class MRange>
	ContainerIndex(const std::shared_ptr<MRange>& range);

	ContainerIndex& operator=(size_t pos) { IB::operator=(pos) ; return *this; }
	
	template <size_t N>
	auto get() const -> decltype( *std::get<N>( mIPack ) )&;

	template <size_t N>
	auto getPtr() const -> decltype( std::get<N>( mIPack ) )&;

	ContainerIndex& sync(); // recalculate 'IB::mPos' when externalControl == true
	ContainerIndex& operator()(const std::shared_ptr<Indices>&... inds); // control via external indices
	ContainerIndex& operator()(); // -> sync; just to shorten the code
	
    private:

	//ContainerIndex(const ContainerIndex& in);
	//ContainerIndex& operator=(const ContainerIndex& in);

	// ==== >>>>> STATIC POLYMORPHISM <<<<< ====
	
	static IndexType S_type(ContainerIndex const* i) { return IndexType::CONT; }
	
	static ContainerIndex& S_pp_op(ContainerIndex* i)
	{
	    if(i->mExternControl){
		i->mPos = PackNum<sizeof...(Indices)-1>::makePos(i->mIPack);
	    }
	    PackNum<sizeof...(Indices)-1>::pp( i->mIPack );
	    ++i->mPos;
	    return *i;
	}
	
	static ContainerIndex& S_mm_op(ContainerIndex* i)
	{
	    if(i->mExternControl){
		i->mPos = PackNum<sizeof...(Indices)-1>::makePos(i->mIPack);
	    }
	    PackNum<sizeof...(Indices)-1>::mm( i->mIPack );
	    --i->mPos;
	    return *i;

	}

	static ContainerIndex& S_ass_op(ContainerIndex* i, size_t pos)
	{
	    i->mPos = pos;
	    PackNum<sizeof...(Indices)-1>::setIndexPack(i->mIPack, pos);
	    return *i;
	}

	static int S_pp(ContainerIndex* i, intptr_t idxPtrNum)
	{
	    int tmp = PackNum<sizeof...(Indices)-1>::pp(i->mIPack, i->mBlockSizes, idxPtrNum);
	    i->mPos += tmp;
	    return tmp;
	}

	static int S_mm(ContainerIndex* i, intptr_t idxPtrNum)
	{
	    int tmp = PackNum<sizeof...(Indices)-1>::mm(i->mIPack, i->mBlockSizes, idxPtrNum);
	    i->mPos -= tmp;
	    return tmp;
	}
	
	static MetaType S_meta(ContainerIndex const* i)
	{
	    MetaType metaTuple;
	    PackNum<sizeof...(Indices)-1>::getMetaPos(metaTuple, i->mIPack);
	    return metaTuple;
	}

	static ContainerIndex& S_at(ContainerIndex* i, const MetaType& metaPos)
	{
	    PackNum<sizeof...(Indices)-1>::setMeta(i->mIPack, metaPos);
	    i->mPos = PackNum<sizeof...(Indices)-1>::makePos(i->mIPack);
	    return *i;
	}

	static size_t S_dim(ContainerIndex const* i)
	{
	    return sizeof...(Indices);
	}

	static bool S_first(ContainerIndex const* i)
	{
	    return i->pos() == 0;
	}

	static bool S_last(ContainerIndex const* i)
	{
	    return i->pos() == i->mMax - 1;
	}

	static std::shared_ptr<RangeType> S_range(ContainerIndex const* i)
	{
	    return std::dynamic_pointer_cast<RangeType>( i->mRangePtr );
	}
	
	template <size_t N>
	static auto S_getPtr(ContainerIndex const* i) -> decltype( std::get<N>( mIPack ) )&
	{
	    return std::get<N>( i->mIPack );
	}
	
	static std::shared_ptr<VIWB> S_getVPtr(ContainerIndex const* i, size_t n)
	{
	    if(n >= sizeof...(Indices)){
		assert(0);
		// throw !!
	    }
	    ContainerIndex<Indices...> const* t = i;
	    return PackNum<sizeof...(Indices)-1>::getIndexPtr(*t, n);
	}

	static size_t S_getStepSize(ContainerIndex const* i, size_t n)
	{
	    if(n >= sizeof...(Indices)){
		assert(0);
		// throw !!
	    }
	    return i->mBlockSizes[n+1];
	}
	
	static std::string S_id(ContainerIndex const* i) { return std::string("con") + std::to_string(i->mId); }
    };

    
    template <class... Ranges>
    class ContainerRangeFactory : public RangeFactoryBase
    {
    public:

	typedef ContainerRange<Ranges...> oType;
	
	ContainerRangeFactory() = delete;
	ContainerRangeFactory(const std::shared_ptr<Ranges>&... rs);
	ContainerRangeFactory(const typename ContainerRange<Ranges...>::SpaceType& space);
	
	virtual std::shared_ptr<RangeBase> create() override; 
	
    protected:
	
    };
    
    template <class... Ranges>
    class ContainerRange : public RangeInterface<ContainerIndex<typename Ranges::IndexType...> >
    {
    public:

	typedef RangeBase RB;
	typedef std::tuple<std::shared_ptr<Ranges>...> SpaceType;
	typedef ContainerIndex<typename Ranges::IndexType...> IndexType;
	//typedef typename RangeInterface<ContainerIndex<typename Ranges::IndexType...> >::IndexType IndexType;
	
    protected:
	ContainerRange() = default;
	ContainerRange(const ContainerRange& in) = delete;
	ContainerRange& operator=(const ContainerRange& in) = delete;
	
	ContainerRange(const std::shared_ptr<Ranges>&... rs);
	ContainerRange(const SpaceType& space);

	SpaceType mSpace;	

    public:	
	static const size_t sdim = sizeof...(Ranges);

	virtual size_t dim() const override;
	virtual size_t size() const override;

	template <size_t N>
	auto get() const -> decltype( *std::get<N>( mSpace ) )&;

	template <size_t N>
	auto getPtr() const -> decltype( std::get<N>( mSpace ) )&;

	const SpaceType& space() const;
	
	virtual IndexType begin() const override;
	virtual IndexType end() const override;
	
	virtual std::shared_ptr<VIWB> index() const override;

	friend ContainerRangeFactory<Ranges...>;
	
    };
   
} // end namespace MultiArrayTools

/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */

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
    template <class MRange>
    ContainerIndex<Indices...>::ContainerIndex(const std::shared_ptr<MRange>& range) :
	IndexInterface<ContainerIndex<Indices...>,std::tuple<typename Indices::MetaType...> >(range, 0)
    {
	PackNum<sizeof...(Indices)-1>::construct(mIPack, *range);
	IB::mPos = PackNum<sizeof...(Indices)-1>::makePos(mIPack);
	std::get<sizeof...(Indices)>(mBlockSizes) = 1;
	PackNum<sizeof...(Indices)-1>::initBlockSizes(mBlockSizes, mIPack);
    }

    template <class... Indices>
    ContainerIndex<Indices...>& ContainerIndex<Indices...>::sync()
    {
	if(mExternControl){
	    IB::mPos = PackNum<sizeof...(Indices)-1>::makePos(mIPack);
	    //VCHECK(id());
	    //VCHECK(sizeof...(Indices));
	    //assert(IB::mPos < IB::max());
	}
	return *this;
    }
    
    template <class... Indices>
    template <size_t N>
    auto ContainerIndex<Indices...>::get() const -> decltype( *std::get<N>( mIPack ) )&
    {
	return *std::get<N>( mIPack );
    }

    template <class... Indices>
    template <size_t N>
    auto ContainerIndex<Indices...>::getPtr() const -> decltype( std::get<N>( mIPack ) )&
    {
	return std::get<N>( mIPack );
    }

    template <class... Indices>
    ContainerIndex<Indices...>& ContainerIndex<Indices...>::operator()(const std::shared_ptr<Indices>&... inds)
    {
	PackNum<sizeof...(Indices)-1>::swapIndices(mIPack, inds...);
	mExternControl = true;
	return sync();
    }    

    template <class... Indices>
    ContainerIndex<Indices...>& ContainerIndex<Indices...>::operator()()
    {
	return sync();
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
    template <size_t N>
    auto ContainerRange<Ranges...>::get() const -> decltype( *std::get<N>( mSpace ) )&
    {
	return *std::get<N>( mSpace );
    }

    template <class... Ranges>
    template <size_t N>
    auto ContainerRange<Ranges...>::getPtr() const -> decltype( std::get<N>( mSpace ) )&
    {
	return std::get<N>( mSpace );
    }

    template <class... Ranges>
    const typename ContainerRange<Ranges...>::SpaceType& ContainerRange<Ranges...>::space() const
    {
	return mSpace;
    }
        
    template <class... Ranges>
    typename ContainerRange<Ranges...>::IndexType ContainerRange<Ranges...>::begin() const
    {
	ContainerIndex<typename Ranges::IndexType...>
	    i( std::dynamic_pointer_cast<ContainerRange<Ranges...> >
	       ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
	i = 0;
	return i;
    }

    template <class... Ranges>
    typename ContainerRange<Ranges...>::IndexType ContainerRange<Ranges...>::end() const
    {
	ContainerIndex<typename Ranges::IndexType...>
	    i( std::dynamic_pointer_cast<ContainerRange<Ranges...> >
	       ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
	i = size();
	return i;
    }
    
    template <class... Ranges>
    std::shared_ptr<VIWB> ContainerRange<Ranges...>::index() const
    {
	typedef IndexWrapper<IndexType> IW;
	return std::make_shared<IW>
	    ( std::make_shared<IndexType>
	      ( std::dynamic_pointer_cast<ContainerRange<Ranges...> >
		( std::shared_ptr<RangeBase>( RB::mThis ) ) ) );
    }
    
} // end namespace MultiArrayTools


#endif
