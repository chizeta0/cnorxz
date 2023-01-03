
#ifndef __cxz_mrange_cc_h__
#define __cxz_mrange_cc_h__

#include "mrange.h"

namespace CNORXZ
{
    /************************
     *   GMIndex (private)  *
     ************************/

    template <class FormatT, class... Indices>
    template <SizeT... Is>
    constexpr decltype(auto) GMIndex<FormatT,Indices...>::mkIPack(Isq<Is...> is) const
    {
	static_assert(sizeof...(Is) == NI,
		      "sequence sioze does not match number of indices");
	CXZ_ASSERT( ( (mRange->sub(Is) != nullptr) and ... ),
		    "subranges not available" );
	return std::make_tuple( std::make_shared<Indices>( mRange->sub(Is) )... );
    }

    template <class FormatT, class... Indices>
    constexpr decltype(auto) GMIndex<FormatT,Indices...>::mkLMax(const IndexPack& ipack)
    {
	return iter<0,NI>( [&](auto i) { return std::get<i>(ipack)->lmax(); },
			   [](auto... e) { return (e * ...); });
    }

    template <class FormatT, class... Indices>
    constexpr decltype(auto) GMIndex<FormatT,Indices...>::mkPMax(const IndexPack& ipack, const FormatT& format)
    {
	if constexpr(std::is_same<FormatT,None>::value){
	    return mkLMax(ipack);
	}
	else {
	    return iter<0,NI>
		( [&](auto i)
		{ return (std::get<i>(ipack)->pmax() - SPos<1>()) * format[i]; },
		    [](auto... e) { return (e + ...); }) + SPos<1>();
	}
    }

    template <class FormatT, class... Indices>
    inline void GMIndex<FormatT,Indices...>::mkPos()
    {
	mLex = iter<0,NI>
	    ([&](auto i) { return std::get<i>(mIPack)->lex() * mLexFormat[i].val(); },
	     [](const auto&... e) { return (e + ...); });
	if constexpr(not std::is_same<FormatT,None>::value){
	    IB::mPos = iter<0,NI>
		([&](auto i) { return std::get<i>(mIPack)->pos() * mFormat[i].val(); },
		 [](const auto&... e) { return (e + ...); });
	}
	else {
	    IB::mPos = mLex;
	}
    }

    template <class FormatT, class... Indices>
    template <SizeT... Is>
    constexpr decltype(auto) GMIndex<FormatT,Indices...>::mkLexFormat(const IndexPack& ipack, Isq<Is...> is)
    {
	return gmformat
	    ( iter<Is,NI>
	      ( [&](auto i) { return std::get<i>(ipack)->pmax(); },
		[](const auto&... as) { return (as * ...); } )...,
	      SPos<1>() );
    }

    template <class FormatT, class... Indices>
    template <SizeT I>
    inline void GMIndex<FormatT,Indices...>::up()
    {
	std::integral_constant<SizeT,I> i;
	auto& ind = std::get<I>(mIPack);
	if constexpr(I != 0){
	    if(ind->lex() == ind->lmax().val()-1){
		IB::mPos -= format()[i].val() * ind->pos();
		if constexpr(not std::is_same<FormatT,None>::value){
		    mLex -= lexFormat()[i].val() * ind->lex();
		}
		(*ind) = 0;
		up<I-1>();
		return;
	    }
	}
	IB::mPos += format()[i].val();
	if constexpr(not std::is_same<FormatT,None>::value){
	    mLex += lexFormat()[i].val();
	}
	++(*ind);
    }

    template <class FormatT, class... Indices>
    template <SizeT I>
    inline void GMIndex<FormatT,Indices...>::down()
    {
	std::integral_constant<SizeT,I> i;
	auto& ind = std::get<I>(mIPack);
	if constexpr(I != 0){
	    if(ind->lex() == 0){
		(*ind) = ind->lmax().val()-1;
		IB::mPos += format()[i].val() * ind->pos();
		if constexpr(not std::is_same<FormatT,None>::value){
		    mLex += lexFormat()[i].val() * ind->lex();
		}
		down<I-1>();
		return;
	    }
	}
	IB::mPos -= format()[i].val();
	if constexpr(not std::is_same<FormatT,None>::value){
	    mLex -= lexFormat()[i].val();
	}
	--(*ind);
    }

    template <class FormatT, class... Indices>
    template <SizeT I, class Xpr, class F>
    constexpr decltype(auto) GMIndex<FormatT,Indices...>::mkIFor(const Xpr& xpr, F&& f) const
    {
	if constexpr(I == sizeof...(Indices)-1){
	    return std::get<I>(mIPack)->ifor(xpr,std::forward<F>(f));
	}
	else {
	    auto f1 = f;
	    auto f2 = f1;
	    return std::get<I>(mIPack)->ifor( mkIFor<I+1>( xpr, std::move(f1) ), std::move(f2) );
	}
    }

