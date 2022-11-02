
#ifndef __cxz_mrange_cc_h__
#define __cxz_mrange_cc_h__

#include "mrange.h"
#include "range_helper.h"
#include "statics/static_for.h"

namespace CNORXZ
{
    /***********************
     *   MIndex (private)  *
     ***********************/

    template <class... Indices>
    template <SizeT P, SizeT... Is>
    constexpr decltype(auto) MIndex<Indices...>::indexSequencePlus(std::index_sequence<Is...> is) const
    {
	return std::index_sequence<P+Is...> {};
    }

    template <class... Indices>
    template <SizeT B, SizeT E>
    constexpr decltype(auto) MIndex<Indices...>::mkIndexSequence() const
    {
	return indexSequencePlus<B>(std::make_index_sequence<E-B>{});
    }

    template <class... Indices>
    template <class G, class F, SizeT... Is>
    constexpr decltype(auto) MIndex<Indices...>::accumulatei(const G& g, const F& f, std::index_sequence<Is...> is) const
    {
	return f( g(std::get<Is>(mIPack))... );
    }

    template <class... Indices>
    template <SizeT B, SizeT E, class G, class F>
    constexpr decltype(auto) MIndex<Indices...>::accumulate(const G& g, const F& f) const
    {
	return accumulatei(g, f, mkIndexSequence<B,E>())
    }

    template <class... Indices>
    template <SizeT... Is>
    constexpr decltype(auto) mkIPack(SizeT pos, std::index_sequence<Is...> is) const
    {
	static_assert(sizeof...(Is) == sizeof...(Indices),
		      "sequence size does not match number of indices");
	return std::make_tuple( std::make_shared<Indices>( mRange->sub(Is) )... );
    }
    
    template <class... Indices>
    template <SizeT... Is>
    constexpr decltype(auto) mkBlockSizes(std::index_sequence<Is...> is) const
    {
	static_assert(sizeof...(Is) == sizeof...(Indices)-1,
		      "set lowest block size manually");
	return Arr<SizeT,sizeof...(Indices)>
	    { accumulate<Is,sizeof...(Indices)>( [](const auto& i) { return i->max(); },
						 [](const auto&... as) { return (as * ...); } ),
	      1 };
    }

    /**************
     *   MIndex   *
     **************/

    template <class... Indices>
    template <class MRange>
    MIndex<Indices...>::MIndex(const Sptr<MRange>& range, SizeT pos) :
	IndexInterface<MIndex<Indices...>,Tuple<typename Indices::MetaType...>>(pos),
	mIPack(mkIPack(IB::mPos, std::make_index_sequence<sizeof...(Indices)>{})),
	mBlockSizes(mkBlockSizes(IB::mPos), std::make_index_sequence<sizeof...(Indices)>{}),
	mRange(range)
    {}

    template <class... Indices>
    MIndex<Indices...>& MIndex<Indices...>::operator()(const Sptr<MIndex>& mi)
    {
	sfor_pn<0,sizeof...(Indices)>
	    ( [&](auto i) { std::get<i>(mIPack) = std::get<i>(indices); return 0; } );
	RangeHelper::setIndexPack<sizeof...(Indices)-1>(mIPack, IB::mPos);
	return *this;
    }

    template <class... Indices>
    MIndex<Indices...>& MIndex<Indices...>::operator=(SizeT pos)
    {
	IB::mPos = pos;
	RangeHelper::setIndexPack<sizeof...(Indices)-1>(mIPack, pos);
	return *this;
    }

    template <class... Indices>
    MIndex<Indices...>& MIndex<Indices...>::operator++()
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
    MIndex<Indices...>& MIndex<Indices...>::operator--()
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
    int MIndex<Indices...>::pp(PtrId idxPtrNum)
    {
	const int tmp = RangeHelper::ppx<sizeof...(Indices)-1>(mIPack, mBlockSizes, idxPtrNum);
	IB::mPos += tmp;
	return tmp;
    }

    template <class... Indices>
    int MIndex<Indices...>::mm(PtrId idxPtrNum)
    {
	const int tmp = RangeHelper::mmx<sizeof...(Indices)-1>(mIPack, mBlockSizes, idxPtrNum);
	IB::mPos -= tmp;
	return tmp;
    }

    template <class... Indices>
    String MIndex<Indices...>::stringMeta() const
    {
	return std::dynamic_pointer_cast<RangeType>( IB::mRangePtr )->stringMeta(IB::mPos);
    }
    
    template <class... Indices>
    typename MIndex<Indices...>::MetaType MIndex<Indices...>::meta() const
    {
	MetaType metaTuple;
	sfor_pn<0,sizeof...(Indices)>
	    ( [&](auto i) { std::get<i>(metaTuple) = std::get<i>(mIPack)->meta(); return 0; } );
	return metaTuple;
    }

