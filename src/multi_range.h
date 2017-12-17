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

	// ==== >>>>> STATIC POLYMORPHISM <<<<< ====
	
	IndexType type() { return IndexType::MULTI; }

	MultiIndex& operator=(size_t pos)
	{
	    IB::mPos = pos;
	    PackNum<sizeof...(Indices)-1>::setIndexPack(mIPack, pos);
	    return *this;
	}

	MultiIndex& operator++()
	{
	    PackNum<sizeof...(Indices)-1>::pp( mIPack );
	    ++IB::mPos;
	    return *this;
	}

	MultiIndex& operator--()
	{
	    PackNum<sizeof...(Indices)-1>::mm( mIPack );
	    --IB::mPos;
	    return *this;
	}

	int pp(std::intptr_t idxPtrNum)
	{
	    int tmp = PackNum<sizeof...(Indices)-1>::pp(mIPack, mBlockSizes, idxPtrNum);
	    IB::mPos += tmp;
	    return tmp;
	}

	int mm(std::intptr_t idxPtrNum)
	{
	    int tmp = PackNum<sizeof...(Indices)-1>::mm(mIPack, mBlockSizes, idxPtrNum);
	    IB::mPos -= tmp;
	    return tmp;
	}

	MetaType meta()
	{
	    MetaType metaTuple;
	    PackNum<sizeof...(Indices)-1>::getMetaPos(metaTuple, mIPack);
	    return metaTuple;
	}

	MultiIndex& at(const MetaType& metaPos)
	{
	    PackNum<sizeof...(Indices)-1>::setMeta(mIPack, metaPos);
	    IB::mPos = PackNum<sizeof...(Indices)-1>::makePos(mIPack);
	    return *this;
	}
	
	size_t dim()
	{
	    return sizeof...(Indices);
	}

	bool first()
	{
	    return IB::mPos == 0;
	}

	bool last()
	{
	    return IB::mPos == IB::mMax - 1;
	}

	std::shared_ptr<RangeType> range()
	{
	    return std::dynamic_pointer_cast<RangeType>( IB::mRangePtr );
	}

	template <size_t N>
	auto getPtr() -> decltype( std::get<N>( mIPack ) )&
	{
	    return std::get<N>(mIPack);
	}
	
	//const IndexBase& get(size_t n);
	std::shared_ptr<VIWB> getVPtr(size_t n)
	{
	    if(n >= sizeof...(Indices)){
		assert(0);
		// throw !!
	    }
	    MultiIndex<Indices...> const* t = this;
	    return PackNum<sizeof...(Indices)-1>::getIndexPtr(*t, n);
	}
	
	size_t getStepSize(size_t n)
	{
	    if(n >= sizeof...(Indices)){
		assert(0);
		// throw !!
	    }
	    return mBlockSizes[n+1];
	}

	std::string id() { return std::string("mul") + std::to_string(IB::mId); }

	void print(size_t offset)
	{
	    if(offset == 0){
		std::cout << " === " << std::endl;
	    }
	    for(size_t j = 0; j != offset; ++j) { std::cout << "\t"; }
	    std::cout << id() << "[" << reinterpret_cast<std::intptr_t>(this)
		      << "]" << "(" << IB::mRangePtr << "): " << meta() << std::endl;
	    PackNum<sizeof...(Indices)-1>::printIndex(mIPack, offset+1);
	}
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

	static const bool defaultable = false;
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
