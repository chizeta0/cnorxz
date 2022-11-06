
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
    template <class G, class F, SizeT... Is>
    constexpr decltype(auto) MIndex<Indices...>::accumulate2i(const G& g, const F& f, std::index_sequence<Is...> is) const
    {
	return f( g(std::get<Is>(mIPack), std::get<Is>(mBlockSizes))... );
    }

    template <class... Indices>
    template <SizeT B, SizeT E, class G, class F>
    constexpr decltype(auto) MIndex<Indices...>::accumulate(const G& g, const F& f) const
    {
	return accumulatei(g, f, mkIndexSequence<B,E>())
    }

    template <class... Indices>
    template <SizeT B, SizeT E, class G, class F>
    constexpr decltype(auto) MIndex<Indices...>::accumulate2(const G& g, const F& f) const
    {
	return accumulate2i(g, f, mkIndexSequence<B,E>())
    }

    template <class... Indices>
    template <SizeT... Is>
    constexpr decltype(auto) MIndex<Indices...>::mkIPack(SizeT pos, std::index_sequence<Is...> is) const
    {
	static_assert(sizeof...(Is) == sizeof...(Indices),
		      "sequence size does not match number of indices");
	return std::make_tuple( std::make_shared<Indices>( mRange->sub(Is) )... );
				//, (pos / std::get<Is>(mBlockSizes) ) %  )... );
    }
    
    template <class... Indices>
    template <SizeT... Is>
    inline void MIndex<Indices...>::setIPack(std::index_sequence<Is...> is) const
    {
	( (std::get<Is>(mIPack) = (IB::mPos / std::get<Is>(mBlockSize)) % std::get<Is>(mIPack)->max() ),... );
    }
    
    template <class... Indices>
    template <SizeT... Is>
    constexpr decltype(auto) MIndex<Indices...>::mkBlockSizes(std::index_sequence<Is...> is) const
    {
	static_assert(sizeof...(Is) == sizeof...(Indices)-1,
		      "set lowest block size manually");
	return std::make_tuple
	    ( accumulate<Is,sizeof...(Indices)>
	      ( [](const auto& i) { return i->max(); },
		[](const auto&... as) { return (as * ...); } )...,
	      SPos<1>() );
    }

    template <class... Indices>
    template <SizeT... Is>
    constexpr decltype(auto) MIndex<Indices...>::mkPos(std::index_sequence<Is...> is) const
    {
	return ( (std::get<Is>(mIPack)*std::get<Is>(mBlockSizes)) + ... );
    }

    template <class... Indices>
    template <SizeT I>
    inline void MIndex<Indices...>::up()
    {
	auto& i = std::get<I>(mIPack);
	if constexpr(I != 0){
	    if(i->pos() == i->max()-1){
		IB::mPos -= std::get<I>(mBlockSize) * i->pos();
		(*i) = 0;
		upi<N-1>();
		return;
	    }
	}
	IB::mPos += std::get<I>(mBlockSize);
	++(*i);
    }

    template <class... Indices>
    template <SizeT I>
    inline void MIndex<Indices...>::down()
    {
	auto& i = std::get<I>(mIPack);
	if constexpr(I != 0){
	    if(i->pos() == 0){
		(*i) = i->max()-1;
		IB::mPos += std::get<I>(mBlockSize) * i->pos();
		downi<N-1>();
		return;
	    }
	}
	IB::mPos -= std::get<I>(mBlockSize);
	--(*i);
    }

    template <class... Indices>
    template <SizeT... Is>
    inline String MIndex<Indices...>::mkStringMeta(std::index_sequence<Is...> is) const
    {
	const String blim = "[";
	const String elim = "]";
	const String dlim = ",";
	static_assert(sizeof...(Is) == sizeof...(Indices)-1,
		      "set last position outside the index pack");
	if constexpr(sizeof...(Is) == 0){
	    return blim + std::get<0>(mIPack)->stringMeta() + elim;
	}
	else {
	    return blim +
		( ( std::get<Is>(mIPack)->stringMeta() + dlim ) + ... +
		  std::get<sizeof...(Indices)-1>(mIPack)->stringMeta() )
		+ elim;
	}
    }

    template <class... Indices>
    template <SizeT... Is>
    inline typename MIndex<Indices...>::MetaType MIndex<Indices...>::mkMeta(std::index_sequence<Is...> is) const
    {
	return std::make_tuple( std::get<Is>(mIPack)->meta()... );
    }
    
    template <class... Indices>
    template <SizeT I, class Xpr, class F>
    constexpr decltype(auto) MIndex<Indices...>::mkIFor(const Xpr& xpr, F&& f) const
    {
	if constexpr(I == sizeof..(Indices)-1){
	    return std::get<I>(mIPack)->ifor(xpr,f);
	}
	else {
	    return std::get<I>(mIPack)->ifor( mkIFor<I+1>( xpr, f ), f );
	}
    }

    template <SizeT... Is>
    inline void ati(const MetaType& meta, std::index_sequence<Is...> is) const
    {
	( std::get<Is>(mIPack)->at( std::get<Is>(meta) ), ... );
    }

    /**************
     *   MIndex   *
     **************/

    template <class... Indices>
    template <class MRange>
    MIndex<Indices...>::MIndex(const Sptr<MRange>& range, SizeT pos) :
	IndexInterface<MIndex<Indices...>,Tuple<typename Indices::MetaType...>>(0),
	mIPack(mkIPack(IB::mPos, std::make_index_sequence<sizeof...(Indices)>{})),
	mBlockSizes(mkBlockSizes(IB::mPos), std::make_index_sequence<sizeof...(Indices)>{}),
	mRange(range),
    {
	(*this) = pos;
    }

    template <class... Indices>
    MIndex<Indices...>& MIndex<Indices...>::operator()(const Sptr<MIndex>& mi)
    {
	mIPack = mi.mIPack;
	IB::mPos = mkPos(std::make_index_sequence<sizeof...(Indices)>{});
	return *this;
    }

    template <class... Indices>
    MIndex<Indices...>& MIndex<Indices...>::operator=(SizeT pos)
    {
	IB::mPos = pos;
	setIPack(std::make_index_sequence<sizeof...(Indices)>{});
	return *this;
    }

    template <class... Indices>
    MIndex<Indices...>& MIndex<Indices...>::operator++()
    {
	// End state is defined by high-index being end while all other indices are zero
	auto& i0 = std::get<0>(mIPack);
	if(i0->pos() != i0->max()){
	    up<sizeof...(Indices)-1>();
	}
	return *this;
    }

    template <class... Indices>
    MIndex<Indices...>& MIndex<Indices...>::operator--()
    {
	if(IB::mPos != 0){
	    down<sizeof...(Indices)-1>();
	}
	return *this;
    }

    template <class... Indices>
    String MIndex<Indices...>::stringMeta() const
    {
	return mkStringMeta(std::make_index_sequence<sizeof...(Indices)-1>{});
    }
    
    template <class... Indices>
    typename MIndex<Indices...>::MetaType MIndex<Indices...>::meta() const
    {
	return mkMeta(std::make_index_sequence<sizeof...(Indices)>{});
    }

    template <class... Indices>
    MIndex<Indices...>& MIndex<Indices...>::at(const MetaType& metaPos)
    {
	ati(metaPos, std::make_index_sequence<sizeof...(Indices)>{});
	IB::mPos = mkPos(std::make_index_sequence<sizeof...(Indices)>{});
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
    template <SizeT I>
    decltype(auto) MIndex<Indices...>::stepSize(const IndexId<I>& id) const;
    {
	return accumulate2( [](const auto& i, const auto& b) { return i->stepSize(id) * b },
			    [](const auto&... ss) { return ( ss + ... ); });
    }

    template <class... Indices>	
    template <class Xpr, class F>
    constexpr decltype(auto) MIndex<Indices...>::ifor(const Xpr& xpr, F&& f) const
    {
	return mkIFor<0>(xpr, f);
    }
    
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
	return (begin()+pos).stringMeta();
    }

    template <class... Ranges>
    decltype(auto) MRange<Ranges...>::space() const
    {
	return mRs;
    }

    template <class... Ranges>
    const MetaType MRange<Ranges...>::get(SizeT pos) const
    {
	return (begin()+pos)->meta();
    }

    template <class... Ranges>
    SizeT MRange<Ranges...>::getMeta(const MetaType& metaPos) const
    {
	auto i = begin();
	return i.at(metaPos).pos();
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
