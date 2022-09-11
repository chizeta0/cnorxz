// -*- C++ -*-

#ifndef __cxz_multi_range_h__
#define __cxz_multi_range_h__

#include "base/base.h"
#include <map>

#include "range_base.h"
#include "index_base.h"

#include "range_helper.h"
#include "multi_range_factory_product_map.h"

#include "statics/static_for.h"

namespace CNORXZ
{
    
    template <class... Indices>
    class MultiIndex : public IndexInterface<MultiIndex<Indices...>,
					     Tuple<typename Indices::MetaType...> >
    {
    public:
	
	typedef IndexInterface<MultiIndex<Indices...>,
			       Tuple<typename Indices::MetaType...> > IB;
	typedef Tuple<Sptr<Indices>...> IndexPack;
	typedef Tuple<typename Indices::MetaType...> MetaType;
	typedef MultiRange<typename Indices::RangeType...> RangeType;
	typedef MultiIndex IType;

	static constexpr IndexType sType() { return IndexType::MULTI; }
	static constexpr SizeT sDim() { return sizeof...(Indices); }
	static constexpr SizeT totalDim() { return (... * Indices::totalDim()); }

	static constexpr SpaceType STYPE = SpaceType::ANY;
        static constexpr bool PARALLEL = TupleElem<0,Tuple<Indices...>>::type::PARALLEL;
        
    private:
	
	IndexPack mIPack;
	Arr<SizeT,sizeof...(Indices)+1> mBlockSizes;
	MultiIndex() = default;

    public:

	const IndexPack& pack() const { return mIPack; }
	
	// NO DEFAULT HERE !!!
	// ( have to assign sub-indices (ptr!) correctly )

	template <class MRange>
	MultiIndex(const Sptr<MRange>& range);

	template <SizeT DIR>
	MultiIndex& up();

	template <SizeT DIR>
	MultiIndex& down();

	template <SizeT N>
	auto get() const -> decltype( *std::get<N>( mIPack ) )&;

	template <SizeT N>
	auto getPtr() const -> decltype( std::get<N>( mIPack ) )&;

	template <SizeT N>
	SizeT getBlockSize() const { return std::get<N>(mBlockSizes); }
	
	// raplace instances (in contrast to its analogon in ConstContainerIndex
	// MultiIndices CANNOT be influences be its subindices, so there is
	// NO foreign/external controll)
	// Do NOT share index instances between two or more MultiIndex instances
	MultiIndex& operator()(Sptr<Indices>&... indices);
	MultiIndex& operator()(const Tuple<Sptr<Indices>...>& indices);

	// ==== >>>>> STATIC POLYMORPHISM <<<<< ====
	
	IndexType type() const;

	MultiIndex& operator=(SizeT pos);

	MultiIndex& operator++();
	MultiIndex& operator--();

	int pp(PtrId idxPtrNum);
	int mm(PtrId idxPtrNum);

	std::string stringMeta() const;
	MetaType meta() const;
	MultiIndex& at(const MetaType& metaPos);
	
	SizeT dim();
	bool first();
	bool last();
	Sptr<RangeType> range();

	template <SizeT N>
	auto getPtr() -> decltype( std::get<N>( mIPack ) )&;

	SizeT getStepSize(SizeT n);

	template <class Exprs>
	auto ifor(SizeT step, Exprs exs) const;

	template <class Exprs>
	auto iforh(SizeT step, Exprs exs) const;

        template <class Exprs>
	auto pifor(SizeT step, Exprs exs) const;

    };

    /*************************
     *   MultiRangeFactory   *
     *************************/

    // NOT THREAD SAVE
    template <class... Ranges>
    class MultiRangeFactory : public RangeFactoryBase
    {
    public:
	typedef MultiRange<Ranges...> oType;
	
	MultiRangeFactory() = delete;
	MultiRangeFactory(const Sptr<Ranges>&... rs);
	MultiRangeFactory(const typename MultiRange<Ranges...>::Space& space);

	virtual Sptr<RangeBase> create() override;

    private:
	
	Sptr<RangeBase> checkIfCreated(const Tuple<Sptr<Ranges>...>& ptp);
	
    };
    
    /******************
     *   MultiRange   *
     ******************/
    
    template <class... Ranges>
    class MultiRange : public RangeInterface<MultiIndex<typename Ranges::IndexType...> >
    {
    public:
	typedef RangeBase RB;
	typedef Tuple<Sptr<Ranges>...> Space;
	typedef MultiIndex<typename Ranges::IndexType...> IndexType;
        typedef Tuple<typename Ranges::IndexType::MetaType...> MetaType;
	typedef MultiRange RangeType;
	typedef MultiRangeFactory<Ranges...> FType;

