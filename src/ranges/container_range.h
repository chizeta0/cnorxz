// -*- C++ -*-

#ifndef __container_range_h__
#define __container_range_h__

#include <cstdlib>
#include <tuple>
#include <memory>

//#include "base_def.h"
#include "ranges/range_base.h"
#include "ranges/index_base.h"

#include "rpack_num.h"

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

	static IndexType sType() { return IndexType::CONT; }
	static size_t sDim() { return sizeof...(Indices); }
	static size_t totalDim() { return mkTotalDim<Indices...>(); }
	
    private:

	bool mExternControl = false;
	IndexPack mIPack;
	std::array<size_t,sizeof...(Indices)+1> mBlockSizes; 
	
    public:

	ContainerIndex() = delete;
	
	template <class MRange>
	ContainerIndex(const std::shared_ptr<MRange>& range);
	
	template <size_t N>
	auto get() const -> decltype( *std::get<N>( mIPack ) )&;

	template <size_t N>
	auto getPtr() const -> decltype( std::get<N>( mIPack ) )&;

	const IndexPack& pack() const { return mIPack; }
	
	ContainerIndex& sync(); // recalculate 'IB::mPos' when externalControl == true
	ContainerIndex& operator()(const std::shared_ptr<Indices>&... inds); // control via external indices
	ContainerIndex& operator()(); // -> sync; just to shorten the code

	// ==== >>>>> STATIC POLYMORPHISM <<<<< ====
	
	IndexType type() const;
	
	ContainerIndex& operator++();
	ContainerIndex& operator--();
	
	ContainerIndex& operator=(size_t pos);

	int pp(std::intptr_t idxPtrNum);
	int mm(std::intptr_t idxPtrNum);
	
	MetaType meta();
	ContainerIndex& at(const MetaType& metaPos);

	size_t dim();
	bool first();
	bool last();

	std::shared_ptr<RangeType> range();
	
	template <size_t N>
	auto getPtr() -> decltype( std::get<N>( mIPack ) )&;

	std::shared_ptr<VIWB> getVPtr(size_t n);
	size_t getStepSize(size_t n);

	std::vector<IndexInfo> infoVec() const;
	
	std::string id();
	void print(size_t offset);
	
	template <class Exprs>
	auto ifor(Exprs&& exs) const
	    -> decltype(RPackNum<sizeof...(Indices)-1>::mkFor(mIPack, exs));
	
    };

    
    template <class... Ranges>
    class ContainerRangeFactory : public RangeFactoryBase
    {
    public:

	typedef ContainerRange<Ranges...> oType;
	
	ContainerRangeFactory();
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

	static const bool defaultable = false;
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
	RPackNum<sizeof...(Indices)-1>::construct(mIPack, *range);
	IB::mPos = RPackNum<sizeof...(Indices)-1>::makePos(mIPack);
	std::get<sizeof...(Indices)>(mBlockSizes) = 1;
	RPackNum<sizeof...(Indices)-1>::initBlockSizes(mBlockSizes, mIPack);
    }

    template <class... Indices>
    ContainerIndex<Indices...>& ContainerIndex<Indices...>::sync()
    {
	if(mExternControl){
	    IB::mPos = RPackNum<sizeof...(Indices)-1>::makePos(mIPack);
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
	RPackNum<sizeof...(Indices)-1>::swapIndices(mIPack, inds...);
	mExternControl = true;
	return sync();
    }    

    template <class... Indices>
    ContainerIndex<Indices...>& ContainerIndex<Indices...>::operator()()
    {
	return sync();
    }

    template <class... Indices>
    IndexType ContainerIndex<Indices...>::type() const { return IndexType::CONT; }

    template <class... Indices>
    ContainerIndex<Indices...>& ContainerIndex<Indices...>::operator++()
    {
	if(mExternControl){
	    IB::mPos = RPackNum<sizeof...(Indices)-1>::makePos(mIPack);
	}
	RPackNum<sizeof...(Indices)-1>::pp( mIPack );
	++IB::mPos;
	return *this;
    }

    template <class... Indices>
    ContainerIndex<Indices...>& ContainerIndex<Indices...>::operator--()
    {
	if(mExternControl){
	    IB::mPos = RPackNum<sizeof...(Indices)-1>::makePos(mIPack);
	}
	RPackNum<sizeof...(Indices)-1>::mm( mIPack );
	--IB::mPos;
	return *this;

    }

    template <class... Indices>
    ContainerIndex<Indices...>& ContainerIndex<Indices...>::operator=(size_t pos)
    {
	IB::mPos = pos;
	RPackNum<sizeof...(Indices)-1>::setIndexPack(mIPack, pos);
	return *this;
    }

    template <class... Indices>
    int ContainerIndex<Indices...>::pp(std::intptr_t idxPtrNum)
    {
	int tmp = RPackNum<sizeof...(Indices)-1>::pp(mIPack, mBlockSizes, idxPtrNum);
	IB::mPos += tmp;
	return tmp;
    }

    template <class... Indices>
    int ContainerIndex<Indices...>::mm(std::intptr_t idxPtrNum)
    {
	int tmp = RPackNum<sizeof...(Indices)-1>::mm(mIPack, mBlockSizes, idxPtrNum);
	IB::mPos -= tmp;
	return tmp;
    }

    template <class... Indices>
    typename ContainerIndex<Indices...>::MetaType ContainerIndex<Indices...>::meta()
    {
	MetaType metaTuple;
	RPackNum<sizeof...(Indices)-1>::getMetaPos(metaTuple, mIPack);
	return metaTuple;
    }

    template <class... Indices>
    ContainerIndex<Indices...>& ContainerIndex<Indices...>::at(const MetaType& metaPos)
    {
	RPackNum<sizeof...(Indices)-1>::setMeta(mIPack, metaPos);
	IB::mPos = RPackNum<sizeof...(Indices)-1>::makePos(mIPack);
	return *this;
    }

    template <class... Indices>
    size_t ContainerIndex<Indices...>::dim()
    {
	return sizeof...(Indices);
    }

    template <class... Indices>
    bool ContainerIndex<Indices...>::first()
    {
	return IB::pos() == 0;
    }

    template <class... Indices>
    bool ContainerIndex<Indices...>::last()
    {
	return IB::pos() == IB::mMax - 1;
    }

    template <class... Indices>
    std::shared_ptr<typename ContainerIndex<Indices...>::RangeType>
    ContainerIndex<Indices...>::range()
    {
	return std::dynamic_pointer_cast<RangeType>( IB::mRangePtr );
    }

    template <class... Indices>
    template <size_t N>
    auto ContainerIndex<Indices...>::getPtr() -> decltype( std::get<N>( mIPack ) )&
    {
	return std::get<N>( mIPack );
    }

    template <class... Indices>
    std::shared_ptr<VIWB> ContainerIndex<Indices...>::getVPtr(size_t n)
    {
	if(n >= sizeof...(Indices)){
	    assert(0);
	    // throw !!
	}
	ContainerIndex<Indices...> const* t = this;
	return RPackNum<sizeof...(Indices)-1>::getIndexPtr(*t, n);
    }

    template <class... Indices>
    size_t ContainerIndex<Indices...>::getStepSize(size_t n)
    {
	if(n >= sizeof...(Indices)){
	    assert(0);
	    // throw !!
	}
	return mBlockSizes[n+1];
    }

    template <class... Indices>
    std::vector<IndexInfo> ContainerIndex<Indices...>::infoVec() const
    {
	std::vector<IndexInfo> out;
	out.reserve(sizeof...(Indices));
	RPackNum<sizeof...(Indices)-1>::buildInfoVec(out, mIPack, mBlockSizes);
	return std::move( out );
    }
    
    template <class... Indices>
    std::string ContainerIndex<Indices...>::id()
    {
	return std::string("con") + std::to_string(IB::mId);
    }

    template <class... Indices>
    void ContainerIndex<Indices...>::print(size_t offset)
    {
	if(offset == 0){
	    std::cout << " === " << std::endl;
	}
	for(size_t j = 0; j != offset; ++j) { std::cout << "\t"; }
	std::cout << id() << "[" << reinterpret_cast<std::intptr_t>(this) << "]"
		  << "(" << IB::mRangePtr << "): " << meta() << std::endl;
	RPackNum<sizeof...(Indices)-1>::printIndex(mIPack, offset+1);
    }

    template <class... Indices>
    template <class Exprs>
    auto ContainerIndex<Indices...>::ifor(Exprs&& exs) const
	-> decltype(RPackNum<sizeof...(Indices)-1>::mkFor(mIPack, exs))
    {
	return RPackNum<sizeof...(Indices)-1>::mkFor(mIPack, exs);
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
	return RPackNum<sizeof...(Ranges)-1>::getSize(mSpace);
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