    /***************
     *   GMIndex   *
     ***************/

    template <class FormatT, class... Indices>
    constexpr GMIndex<FormatT,Indices...>::GMIndex(const GMIndex& i) :
	IndexInterface<GMIndex<FormatT,Indices...>,Tuple<typename Indices::MetaType...>>(0),
	mRange(rangeCast<RangeType>(i.range())),
	mIPack(mkIPack(Isqr<0,NI>{})),
	mLexFormat(mkLexFormat(mIPack,Isqr<1,NI>{})),
	mFormat(i.mFormat),
	mLMax(mkLMax(mIPack)),
	mPMax(mkPMax(mIPack,mFormat))
    {
	*this = i.lex();
    }

    template <class FormatT, class... Indices>
    constexpr GMIndex<FormatT,Indices...>& GMIndex<FormatT,Indices...>::operator=(const GMIndex& i)
    {
	IndexInterface<GMIndex<FormatT,Indices...>,Tuple<typename Indices::MetaType...>>::operator=(0);
	mRange = rangeCast<RangeType>(i.range());
	mIPack = mkIPack(Isqr<0,NI>{});
	mLexFormat = mkLexFormat(mIPack,Isqr<1,NI>{});
	mFormat = i.mFormat;
	mLMax = mkLMax(mIPack);
	mPMax = mkPMax(mIPack,mFormat);
	return *this = i.lex();
    }

    template <class FormatT, class... Indices>
    constexpr GMIndex<FormatT,Indices...>::GMIndex(const Indices&... is) :
	IndexInterface<GMIndex<FormatT,Indices...>,Tuple<typename Indices::MetaType...>>(0),
	mRange(std::dynamic_pointer_cast<RangeType>(mrange(is.range()...))),
	mIPack(std::make_shared<Indices>(is)...),
	mLexFormat(mkLexFormat(mIPack,Isqr<1,NI>{})),
	mFormat(),
	mLMax(mkLMax(mIPack)),
	mPMax(mkPMax(mIPack,mFormat))
    {
	mkPos();
    }

    template <class FormatT, class... Indices>
    constexpr GMIndex<FormatT,Indices...>::GMIndex(const FormatT& bs, const Indices&... is) :
	IndexInterface<GMIndex<FormatT,Indices...>,Tuple<typename Indices::MetaType...>>(0),
	mRange(std::dynamic_pointer_cast<RangeType>(mrange(is.range()...))),
	mIPack(std::make_shared<Indices>(is)...),
	mLexFormat(mkLexFormat(mIPack,Isqr<1,NI>{})),
	mFormat(bs),
	mLMax(mkLMax(mIPack)),
	mPMax(mkPMax(mIPack,mFormat))
    {
	mkPos();
    }

    template <class FormatT, class... Indices>
    constexpr GMIndex<FormatT,Indices...>::GMIndex(const Sptr<Indices>&... is) :
	IndexInterface<GMIndex<FormatT,Indices...>,Tuple<typename Indices::MetaType...>>(0),
	mRange(std::dynamic_pointer_cast<RangeType>(mrange(is->range()...))),
	mIPack(is...),
	mLexFormat(mkLexFormat(mIPack,Isqr<1,NI>{})),
	mFormat(),
	mLMax(mkLMax(mIPack)),
	mPMax(mkPMax(mIPack,mFormat))
    {
	mkPos();
    }

    template <class FormatT, class... Indices>
    constexpr GMIndex<FormatT,Indices...>::GMIndex(const FormatT& bs,
						     const Sptr<Indices>&... is) :
	IndexInterface<GMIndex<FormatT,Indices...>,Tuple<typename Indices::MetaType...>>(0),
	mRange(std::dynamic_pointer_cast<RangeType>(mrange(is->range()...))),
	mIPack(is...),
	mLexFormat(mkLexFormat(mIPack,Isqr<1,NI>{})),
	mFormat(bs),
	mLMax(mkLMax(mIPack)),
	mPMax(mkPMax(mIPack,mFormat))
    {
	mkPos();
    }