    protected:
	MultiRange() = delete;
	MultiRange(const MultiRange& in) = delete;
	MultiRange& operator=(const MultiRange& in) = delete;
	
	MultiRange(const Sptr<Ranges>&... rs);
	MultiRange(const Space& space);
	
	Space mSpace;

    public:
	
	static const SizeT sdim = sizeof...(Ranges);

	template <SizeT N>
	auto get() const -> decltype( *std::get<N>( mSpace ) )&;

	SizeT getMeta(const MetaType& metaPos) const;

	template <SizeT N>
	auto getPtr() const -> decltype( std::get<N>( mSpace ) )&;

	virtual Sptr<RangeBase> sub(SizeT num) const override;
	
	virtual SizeT dim() const final;
	virtual SizeT size() const final;

	virtual SpaceType spaceType() const final;
        virtual DataHeader dataHeader() const final;
        
        virtual Vector<SizeT> typeNum() const final;
        virtual SizeT cmeta(char* target, SizeT pos) const final;
        virtual SizeT cmetaSize() const final;
	virtual std::string stringMeta(SizeT pos) const final;
	virtual Vector<char> data() const final;
	
	const Space& space() const;
	
	virtual IndexType begin() const final;
	virtual IndexType end() const final;

	template <class... ERanges>
	auto cat(const Sptr<MultiRange<ERanges...> >& erange)
	    -> Sptr<MultiRange<Ranges...,ERanges...> >;
	
	friend MultiRangeFactory<Ranges...>;

	static constexpr bool HASMETACONT = false;
	static constexpr bool defaultable = false;
	static constexpr SizeT ISSTATIC = (... & Ranges::ISSTATIC);
	static constexpr SizeT SIZE = (... * Ranges::SIZE);
    };
    
}

/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */

namespace CNORXZ
{

    namespace
    {
	using namespace CNORXZInternal;
    }

    // -> define in range_base.cc
    Sptr<RangeFactoryBase> mkMULTI(const char** dp);
    
    /******************
     *   MultiIndex   *
     ******************/
    
    template <class... Indices>
    template <class MRange>
    MultiIndex<Indices...>::MultiIndex(const Sptr<MRange>& range) :
	IndexInterface<MultiIndex<Indices...>,Tuple<typename Indices::MetaType...> >(range, 0)
    {
	std::get<sizeof...(Indices)>(mBlockSizes) = 1;
	sfor_mn<sizeof...(Indices),0>
	    ( [&](auto i) {
		  auto r = range->template getPtr<i>();
		  std::get<i>(mIPack) = r->beginPtr();
		  *std::get<i>(mIPack) = 0;
		  
		  std::get<i>(mBlockSizes) = sfor_p<i,sizeof...(Indices)>
		      ( [&](auto j) { return std::get<j>(mIPack)->max(); } ,
			[&](auto a, auto b) { return a * b; });
		  return 0;
	      });
	
	IB::mPos = sfor_m<sizeof...(Indices),0>
	    ( [&](auto i) { return std::get<i>(mIPack); },
	      [&](auto a, auto b) {return a->pos() + b*a->max();}, 0 );
    }

    template <class... Indices>
    template <SizeT DIR>
    MultiIndex<Indices...>& MultiIndex<Indices...>::up()
    {
	static_assert(DIR < sizeof...(Indices), "DIR exceeds number of sub-indices");
	IB::mPos += sfor_p<DIR,sizeof...(Indices)>
	    ( [&](auto i) { return std::get<i>(mIPack)->max(); },
	      [&](auto a, auto b) { return a * b; } );
	sfor_m<DIR+1,0>
	    ( [&](auto i) {
		auto& si = *std::get<i>( mIPack );
		if(si.last() and i != 0) { si = 0; return true; }
		else { ++si; return false; }
		return false;
	    } );
	return *this;
    }

    template <class... Indices>
    template <SizeT DIR>
    MultiIndex<Indices...>& MultiIndex<Indices...>::down()
    {
	static_assert(DIR < sizeof...(Indices), "DIR exceeds number of sub-indices");
	IB::mPos -= sfor_p<DIR,sizeof...(Indices)>
	    ( [&](auto i) { return std::get<i>(mIPack)->max(); },
	      [&](auto a, auto b) { return a * b; } );
	sfor_m<DIR+1,0>
	    ( [&](auto i) {
		auto& si = *std::get<i>( mIPack );
		if(si.first() and i != 0) { si = si.max()-1; return true; }
		else { --si; return false; }
		return false;
	    } );
	return *this;
    }
    