    template <class... Indices>
    MIndex<Indices...>& MIndex<Indices...>::at(const MetaType& metaPos)
    {
	sfor_pn<0,sizeof...(Indices)>
	    ( [&](auto i) { std::get<i>(mIPack)->at( std::get<i>(metaPos) ); return 0; } );
	IB::mPos = sfor_m<sizeof...(Indices),0>
	    ( [&](auto i) { return std::get<i>(mIPack); },
	      [&](auto a, auto b) {return a->pos() + b*a->max();}, 0 );
	return *this;
    }

    template <class... Indices>
    SizeT MIndex<Indices...>::dim()
    {
	return sizeof...(Indices);
    }

    template <class... Indices>
    Sptr<typename MIndex<Indices...>::RangeType>
    MIndex<Indices...>::range()
    {
	return std::dynamic_pointer_cast<RangeType>( mRangePtr );
    }

    template <class... Indices>	
    SizeT MIndex<Indices...>::getStepSize(SizeT n)
    {
	if(n >= sizeof...(Indices)){
	    assert(0);
	    // throw !!
	}
	return mBlockSizes[n+1];
    }
    /*
    template <class... Indices>
    template <class Exprs>
    auto MIndex<Indices...>::ifor(SizeT step, Exprs exs) const
    {
	return RangeHelper::mkFor<0>(step, mIPack, mBlockSizes, exs);
    }
    */
    
    /*********************
     *   MRangeFactory   *
     *********************/

    template <class... Ranges>
    MRangeFactory<Ranges...>::MRangeFactory(const Tuple<Sptr<Ranges>...>& rs) :
	mRs(rs)
    {}

    template <class... Ranges>
    MRangeFactory<Ranges...>::MRangeFactory(const Tuple<Sptr<Ranges>...>& rs,
					    const RangePtr& ref) :
	mRs(rs),
	mRef(ref)
    {}

    template <class... Ranges>
    void MRangeFactory<Ranges...>::make()
    {
	auto info = typeid(MRange<Ranges...>);
	if(mRef != nullptr) {
	    mProd = this->fromCreated[info.hash_code()][mRef->id()];
	}
	if(mProd == nullptr) {
	    RangePtr key = mProd = std::shared_ptr<MRange<Ranges...>>
		( new MRange<Ranges...>( mSpace ) );
	    if(mRef != nullptr) { key = mRef }
	    this->addToCreated(info, { key->id() }, mProd);
	}
    }
    
    /**************
     *   MRange   *
     **************/

    template <class... Ranges>
    MRange<Ranges...>::MRange(const Tuple<Sptr<Ranges>...>& rs) :
	mRs(rs),
	mA( mkA( std::make_index_sequence<sizeof...(Ranges)>{} ) )
    {}

    template <class... Ranges>
    RangePtr MRange<Ranges...>::sub(SizeT num) const
    {
	CXZ_ASSERT(num < this->dim(), "index out of range");
	return mA[num];
    }

    template <class... Ranges>
    SizeT MRange<Ranges...>::size() const
    {
	return this->sizei(std::make_index_sequence<sizeof...(Ranges)> {});
    }

    template <class... Ranges>
    SizeT MRange<Ranges...>::dim() const
    {
	return sizeof...(Ranges);
    }
    
    template <class... Ranges>
    String MRange<Ranges...>::stringMeta(SizeT pos) const
    {
	
    }

    template <class... Ranges>
    IndexType MRange<Ranges...>::begin() const
    {
	
    }
    
    template <class... Ranges>
    IndexType MRange<Ranges...>::end() const
    {

    }

    template <class... Ranges>
    decltype(auto) MRange<Ranges...>::space() const
    {
	return mRs;
    }

    template <class... Ranges>
    const MetaType MRange<Ranges...>::get(SizeT pos) const
    {
	
    }

    template <class... Ranges>
    SizeT MRange<Ranges...>::getMeta(const MetaType& metaPos) const
    {
	assert(0);
	//return RangeHelper::getMeta<sizeof...(Ranges)-1>(mSpace,metaPos);
    }

    /************************
     *   MRange (private)   *
     ************************/

    template <class... Ranges>
    template <SizeT... Is>
    decltype(auto) MRange<Ranges...>::mkA(std::index_sequence<Is...> is) const
    {
	return Arr<RangePtr,sizeof...(Ranges)> { std::get<Is>(mRs)... };
    }

    template <class... Ranges>
    SizeT MRange<Ranges...>::sizei(std::index_sequence<Is...> is) const
    {
	return ( std::get<Is>(mRs)->size() * ... );
    }
    
}

#endif