    template <class FormatT, class... Indices>
    constexpr GMIndex<FormatT,Indices...>::GMIndex(const RangePtr& range, SizeT lexpos) :
	IndexInterface<GMIndex<FormatT,Indices...>,Tuple<typename Indices::MetaType...>>(0),
	mRange(rangeCast<RangeType>(range)),
	mIPack(mkIPack(Isqr<0,NI>{})),
	mLexFormat(mkLexFormat(mIPack,Isqr<1,NI>{})),
	mFormat(),
	mLMax(mkLMax(mIPack)),
	mPMax(mkPMax(mIPack,mFormat))
    {
	*this = lexpos;
    }

    template <class FormatT, class... Indices>
    constexpr GMIndex<FormatT,Indices...>::GMIndex(const RangePtr& range, const FormatT& format, SizeT lexpos) :
	IndexInterface<GMIndex<FormatT,Indices...>,Tuple<typename Indices::MetaType...>>(0),
	mRange(rangeCast<RangeType>(range)),
	mIPack(mkIPack(Isqr<0,NI>{})),
	mLexFormat(mkLexFormat(mIPack,Isqr<1,NI>{})),
	mFormat(format),
	mLMax(mkLMax(mIPack)),
	mPMax(mkPMax(mIPack,mFormat))
    {
	*this = lexpos;
    }

    template <class FormatT, class... Indices>
    GMIndex<FormatT,Indices...>& GMIndex<FormatT,Indices...>::operator=(SizeT lexpos)
    {
	if(lexpos >= lmax().val()){
	    if constexpr(not std::is_same<FormatT,None>::value){ mLex = lmax().val(); }
	    IB::mPos = pmax().val();
	    return *this;
	}
	if constexpr(not std::is_same<FormatT,None>::value){ mLex = lexpos; }
	IB::mPos = iter<0,NI>( [&](auto i) {
	    *std::get<i>(mIPack) = (lex() / lexFormat()[i].val()) % std::get<i>(mIPack)->lmax().val();
	    return format()[i].val() * std::get<i>(mIPack)->pos();
	}, [](const auto&... e) { return (e + ...); } );
	return *this;
    }

    template <class FormatT, class... Indices>
    GMIndex<FormatT,Indices...>& GMIndex<FormatT,Indices...>::operator++()
    {
	if(lex() == lmax().val()-1){
	    return *this = lmax().val();
	}
	if(lex() != lmax().val()){
	    up<NI-1>();
	}
	return *this;
    }

    template <class FormatT, class... Indices>
    GMIndex<FormatT,Indices...>& GMIndex<FormatT,Indices...>::operator--()
    {
	if(lex() == lmax().val()){
	    return *this = lmax().val()-1;
	}
	if(lex() != 0){
	    down<NI-1>();
	}
	return *this;
    }

    template <class FormatT, class... Indices>
    GMIndex<FormatT,Indices...> GMIndex<FormatT,Indices...>::operator+(Int n) const
    {
	GMIndex o(*this);
	return o += n;
    }

    template <class FormatT, class... Indices>
    GMIndex<FormatT,Indices...> GMIndex<FormatT,Indices...>::operator-(Int n) const
    {
	GMIndex o(*this);
	return o -= n;
    }

    template <class FormatT, class... Indices>
    GMIndex<FormatT,Indices...>& GMIndex<FormatT,Indices...>::operator+=(Int n)
    {
	if(-n > static_cast<long int>(lex())){
	    (*this) = 0;
	}
	const SizeT p = lex() + n;
	if(p > lmax().val()){
	    (*this) = lmax().val();
	}
	(*this) = p;
	return *this;
    }

    template <class FormatT, class... Indices>
    GMIndex<FormatT,Indices...>& GMIndex<FormatT,Indices...>::operator-=(Int n)
    {
	if(n > static_cast<long int>(lex())){
	    (*this) = 0;
	}
	const SizeT p = lex() + n;
	if(p > lmax().val()){
	    (*this) = lmax().val();
	}
	(*this) = p;
	return *this;
    }

    template <class FormatT, class... Indices>
    SizeT GMIndex<FormatT,Indices...>::lex() const
    {
	if constexpr(std::is_same<FormatT,None>::value){
	    return IB::mPos;
	}
	else {
	    return mLex;
	}
    }
    
    template <class FormatT, class... Indices>
    constexpr decltype(auto) GMIndex<FormatT,Indices...>::pmax() const
    {
	return mPMax;
    }

    template <class FormatT, class... Indices>
    constexpr decltype(auto) GMIndex<FormatT,Indices...>::lmax() const
    {
	return mPMax;
    }

    template <class FormatT, class... Indices>
    IndexId<0> GMIndex<FormatT,Indices...>::id() const
    {
	return IndexId<0>(this->ptrId());
    }

    template <class FormatT, class... Indices>
    typename GMIndex<FormatT,Indices...>::MetaType GMIndex<FormatT,Indices...>::operator*() const
    {
	return meta();
    }
    
