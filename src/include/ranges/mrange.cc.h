
#ifndef __cxz_mrange_cc_h__
#define __cxz_mrange_cc_h__

#include "mrange.h"

namespace CNORXZ
{
    /***********************
     *   MIndex (private)  *
     ***********************/

    template <class... Indices>
    template <SizeT... Is>
    constexpr decltype(auto) MIndex<Indices...>::mkIPack(SizeT pos, Isq<Is...> is) const
    {
	static_assert(sizeof...(Is) == NI,
		      "sequence size does not match number of indices");
	return std::make_tuple( std::make_shared<Indices>( mRange->sub(Is) )... );
    }
    
    template <class... Indices>
    template <SizeT... Is>
    constexpr decltype(auto) MIndex<Indices...>::mkBlockSizes(const IndexPack& ipack, Isq<Is...> is)
    {
	return std::make_tuple
	    ( iter<Is,NI>
	      // replace UPos by SPos where possible !!!
	      ( [&](auto i) { return UPos(std::get<i>(ipack)->max()); },
		[&](const auto&... as) { return (as * ...); } )...,
	      SPos<1>() );
    }

    template <class... Indices>
    template <SizeT I>
    inline void MIndex<Indices...>::up()
    {
	auto& i = std::get<I>(mIPack);
	if constexpr(I != 0){
	    if(i->pos() == i->max()-1){
		IB::mPos -= std::get<I>(mBlockSizes).val() * i->pos();
		(*i) = 0;
		up<I-1>();
		return;
	    }
	}
	IB::mPos += std::get<I>(mBlockSizes).val();
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
		IB::mPos += std::get<I>(mBlockSizes).val() * i->pos();
		down<I-1>();
		return;
	    }
	}
	IB::mPos -= std::get<I>(mBlockSizes).val();
	--(*i);
    }

    template <class... Indices>
    template <SizeT I, class Xpr, class F>
    constexpr decltype(auto) MIndex<Indices...>::mkIFor(const Xpr& xpr, F&& f) const
    {
	if constexpr(I == sizeof...(Indices)-1){
	    return std::get<I>(mIPack)->ifor(xpr,f);
	}
	else {
	    return std::get<I>(mIPack)->ifor( mkIFor<I+1>( xpr, f ), f );
	}
    }

    /**************
     *   MIndex   *
     **************/

    template <class... Indices>
    MIndex<Indices...>::MIndex(const MIndex& i) :
	IndexInterface<MIndex<Indices...>,Tuple<typename Indices::MetaType...>>(i.pos()),
	mIPack(mkIPack(IB::mPos, Isqr<0,NI>{})),
	mBlockSizes(mkBlockSizes(mIPack,Isqr<0,NI-1>{})),
	mRange(rangeCast<RangeType>(i.range()))
    {}

    template <class... Indices>
    MIndex<Indices...>& MIndex<Indices...>::operator=(const MIndex& i)
    {
	IndexInterface<MIndex<Indices...>,Tuple<typename Indices::MetaType...>>::operator=(i);
	mIPack = mkIPack(IB::mPos, Isqr<0,NI>{});
	mBlockSizes = mkBlockSizes(mIPack,Isqr<0,NI-1>{});
	mRange = rangeCast<RangeType>(i.range());
	return *this;
    }

    template <class... Indices>
    MIndex<Indices...>::MIndex(const RangePtr& range, SizeT pos) :
	IndexInterface<MIndex<Indices...>,Tuple<typename Indices::MetaType...>>(0),
	mIPack(mkIPack(IB::mPos, Isqr<0,NI>{})),
	mBlockSizes(mkBlockSizes(mIPack,Isqr<0,NI-1>{})),
	mRange(rangeCast<RangeType>(range))
    {
	(*this) = pos;
    }

    template <class... Indices>
    MIndex<Indices...>& MIndex<Indices...>::operator=(SizeT pos)
    {
	IB::mPos = pos;
	iter<0,NI>( [&](auto i) { *std::get<i>(mIPack) = (IB::mPos / std::get<i>(mBlockSizes).val()) % std::get<i>(mIPack)->max(); }, NoF{} );
	return *this;
    }

    template <class... Indices>
    MIndex<Indices...>& MIndex<Indices...>::operator++()
    {
	// End state is defined by high-index being end while all other indices are zero
	auto& i0 = std::get<0>(mIPack);
	if(i0->pos() != i0->max()){
	    up<NI-1>();
	}
	return *this;
    }

    template <class... Indices>
    MIndex<Indices...>& MIndex<Indices...>::operator--()
    {
	if(IB::mPos != 0){
	    down<NI-1>();
	}
	return *this;
    }

    template <class... Indices>
    MIndex<Indices...> MIndex<Indices...>::operator+(Int n) const
    {
	MIndex o(*this);
	return o += n;
    }

    template <class... Indices>
    MIndex<Indices...> MIndex<Indices...>::operator-(Int n) const
    {
	MIndex o(*this);
	return o -= n;
    }

    template <class... Indices>
    MIndex<Indices...>& MIndex<Indices...>::operator+=(Int n)
    {
	if(-n > static_cast<long int>(IB::mPos)){
	    (*this) = 0;
	}
	const SizeT p = IB::mPos + n;
	if(p > max()){
	    (*this) = max();
	}
	(*this) = p;
	return *this;
    }

    template <class... Indices>
    MIndex<Indices...>& MIndex<Indices...>::operator-=(Int n)
    {
	if(n > static_cast<long int>(IB::mPos)){
	    (*this) = 0;
	}
	const SizeT p = IB::mPos + n;
	if(p > max()){
	    (*this) = max();
	}
	(*this) = p;
	return *this;
    }

    template <class... Indices>
    SizeT MIndex<Indices...>::max() const
    {
	return mRange->size();
    }

    template <class... Indices>
    IndexId<0> MIndex<Indices...>::id() const
    {
	return IndexId<0>(this->ptrId());
    }

    template <class... Indices>
    typename MIndex<Indices...>::MetaType MIndex<Indices...>::operator*() const
    {
	return meta();
    }
    
    template <class... Indices>	
    SizeT MIndex<Indices...>::dim() const
    {
	return NI;
    }	

    template <class... Indices>	
    Sptr<typename MIndex<Indices...>::RangeType> MIndex<Indices...>::range() const
    {
	return mRange;
    }

    template <class... Indices>	
    template <SizeT I>
    decltype(auto) MIndex<Indices...>::stepSize(const IndexId<I>& id) const
    {
	return iter<0,NI>
	    ( [&](auto i) { return std::get<i>(mIPack)->stepSize(id) * std::get<i>(mBlockSizes); },
	      [](const auto&... ss) { return ( ss + ... ); });
    }

    template <class... Indices>
    String MIndex<Indices...>::stringMeta() const
    {
	const String blim = "(";
	const String elim = ")";
	const String dlim = ",";
	return iter<1,NI>
	    ( [&](auto i) { return std::get<i>(mIPack)->stringMeta(); },
	      [&](const auto&... xs) {
		  return blim + std::get<0>(mIPack)->stringMeta() + ( (dlim + xs) + ... ) + elim;
	      } );
    }
    
    template <class... Indices>
    typename MIndex<Indices...>::MetaType MIndex<Indices...>::meta() const
    {
	return iter<0,NI>( [&](auto i) { return std::get<i>(mIPack)->meta(); },
			   [](const auto&... xs) { return std::make_tuple(xs...); } );
    }

    template <class... Indices>
    MIndex<Indices...>& MIndex<Indices...>::at(const MetaType& metaPos)
    {
	iter<0,NI>( [&](auto i) { std::get<i>(mIPack)->at( std::get<i>(metaPos) ); }, NoF {} );
	IB::mPos = iter<0,NI>
	    ( [&](auto i) { return std::get<i>(mIPack)->pos()*std::get<i>(mBlockSizes).val(); },
	      [](const auto&... xs) { return (xs + ...); });
	return *this;
    }

    template <class... Indices>	
    template <class Xpr, class F>
    constexpr decltype(auto) MIndex<Indices...>::ifor(const Xpr& xpr, F&& f) const
    {
	return mkIFor<0>(xpr, f);
    }

    template <class... Indices>
    MIndex<Indices...>& MIndex<Indices...>::operator()(const Sptr<MIndex>& mi)
    {
	mIPack = mi.mIPack;
	IB::mPos = iter<0,NI>
	    ( [&](auto i) { return std::get<i>(mIPack)->pos()*std::get<i>(mBlockSizes).val(); },
	      [](const auto&... xs) { return (xs + ...); });
	return *this;
    }

    template <class... Indices>
    const typename MIndex<Indices...>::IndexPack& MIndex<Indices...>::pack() const
    {
	return mIPack;
    }

    template <class... Indices>
    const auto& MIndex<Indices...>::blockSizes() const
    {
	return mBlockSizes;
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
	const auto& info = typeid(MRange<Ranges...>);
	if(mRef != nullptr) {
	    mProd = this->fromCreated(info, {mRef->id()});
	}
	if(mProd == nullptr) {
	    RangePtr key = mProd = std::shared_ptr<MRange<Ranges...>>
		( new MRange<Ranges...>( mRs ) );
	    if(mRef != nullptr) { key = mRef; }
	    this->addToCreated(info, { key->id() }, mProd);
	}
    }
    
    /**************
     *   MRange   *
     **************/

    template <class... Ranges>
    MRange<Ranges...>::MRange(const Tuple<Sptr<Ranges>...>& rs) :
	mRs(rs),
	mA( mkA() )
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
	return iter<0,NR>( [&](auto i) { return std::get<i>(mRs)->size(); },
			   [](const auto&... xs) { return (xs * ...); } );
    }

    template <class... Ranges>
    SizeT MRange<Ranges...>::dim() const
    {
	return NR;
    }
    
    template <class... Ranges>
    String MRange<Ranges...>::stringMeta(SizeT pos) const
    {
	return (this->begin()+pos).stringMeta();
    }

    template <class... Ranges>
    const TypeInfo& MRange<Ranges...>::type() const
    {
	return typeid(MRange<Ranges...>);
    }
    
    template <class... Ranges>
    const TypeInfo& MRange<Ranges...>::metaType() const
    {
	return typeid(MetaType);
    }

    template <class... Ranges>
    decltype(auto) MRange<Ranges...>::space() const
    {
	return mRs;
    }

    template <class... Ranges>
    const typename MRange<Ranges...>::MetaType MRange<Ranges...>::get(SizeT pos) const
    {
	return (this->begin()+pos)->meta();
    }

    template <class... Ranges>
    SizeT MRange<Ranges...>::getMeta(const MetaType& metaPos) const
    {
	auto i = this->begin();
	return i.at(metaPos).pos();
    }

    /************************
     *   MRange (private)   *
     ************************/

    template <class... Ranges>
    decltype(auto) MRange<Ranges...>::mkA() const
    {
	return iter<0,NR>([&](auto i) { return std::get<i>(mRs); },
			  [](const auto&... xs) { return Arr<RangePtr,NR> { xs... }; } );
    }

    /****************************
     *   non-member functions   *
     ****************************/

    template <class... Ranges>
    RangePtr mrange(const Sptr<Ranges>&... rs)
    {
	return MRangeFactory<Ranges...>(std::make_tuple(rs...)).create();
    }
    
}

#endif