    template <class... Indices>
    template <SizeT N>
    auto MultiIndex<Indices...>::get() const -> decltype( *std::get<N>( mIPack ) )&
    {
	return *std::get<N>(mIPack);
    }

    template <class... Indices>
    template <SizeT N>
    auto MultiIndex<Indices...>::getPtr() const -> decltype( std::get<N>( mIPack ) )&
    {
	return std::get<N>(mIPack);
    }

    template <class... Indices>
    MultiIndex<Indices...>& MultiIndex<Indices...>::operator()(Sptr<Indices>&... indices)
    {
	return (*this)(std::make_tuple(indices...));
    }

    template <class... Indices>
    MultiIndex<Indices...>& MultiIndex<Indices...>::operator()(const Tuple<Sptr<Indices>...>& indices)
    {
	sfor_pn<0,sizeof...(Indices)>
	    ( [&](auto i) { std::get<i>(mIPack) = std::get<i>(indices); return 0; } );
	RangeHelper::setIndexPack<sizeof...(Indices)-1>(mIPack, IB::mPos);
	return *this;
    }

    template <class... Indices>
    IndexType MultiIndex<Indices...>::type() const
    {
	return IndexType::MULTI;
    }

    template <class... Indices>
    MultiIndex<Indices...>& MultiIndex<Indices...>::operator=(SizeT pos)
    {
	IB::mPos = pos;
	RangeHelper::setIndexPack<sizeof...(Indices)-1>(mIPack, pos);
	return *this;
    }

    template <class... Indices>
    MultiIndex<Indices...>& MultiIndex<Indices...>::operator++()
    {
	sfor_m<sizeof...(Indices),0>
	    ( [&](auto i) {
		auto& si = *std::get<i>( mIPack );
		if(si.last() and i != 0) { si = 0; return true; }
		else { ++si; return false; }
		return false;
	    } );
	++IB::mPos;
	return *this;
    }

    template <class... Indices>
    MultiIndex<Indices...>& MultiIndex<Indices...>::operator--()
    {
	sfor_m<sizeof...(Indices),0>
	    ( [&](auto i) {
		auto& si = *std::get<i>( mIPack );
		if(si.first() and i != 0) { si = si.max()-1; return true; }
		else { --si; return false; }
		return false;
	    } );
	--IB::mPos;
	return *this;
    }


    template <class... Indices>
    int MultiIndex<Indices...>::pp(PtrId idxPtrNum)
    {
	const int tmp = RangeHelper::ppx<sizeof...(Indices)-1>(mIPack, mBlockSizes, idxPtrNum);
	IB::mPos += tmp;
	return tmp;
    }

    template <class... Indices>
    int MultiIndex<Indices...>::mm(PtrId idxPtrNum)
    {
	const int tmp = RangeHelper::mmx<sizeof...(Indices)-1>(mIPack, mBlockSizes, idxPtrNum);
	IB::mPos -= tmp;
	return tmp;
    }

    template <class... Indices>
    std::string MultiIndex<Indices...>::stringMeta() const
    {
	return std::dynamic_pointer_cast<RangeType>( IB::mRangePtr )->stringMeta(IB::mPos);
    }
    
    template <class... Indices>
    typename MultiIndex<Indices...>::MetaType MultiIndex<Indices...>::meta() const
    {
	MetaType metaTuple;
	sfor_pn<0,sizeof...(Indices)>
	    ( [&](auto i) { std::get<i>(metaTuple) = std::get<i>(mIPack)->meta(); return 0; } );
	return metaTuple;
    }

    template <class... Indices>
    MultiIndex<Indices...>& MultiIndex<Indices...>::at(const MetaType& metaPos)
    {
	sfor_pn<0,sizeof...(Indices)>
	    ( [&](auto i) { std::get<i>(mIPack)->at( std::get<i>(metaPos) ); return 0; } );
	IB::mPos = sfor_m<sizeof...(Indices),0>
	    ( [&](auto i) { return std::get<i>(mIPack); },
	      [&](auto a, auto b) {return a->pos() + b*a->max();}, 0 );
	return *this;
    }

    template <class... Indices>
    SizeT MultiIndex<Indices...>::dim()
    {
	return sizeof...(Indices);
    }

