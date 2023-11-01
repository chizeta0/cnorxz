
#ifndef __cxz_mrange_cc_h__
#define __cxz_mrange_cc_h__

#include "mrange.h"
#include "operation/op_types.h"
#include "operation/op_types.cc.h" // -> do .cc files
#include "operation/op_utility.cc.h" // -> do .cc files
#include "array/marray.h"

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
    constexpr decltype(auto) GMIndex<FormatT,Indices...>::mkLMax(const SPack<Indices...>& ipack)
    {
	return iter<0,NI>( [&](auto i) { return ipack[i]->lmax(); },
			   [](auto... e) { return (e * ...); });
    }

    template <class FormatT, class... Indices>
    constexpr decltype(auto) GMIndex<FormatT,Indices...>::mkPMax(const SPack<Indices...>& ipack, const FormatT& format)
    {
	if constexpr(std::is_same<FormatT,None>::value){
	    return mkLMax(ipack);
	}
	else {
	    return iter<0,NI>
		( [&](auto i)
		{ return (ipack[i]->pmax() - SPos<1>()) * format[i]; },
		    [](auto... e) { return (e + ...); }) + SPos<1>();
	}
    }

    template <class FormatT, class... Indices>
    inline void GMIndex<FormatT,Indices...>::mkPos()
    {
	mLex = iter<0,NI>
	    ([&](auto i) { return mIPack[i]->lex() * mLexFormat[i].val(); },
	     [](const auto&... e) { return (e + ...); });
	if constexpr(not std::is_same<FormatT,None>::value){
	    IB::mPos = iter<0,NI>
		([&](auto i) { return mIPack[i]->pos() * mFormat[i].val(); },
		 [](const auto&... e) { return (e + ...); });
	}
	else {
	    IB::mPos = mLex;
	}
    }

    template <class FormatT, class... Indices>
    template <SizeT... Is>
    constexpr decltype(auto) GMIndex<FormatT,Indices...>::mkLexFormat(const SPack<Indices...>& ipack, Isq<Is...> is)
    {
	return gmformat
	    ( iter<Is,NI>
	      ( [&](auto i) { return ipack[i]->pmax(); },
		[](const auto&... as) { return (as * ...); } )...,
	      SPos<1>() );
    }

    template <class FormatT, class... Indices>
    template <SizeT I>
    inline void GMIndex<FormatT,Indices...>::up()
    {
	constexpr CSizeT<I> i;
	auto& ind = mIPack[i];
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
	constexpr CSizeT<I> i;
	auto& ind = mIPack[i];
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
	constexpr CSizeT<I> i;
	if constexpr(I == sizeof...(Indices)-1){
	    return mIPack[i]->ifor(xpr,std::forward<F>(f));
	}
	else {
	    auto f1 = f;
	    auto f2 = f1;
	    return mIPack[i]->ifor( mkIFor<I+1>( xpr, std::move(f1) ), std::move(f2) );
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
    constexpr GMIndex<FormatT,Indices...>::GMIndex(const SPack<Indices...>& pack) :
	IndexInterface<GMIndex<FormatT,Indices...>,Tuple<typename Indices::MetaType...>>(0),
	mRange(std::dynamic_pointer_cast<RangeType>(pack.mkRange())),
	mIPack(pack),
	mLexFormat(mkLexFormat(mIPack,Isqr<1,NI>{})),
	mFormat(),
	mLMax(mkLMax(mIPack)),
	mPMax(mkPMax(mIPack,mFormat))
    {
	mkPos();
    }

    template <class FormatT, class... Indices>
    constexpr GMIndex<FormatT,Indices...>::GMIndex(const FormatT& bs, const SPack<Indices...>& pack) :
	IndexInterface<GMIndex<FormatT,Indices...>,Tuple<typename Indices::MetaType...>>(0),
	mRange(std::dynamic_pointer_cast<RangeType>(pack.mkRange())),
	mIPack(pack),
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
	    *mIPack[i] = (lexpos / lexFormat()[i].val()) % mIPack[i]->lmax().val();
	    return format()[i].val() * mIPack[i]->pos();
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
    SizeT GMIndex<FormatT,Indices...>::operator-(const GMIndex& i) const
    {
	return lex() - i.lex();
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
	    ( [&](auto i) { return mIPack[i]->stepSize(id) * format()[i]; },
	      [](const auto&... ss) { return ( ss + ... ); });
    }
    /*
    template <class FormatT, class... Indices>
    template <class Index>
    decltype(auto) GMIndex<FormatT,Indices...>::formatFrom(const Index& ind) const
    {
	static_assert(is_index<Index>::value, "got non-index");
	CXZ_ASSERT(ind.dim() >= dim(), "for formatting index of dimension " << dim()
		   << " need index of at least the same dimension, got " << ind.dim());
	return *this;
    }
    */
    template <class FormatT, class... Indices>
    String GMIndex<FormatT,Indices...>::stringMeta() const
    {
	const String blim = "(";
	const String elim = ")";
	const String dlim = ",";
	return iter<1,NI>
	    ( [&](auto i) { return mIPack[i]->stringMeta(); },
	      [&](const auto&... xs) {
		  return blim + mIPack[CSizeT<0>{}]->stringMeta() + ( (dlim + xs) + ... ) + elim;
	      } );
    }
    
    template <class FormatT, class... Indices>
    typename GMIndex<FormatT,Indices...>::MetaType GMIndex<FormatT,Indices...>::meta() const
    {
	return iter<0,NI>( [&](auto i) { return mIPack[i]->meta(); },
			   [](const auto&... xs) { return std::make_tuple(xs...); } );
    }

    template <class FormatT, class... Indices>
    GMIndex<FormatT,Indices...>& GMIndex<FormatT,Indices...>::at(const MetaType& metaPos)
    {
	iter<0,NI>( [&](auto i) { mIPack[i]->at( std::get<i>(metaPos) ); }, NoF {} );
	IB::mPos = iter<0,NI>
	    ( [&](auto i) { return mIPack[i]->pos()*format()[i].val(); },
	      [](const auto&... xs) { return (xs + ...); });
	return *this;
    }

    template <class FormatT, class... Indices>	
    decltype(auto) GMIndex<FormatT,Indices...>::xpr(const Sptr<MIndex<Indices...>>& _this) const
    {
	return iter<0,NI>( [&](auto i) { return _this->pack()[i]->xpr( _this->pack()[i] ); },
			   [](const auto&... x) {
			       return operation( [](const auto&... e) {
				   return std::make_tuple(e...);
			       }, x... );
			   } );
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
    const SPack<Indices...>& GMIndex<FormatT,Indices...>::pack() const
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
    RangePtr GMIndex<FormatT,Indices...>::prange(const MIndex<Indices...>& last) const
    {
	return iter<0,NI>
	    ( [&](auto i) {
		typedef typename std::remove_reference<decltype(*mIPack[i])>::type::RangeType RangeT;
		return std::dynamic_pointer_cast<RangeT>( mIPack[i]->prange( *last.pack()[i] ) );
	        },
		[](const auto&... e) { return mrange(e...); }
	    );
	
    }

    template <class FormatT, class... Indices>
    auto GMIndex<FormatT,Indices...>::deepFormat() const
    {
	return iter<0,NI>( [&](auto i) { return mul(mIPack[i]->deepFormat(), format()[i].val()); },
			   [&](const auto&... e) { return concat(e...); } );
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
	    ( [&](auto i) { return gmi->pack()[CSizeT<i>{}]; },
	      [&](const auto&... e) { return std::make_shared<GMIndex<BT1,Indices...>>
		    ( bs1, e... ); } );
    }

    template <class... Indices>
    constexpr decltype(auto) mindex(const Sptr<Indices>&... is)
    {
	return MIndex<Indices...>(is...);
    }

    template <class... Indices>
    constexpr decltype(auto) mindex(const SPack<Indices...>& pack)
    {
	return MIndex<Indices...>(pack);
    }

    template <class... Indices>
    constexpr decltype(auto) mindexPtr(const SPack<Indices...>& pack)
    {
	return std::make_shared<MIndex<Indices...>>(pack);
    }

    template <class FormatT, class... Indices>
    constexpr decltype(auto) gmindexPtr(const FormatT& bs, const SPack<Indices...>& pack)
    {
	return std::make_shared<GMIndex<FormatT,Indices...>>(bs, pack);
    }    
    
    template <class I1, class FormatT, class... Indices>
    decltype(auto) operator*(const Sptr<GMIndex<FormatT,Indices...>>& a, const Sptr<I1>& b)
    {
	return iptrMul(a, b);
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
	Vector<Uuid> key = iter<0,sizeof...(Ranges)>
	    ( [&](auto i) { return std::get<i>( mRs ); },
	      [](const auto&... e) { return Vector<Uuid> { e->id()... }; } );
	const auto& info = typeid(MRange<Ranges...>);
	mProd = this->fromCreated(info, key);
	if(mProd == nullptr) {
	    mProd = std::shared_ptr<MRange<Ranges...>>
		( new MRange<Ranges...>( mRs ) );
	    this->addToCreated(info, key, mProd);
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
    MArray<RangePtr> MRange<Ranges...>::sub() const
    {
	// TODO: ZRange (meta and index pos static!)
	if constexpr(NR == 0) {
	    return MArray<RangePtr>();
	}
	else {
	    RangePtr sr = SRangeFactory<SizeT,NR>
		( iter<0,NR>
		  ( [](auto i) { return i; },
		    [](auto... x) { return Arr<SizeT,NR> { x... }; } ) ).create();
	    return MArray<RangePtr>
		( sr, iter<0,NR>
		  ( [&](auto i) { return std::get<i>(mRs); },
		    [](auto... x) { return Vector<RangePtr> ({ x... }); } ) );
	}
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

    template <class... Ranges>
    RangePtr MRange<Ranges...>::extend(const RangePtr& r) const
    {
	CXZ_ASSERT( r->dim() == this->dim(), "cannot extend range of dimension "
		    << this->dim() << " by range of dimension " << r->dim());
	auto rs = iter<0,NR>( [&](auto i) {
	    typedef typename std::remove_reference<decltype(*std::get<i>(mRs))>::type RType;
	    return std::dynamic_pointer_cast<RType>( std::get<i>(mRs)->extend(r->sub(i)) ); },
	    [](const auto&... e) { return std::make_tuple(e...); }
	    );
	return MRangeFactory<Ranges...>( rs ).create();
    }

    template <class... Ranges>
    Vector<Uuid> MRange<Ranges...>::key() const
    {
    	Vector<Uuid> k = iter<0,sizeof...(Ranges)>
	    ( [&](auto i) { return std::get<i>( mRs ); },
	      [](const auto&... e) { return Vector<Uuid> { e->id()... }; } );
	return k;
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
    
    template <class... Ranges>
    Sptr<MRange<Ranges...>> RangeCast<MRange<Ranges...>>::func(const RangePtr& r)
    {
	CXZ_ASSERT(r->dim() == sizeof...(Ranges), "expect range of dimension "
		   << sizeof...(Ranges) << ", got " << r->dim());
	return std::dynamic_pointer_cast<MRange<Ranges...>>( MRangeFactory
	    ( iter<0,sizeof...(Ranges)>
	      ( [&](auto i) {
		  return rangeCast<typename std::tuple_element<i,Tuple<Ranges...>>::type> (r->sub(i)); },
		  [](const auto&... e){ return std::make_tuple( e... ); } ) ).create() );
    }
}

#endif