    template <class FormatT, class... Indices>	
    constexpr SizeT GMIndex<FormatT,Indices...>::dim() const
    {
	return NI;
    }	

    template <class FormatT, class... Indices>	
    Sptr<typename GMIndex<FormatT,Indices...>::RangeType> GMIndex<FormatT,Indices...>::range() const
    {
	return mRange;
    }

    template <class FormatT, class... Indices>	
    template <SizeT I>
    decltype(auto) GMIndex<FormatT,Indices...>::stepSize(const IndexId<I>& id) const
    {
	return iter<0,NI>
	    ( [&](auto i) { return std::get<i>(mIPack)->stepSize(id) * format()[i]; },
	      [](const auto&... ss) { return ( ss + ... ); });
    }

    template <class FormatT, class... Indices>
    String GMIndex<FormatT,Indices...>::stringMeta() const
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
    
    template <class FormatT, class... Indices>
    typename GMIndex<FormatT,Indices...>::MetaType GMIndex<FormatT,Indices...>::meta() const
    {
	return iter<0,NI>( [&](auto i) { return std::get<i>(mIPack)->meta(); },
			   [](const auto&... xs) { return std::make_tuple(xs...); } );
    }

    template <class FormatT, class... Indices>
    GMIndex<FormatT,Indices...>& GMIndex<FormatT,Indices...>::at(const MetaType& metaPos)
    {
	iter<0,NI>( [&](auto i) { std::get<i>(mIPack)->at( std::get<i>(metaPos) ); }, NoF {} );
	IB::mPos = iter<0,NI>
	    ( [&](auto i) { return std::get<i>(mIPack)->pos()*format()[i].val(); },
	      [](const auto&... xs) { return (xs + ...); });
	return *this;
    }
    
    template <class FormatT, class... Indices>	
    template <class Xpr, class F>
    constexpr decltype(auto) GMIndex<FormatT,Indices...>::ifor(const Xpr& xpr, F&& f) const
    {
	return mkIFor<0>(xpr, std::forward<F>(f));
    }

    template <class FormatT, class... Indices>
    GMIndex<FormatT,Indices...>& GMIndex<FormatT,Indices...>::operator()(const Sptr<MIndex<Indices...>>& mi)
    {
	mIPack = mi.pack();
	mkPos();
	return *this;
    }

    template <class FormatT, class... Indices>
    GMIndex<FormatT,Indices...>& GMIndex<FormatT,Indices...>::operator()()
    {
	mkPos();
	return *this;
    }

    template <class FormatT, class... Indices>
    const typename GMIndex<FormatT,Indices...>::IndexPack& GMIndex<FormatT,Indices...>::pack() const
    {
	return mIPack;
    }

    template <class FormatT, class... Indices>
    const auto& GMIndex<FormatT,Indices...>::format() const
    {
	if constexpr(std::is_same<FormatT,None>::value){
	    return mLexFormat;
	}
	else {
	    return mFormat;
	}
    }

    template <class FormatT, class... Indices>
    const auto& GMIndex<FormatT,Indices...>::lexFormat() const
    {
	return mLexFormat;
    }

    template <class FormatT, class... Indices>
    GMIndex<FormatT,Indices...>& GMIndex<FormatT,Indices...>::setFormat(const FormatT& bs)
    {
	if constexpr(not std::is_same<FormatT,None>::value){
	    mFormat = bs;
	}
	return *this;
    }

    template <class BT1, class BT2, class... Indices>
    decltype(auto) replaceFormat(const BT1& bs1, const Sptr<GMIndex<BT2,Indices...>>& gmi)
    {
	return iter<0,sizeof...(Indices)>
	    ( [&](auto i) { return std::get<i>(gmi->pack()); },
	      [&](const auto&... e) { return std::make_shared<GMIndex<BT1,Indices...>>
		    ( bs1, e... ); } );
    }

    template <class BT1, class... Is1, class BT2, class... Is2>
    decltype(auto) operator*(const Sptr<GMIndex<BT1,Is1...>>& a, const Sptr<GMIndex<BT2,Is2...>>& b)
    {
	return iptrMul(a, b);
    }

    template <class... Indices>
    constexpr decltype(auto) mindex(const Sptr<Indices>&... is)
    {
	return MIndex<Indices...>(is...);
    }    

    template <class FormatT, class... Indices>
    constexpr decltype(auto) gmindexPtr(const FormatT& bs, const Sptr<Indices>&... is)
    {
	return std::make_shared<GMIndex<FormatT,Indices...>>(bs, is...);
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