    template <class... Indices>
    bool MultiIndex<Indices...>::first()
    {
	return IB::mPos == 0;
    }

    template <class... Indices>
    bool MultiIndex<Indices...>::last()
    {
	return IB::mPos == IB::mMax - 1;
    }

    template <class... Indices>
    Sptr<typename MultiIndex<Indices...>::RangeType>
    MultiIndex<Indices...>::range()
    {
	return std::dynamic_pointer_cast<RangeType>( IB::mRangePtr );
    }

    template <class... Indices>
    template <SizeT N>
    auto MultiIndex<Indices...>::getPtr() -> decltype( std::get<N>( mIPack ) )&
    {
	return std::get<N>(mIPack);
    }

    template <class... Indices>	
    SizeT MultiIndex<Indices...>::getStepSize(SizeT n)
    {
	if(n >= sizeof...(Indices)){
	    assert(0);
	    // throw !!
	}
	return mBlockSizes[n+1];
    }

    template <class... Indices>
    template <class Exprs>
    auto MultiIndex<Indices...>::ifor(SizeT step, Exprs exs) const
    {
	return RangeHelper::mkFor<0>(step, mIPack, mBlockSizes, exs);
    }

    template <class... Indices>
    template <class Exprs>
    auto MultiIndex<Indices...>::iforh(SizeT step, Exprs exs) const
    {
	return RangeHelper::mkForh<0>(step, mIPack, mBlockSizes, exs);
    }

    template <class... Indices>
    template <class Exprs>
    auto MultiIndex<Indices...>::pifor(SizeT step, Exprs exs) const
    {
	return RangeHelper::mkPFor<0>(step, mIPack, mBlockSizes, exs);
    }

    /*************************
     *   MultiRangeFactory   *
     *************************/

    template <class... Ranges>
    MultiRangeFactory<Ranges...>::MultiRangeFactory(const Sptr<Ranges>&... rs)
    {
	mProd = Sptr< MultiRange<Ranges...> >( new MultiRange<Ranges...>( rs... ) );
    }
    
    template <class... Ranges>
    MultiRangeFactory<Ranges...>::MultiRangeFactory(const typename MultiRange<Ranges...>::Space& st)
    {
	mProd = Sptr< MultiRange<Ranges...> >( new MultiRange<Ranges...>( st ) );
    }

    template <class... Ranges>
    Sptr<RangeBase> MultiRangeFactory<Ranges...>::create()
    {
	mProd = checkIfCreated( std::dynamic_pointer_cast<oType>( mProd )->mSpace );
	setSelf();
	return mProd;
    }

    template <class... Ranges>
    Sptr<RangeBase> MultiRangeFactory<Ranges...>::checkIfCreated(const Tuple<Sptr<Ranges>...>& ptp)
    {
	Sptr<RangeBase> out;
	bool check = false;
	for(auto& x: MultiRangeFactoryProductMap::mAleadyCreated){
	    if(x.second.size() == sizeof...(Ranges)){
		check = sfor_p<0,sizeof...(Ranges)>
		    ( [&](auto i) { return reinterpret_cast<PtrId>( std::get<i>(ptp).get() ) == x.second[i]; },
		      [&](auto a, auto b) { return a and b; } );
		if(check){
		    out = x.first;
		    break;
		}
	    }
	}
	if(not check){
	    Vector<PtrId> pv(sizeof...(Ranges));
	    sfor_pn<0,sizeof...(Ranges)>
		( [&](auto i) { pv[i] = reinterpret_cast<PtrId>( std::get<i>(ptp).get() ); return 0; } );
	    MultiRangeFactoryProductMap::mAleadyCreated[mProd] = pv;
	    out = mProd;
	}
	return out;
    }
    
    /******************
     *   MultiRange   *
     ******************/

    template <class... Ranges>
    MultiRange<Ranges...>::MultiRange(const Sptr<Ranges>&... rs) : mSpace(std::make_tuple(rs...)) {}

    template <class... Ranges>
    MultiRange<Ranges...>::MultiRange(const Space& space) : mSpace( space ) {}

    template <class... Ranges>
    template <SizeT N>
    auto MultiRange<Ranges...>::get() const -> decltype( *std::get<N>( mSpace ) )&
    {
	return *std::get<N>(mSpace);
    }

    template <class... Ranges>
    SizeT MultiRange<Ranges...>::getMeta(const MetaType& metaPos) const
    {
	return RangeHelper::getMeta<sizeof...(Ranges)-1>(mSpace,metaPos);
    }

    template <class... Ranges>
    template <SizeT N>
    auto MultiRange<Ranges...>::getPtr() const -> decltype( std::get<N>( mSpace ) )&
    {
	return std::get<N>(mSpace);
    }

    template <class... Indices>
    Sptr<RangeBase> MultiRange<Indices...>::sub(SizeT num) const
    {
	assert(num < sizeof...(Indices));
	return sforx_p<0,sizeof...(Indices)>
	    ( [&](auto i) { return std::dynamic_pointer_cast<RangeBase>(std::get<i>(mSpace)); },
	      [&](auto i) { return num != i;} );
    }

    template <class... Ranges>
    SizeT MultiRange<Ranges...>::dim() const
    {
	return sdim;
    }
    
    template <class... Ranges>
    SizeT MultiRange<Ranges...>::size() const
    {
	return sfor_p<0,sizeof...(Ranges)>
	    ( [&](auto i) { return std::get<i>(mSpace)->size(); },
	      [&](auto a, auto b) { return a * b; } );
    }

    template <class... Ranges>
    SpaceType MultiRange<Ranges...>::spaceType() const
    {
	return SpaceType::ANY;
    }
    
    template <class... Ranges>
    const typename MultiRange<Ranges...>::Space& MultiRange<Ranges...>::space() const
    {
	return mSpace;
    }

    template <class... Ranges>
    Vector<SizeT> MultiRange<Ranges...>::typeNum() const
    {
        Vector<SizeT> o;
	RangeHelper::getTypeNum<sizeof...(Ranges)-1>(o,mSpace);
        return o;
    }

    template <class... Ranges>
    SizeT MultiRange<Ranges...>::cmeta(char* target, SizeT pos) const
    {
        const SizeT off = cmetaSize();
        MetaType* xtarget = reinterpret_cast<MetaType*>(target);
        return RangeHelper::getCMeta<sizeof...(Ranges)-1>(xtarget,pos,mSpace,off);
    }

    template <class... Ranges>
    SizeT MultiRange<Ranges...>::cmetaSize() const
    {
        return RangeHelper::getCMetaSize<0>(mSpace);
    }

    template <class... Ranges>
    std::string MultiRange<Ranges...>::stringMeta(SizeT pos) const
    {
	auto i = begin();
	i = pos;
	return "[" + RangeHelper::getStringMeta<0>(i) + "]";
    }

    template <class... Ranges>
    Vector<char> MultiRange<Ranges...>::data() const
    {
	DataHeader h = dataHeader();
	Vector<char> out;
	//out.reserve(h.metaSize + sizeof(DataHeader));
	char* hcp = reinterpret_cast<char*>(&h);
	out.insert(out.end(), hcp, hcp + sizeof(DataHeader));
	sfor_pn<0,sizeof...(Ranges)>
	    ( [&](auto i) {
		Vector<char> part = std::get<i>(mSpace)->data();
		out.insert(out.end(), part.begin(), part.end());
		return 0;
	    } );
	return out;
    }

    template <class... Ranges>
    DataHeader MultiRange<Ranges...>::dataHeader() const
    {
	DataHeader h;
	h.spaceType = static_cast<int>( SpaceType::ANY );
	h.metaSize = sizeof...(Ranges);
	h.multiple = 1;
        return h;
    }
    
    template <class... Ranges>
    typename MultiRange<Ranges...>::IndexType MultiRange<Ranges...>::begin() const
    {
	MultiIndex<typename Ranges::IndexType...>
	    i( std::dynamic_pointer_cast<MultiRange<Ranges...> >
	       ( Sptr<RangeBase>( RB::mThis ) ) );
	i = 0;
	return i;
    }

    template <class... Ranges>
    typename MultiRange<Ranges...>::IndexType MultiRange<Ranges...>::end() const
    {
	MultiIndex<typename Ranges::IndexType...>
	    i( std::dynamic_pointer_cast<MultiRange<Ranges...> >
	       ( Sptr<RangeBase>( RB::mThis )) );
	i = size();
	return i;
    }

    template <class... Ranges>
    template <class... ERanges>
    auto MultiRange<Ranges...>::cat(const Sptr<MultiRange<ERanges...> >& erange)
	-> Sptr<MultiRange<Ranges...,ERanges...> >
    {
	auto crange = std::tuple_cat(mSpace, erange->space());
	MultiRangeFactory<Ranges...,ERanges...> rf(crange);
	return std::dynamic_pointer_cast<MultiRange<Ranges...,ERanges...> >(rf.create());
    }

}

#endif
