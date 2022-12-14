
#ifndef __cxz_mrange_cc_h__
#define __cxz_mrange_cc_h__

#include "mrange.h"

namespace CNORXZ
{
    /************************
     *   GMIndex (private)  *
     ************************/

    template <class BlockType, class... Indices>
    template <SizeT... Is>
    constexpr decltype(auto) GMIndex<BlockType,Indices...>::mkIPack(Isq<Is...> is) const
    {
	static_assert(sizeof...(Is) == NI,
		      "sequence sioze does not match number of indices");
	CXZ_ASSERT( ( (mRange->sub(Is) != nullptr) and ... ),
		    "subranges not available" );
	return std::make_tuple( std::make_shared<Indices>( mRange->sub(Is) )... );
    }

    template <class BlockType, class... Indices>
    constexpr decltype(auto) GMIndex<BlockType,Indices...>::mkLMax(const IndexPack& ipack)
    {
	return iter<0,NI>( [&](auto i) { return std::get<i>(ipack)->lmax(); },
			   [](auto... e) { return (e * ...); });
    }

    template <class BlockType, class... Indices>
    constexpr decltype(auto) GMIndex<BlockType,Indices...>::mkPMax(const IndexPack& ipack, const BlockType& blockSizes)
    {
	if constexpr(std::is_same<BlockType,None>::value){
	    return mkLMax(ipack);
	}
	else {
	    return iter<0,NI>
		( [&](auto i)
		{ return (std::get<i>(ipack)->pmax() - SPos<1>()) * std::get<i>(blockSizes); },
		    [](auto... e) { return (e + ...); }) + SPos<1>();
	}
    }

    template <class BlockType, class... Indices>
    inline void GMIndex<BlockType,Indices...>::mkPos()
    {
	mLex = iter<0,NI>
	    ([&](auto i) { return std::get<i>(mIPack)->lex() * std::get<i>(mLexBlockSizes).val(); },
	     [](const auto&... e) { return (e + ...); });
	if constexpr(not std::is_same<BlockType,None>::value){
	    IB::mPos = iter<0,NI>
		([&](auto i) { return std::get<i>(mIPack)->pos() * std::get<i>(mBlockSizes).val(); },
		 [](const auto&... e) { return (e + ...); });
	}
	else {
	    IB::mPos = mLex;
	}
    }

    template <class BlockType, class... Indices>
    template <SizeT... Is>
    constexpr decltype(auto) GMIndex<BlockType,Indices...>::mkLexBlockSizes(const IndexPack& ipack, Isq<Is...> is)
    {
	return std::make_tuple
	    ( iter<Is,NI>
	      ( [&](auto i) { return std::get<i>(ipack)->pmax(); },
		[](const auto&... as) { return (as * ...); } )...,
	      SPos<1>() );
    }

    template <class BlockType, class... Indices>
    template <SizeT I>
    inline void GMIndex<BlockType,Indices...>::up()
    {
	auto& i = std::get<I>(mIPack);
	if constexpr(I != 0){
	    if(i->lex() == i->lmax().val()-1){
		IB::mPos -= std::get<I>(blockSizes()).val() * i->pos();
		if constexpr(not std::is_same<BlockType,None>::value){
		    mLex -= std::get<I>(lexBlockSizes()).val() * i->lex();
		}
		(*i) = 0;
		up<I-1>();
		return;
	    }
	}
	IB::mPos += std::get<I>(blockSizes()).val();
	if constexpr(not std::is_same<BlockType,None>::value){
	    mLex += std::get<I>(lexBlockSizes()).val();
	}
	++(*i);
    }

    template <class BlockType, class... Indices>
    template <SizeT I>
    inline void GMIndex<BlockType,Indices...>::down()
    {
	auto& i = std::get<I>(mIPack);
	if constexpr(I != 0){
	    if(i->lex() == 0){
		(*i) = i->lmax().val()-1;
		IB::mPos += std::get<I>(blockSizes()).val() * i->pos();
		if constexpr(not std::is_same<BlockType,None>::value){
		    mLex += std::get<I>(lexBlockSizes()).val() * i->lex();
		}
		down<I-1>();
		return;
	    }
	}
	IB::mPos -= std::get<I>(blockSizes()).val();
	if constexpr(not std::is_same<BlockType,None>::value){
	    mLex -= std::get<I>(lexBlockSizes()).val();
	}
	--(*i);
    }

    template <class BlockType, class... Indices>
    template <SizeT I, class Xpr, class F>
    constexpr decltype(auto) GMIndex<BlockType,Indices...>::mkIFor(const Xpr& xpr, F&& f) const
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

    template <class BlockType, class... Indices>
    constexpr GMIndex<BlockType,Indices...>::GMIndex(const GMIndex& i) :
	IndexInterface<GMIndex<BlockType,Indices...>,Tuple<typename Indices::MetaType...>>(0),
	mRange(rangeCast<RangeType>(i.range())),
	mIPack(mkIPack(Isqr<0,NI>{})),
	mLexBlockSizes(mkLexBlockSizes(mIPack,Isqr<1,NI>{})),
	mBlockSizes(i.mBlockSizes),
	mLMax(mkLMax(mIPack)),
	mPMax(mkPMax(mIPack,mBlockSizes))
    {
	*this = i.lex();
    }

    template <class BlockType, class... Indices>
    constexpr GMIndex<BlockType,Indices...>& GMIndex<BlockType,Indices...>::operator=(const GMIndex& i)
    {
	IndexInterface<GMIndex<BlockType,Indices...>,Tuple<typename Indices::MetaType...>>::operator=(0);
	mRange = rangeCast<RangeType>(i.range());
	mIPack = mkIPack(Isqr<0,NI>{});
	mLexBlockSizes = mkLexBlockSizes(mIPack,Isqr<1,NI>{});
	mBlockSizes = i.mBlockSizes;
	mLMax = mkLMax(mIPack);
	mPMax = mkPMax(mIPack,mBlockSizes);
	return *this = i.lex();
    }

    template <class BlockType, class... Indices>
    constexpr GMIndex<BlockType,Indices...>::GMIndex(const Indices&... is) :
	IndexInterface<GMIndex<BlockType,Indices...>,Tuple<typename Indices::MetaType...>>(0),
	mRange(std::dynamic_pointer_cast<RangeType>(mrange(is.range()...))),
	mIPack(std::make_shared<Indices>(is)...),
	mLexBlockSizes(mkLexBlockSizes(mIPack,Isqr<1,NI>{})),
	mBlockSizes(),
	mLMax(mkLMax(mIPack)),
	mPMax(mkPMax(mIPack,mBlockSizes))
    {
	mkPos();
    }

    template <class BlockType, class... Indices>
    constexpr GMIndex<BlockType,Indices...>::GMIndex(const BlockType& bs, const Indices&... is) :
	IndexInterface<GMIndex<BlockType,Indices...>,Tuple<typename Indices::MetaType...>>(0),
	mRange(std::dynamic_pointer_cast<RangeType>(mrange(is.range()...))),
	mIPack(std::make_shared<Indices>(is)...),
	mLexBlockSizes(mkLexBlockSizes(mIPack,Isqr<1,NI>{})),
	mBlockSizes(bs),
	mLMax(mkLMax(mIPack)),
	mPMax(mkPMax(mIPack,mBlockSizes))
    {
	mkPos();
    }

    template <class BlockType, class... Indices>
    constexpr GMIndex<BlockType,Indices...>::GMIndex(const Sptr<Indices>&... is) :
	IndexInterface<GMIndex<BlockType,Indices...>,Tuple<typename Indices::MetaType...>>(0),
	mRange(std::dynamic_pointer_cast<RangeType>(mrange(is->range()...))),
	mIPack(is...),
	mLexBlockSizes(mkLexBlockSizes(mIPack,Isqr<1,NI>{})),
	mBlockSizes(),
	mLMax(mkLMax(mIPack)),
	mPMax(mkPMax(mIPack,mBlockSizes))
    {
	mkPos();
    }

    template <class BlockType, class... Indices>
    constexpr GMIndex<BlockType,Indices...>::GMIndex(const BlockType& bs,
						     const Sptr<Indices>&... is) :
	IndexInterface<GMIndex<BlockType,Indices...>,Tuple<typename Indices::MetaType...>>(0),
	mRange(std::dynamic_pointer_cast<RangeType>(mrange(is->range()...))),
	mIPack(is...),
	mLexBlockSizes(mkLexBlockSizes(mIPack,Isqr<1,NI>{})),
	mBlockSizes(bs),
	mLMax(mkLMax(mIPack)),
	mPMax(mkPMax(mIPack,mBlockSizes))
    {
	mkPos();
    }

    template <class BlockType, class... Indices>
    constexpr GMIndex<BlockType,Indices...>::GMIndex(const RangePtr& range, SizeT lexpos) :
	IndexInterface<GMIndex<BlockType,Indices...>,Tuple<typename Indices::MetaType...>>(0),
	mRange(rangeCast<RangeType>(range)),
	mIPack(mkIPack(Isqr<0,NI>{})),
	mLexBlockSizes(mkLexBlockSizes(mIPack,Isqr<1,NI>{})),
	mBlockSizes(),
	mLMax(mkLMax(mIPack)),
	mPMax(mkPMax(mIPack,mBlockSizes))
    {
	*this = lexpos;
    }

    template <class BlockType, class... Indices>
    constexpr GMIndex<BlockType,Indices...>::GMIndex(const RangePtr& range, const BlockType& blockSizes, SizeT lexpos) :
	IndexInterface<GMIndex<BlockType,Indices...>,Tuple<typename Indices::MetaType...>>(0),
	mRange(rangeCast<RangeType>(range)),
	mIPack(mkIPack(Isqr<0,NI>{})),
	mLexBlockSizes(mkLexBlockSizes(mIPack,Isqr<1,NI>{})),
	mBlockSizes(blockSizes),
	mLMax(mkLMax(mIPack)),
	mPMax(mkPMax(mIPack,mBlockSizes))
    {
	*this = lexpos;
    }

    template <class BlockType, class... Indices>
    GMIndex<BlockType,Indices...>& GMIndex<BlockType,Indices...>::operator=(SizeT lexpos)
    {
	if(lexpos >= lmax().val()){
	    if constexpr(not std::is_same<BlockType,None>::value){ mLex = lmax().val(); }
	    IB::mPos = pmax().val();
	    return *this;
	}
	if constexpr(not std::is_same<BlockType,None>::value){ mLex = lexpos; }
	IB::mPos = iter<0,NI>( [&](auto i) {
	    *std::get<i>(mIPack) = (lex() / std::get<i>(lexBlockSizes()).val()) % std::get<i>(mIPack)->lmax().val();
	    return std::get<i>(blockSizes()).val() * std::get<i>(mIPack)->pos();
	}, [](const auto&... e) { return (e + ...); } );
	return *this;
    }

    template <class BlockType, class... Indices>
    GMIndex<BlockType,Indices...>& GMIndex<BlockType,Indices...>::operator++()
    {
	if(lex() == lmax().val()-1){
	    return *this = lmax().val();
	}
	if(lex() != lmax().val()){
	    up<NI-1>();
	}
	return *this;
    }

    template <class BlockType, class... Indices>
    GMIndex<BlockType,Indices...>& GMIndex<BlockType,Indices...>::operator--()
    {
	if(lex() == lmax().val()){
	    return *this = lmax().val()-1;
	}
	if(lex() != 0){
	    down<NI-1>();
	}
	return *this;
    }

    template <class BlockType, class... Indices>
    GMIndex<BlockType,Indices...> GMIndex<BlockType,Indices...>::operator+(Int n) const
    {
	GMIndex o(*this);
	return o += n;
    }

    template <class BlockType, class... Indices>
    GMIndex<BlockType,Indices...> GMIndex<BlockType,Indices...>::operator-(Int n) const
    {
	GMIndex o(*this);
	return o -= n;
    }

    template <class BlockType, class... Indices>
    GMIndex<BlockType,Indices...>& GMIndex<BlockType,Indices...>::operator+=(Int n)
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

    template <class BlockType, class... Indices>
    GMIndex<BlockType,Indices...>& GMIndex<BlockType,Indices...>::operator-=(Int n)
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

    template <class BlockType, class... Indices>
    SizeT GMIndex<BlockType,Indices...>::lex() const
    {
	if constexpr(std::is_same<BlockType,None>::value){
	    return IB::mPos;
	}
	else {
	    return mLex;
	}
    }
    
    template <class BlockType, class... Indices>
    constexpr decltype(auto) GMIndex<BlockType,Indices...>::pmax() const
    {
	return mPMax;
    }

    template <class BlockType, class... Indices>
    constexpr decltype(auto) GMIndex<BlockType,Indices...>::lmax() const
    {
	return mPMax;
    }

    template <class BlockType, class... Indices>
    IndexId<0> GMIndex<BlockType,Indices...>::id() const
    {
	return IndexId<0>(this->ptrId());
    }

    template <class BlockType, class... Indices>
    typename GMIndex<BlockType,Indices...>::MetaType GMIndex<BlockType,Indices...>::operator*() const
    {
	return meta();
    }
    
    template <class BlockType, class... Indices>	
    constexpr SizeT GMIndex<BlockType,Indices...>::dim() const
    {
	return NI;
    }	

    template <class BlockType, class... Indices>	
    Sptr<typename GMIndex<BlockType,Indices...>::RangeType> GMIndex<BlockType,Indices...>::range() const
    {
	return mRange;
    }

    template <class BlockType, class... Indices>	
    template <SizeT I>
    decltype(auto) GMIndex<BlockType,Indices...>::stepSize(const IndexId<I>& id) const
    {
	return iter<0,NI>
	    ( [&](auto i) { return std::get<i>(mIPack)->stepSize(id) * std::get<i>(blockSizes()); },
	      [](const auto&... ss) { return ( ss + ... ); });
    }

    template <class BlockType, class... Indices>
    String GMIndex<BlockType,Indices...>::stringMeta() const
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
    
    template <class BlockType, class... Indices>
    typename GMIndex<BlockType,Indices...>::MetaType GMIndex<BlockType,Indices...>::meta() const
    {
	return iter<0,NI>( [&](auto i) { return std::get<i>(mIPack)->meta(); },
			   [](const auto&... xs) { return std::make_tuple(xs...); } );
    }

    template <class BlockType, class... Indices>
    GMIndex<BlockType,Indices...>& GMIndex<BlockType,Indices...>::at(const MetaType& metaPos)
    {
	iter<0,NI>( [&](auto i) { std::get<i>(mIPack)->at( std::get<i>(metaPos) ); }, NoF {} );
	IB::mPos = iter<0,NI>
	    ( [&](auto i) { return std::get<i>(mIPack)->pos()*std::get<i>(blockSizes()).val(); },
	      [](const auto&... xs) { return (xs + ...); });
	return *this;
    }

    template <class BlockType, class... Indices>	
    template <class Xpr, class F>
    constexpr decltype(auto) GMIndex<BlockType,Indices...>::ifor(const Xpr& xpr, F&& f) const
    {
	return mkIFor<0>(xpr, std::forward<F>(f));
    }

    template <class BlockType, class... Indices>
    GMIndex<BlockType,Indices...>& GMIndex<BlockType,Indices...>::operator()(const Sptr<MIndex<Indices...>>& mi)
    {
	mIPack = mi.pack();
	mkPos();
	return *this;
    }

    template <class BlockType, class... Indices>
    GMIndex<BlockType,Indices...>& GMIndex<BlockType,Indices...>::operator()()
    {
	mkPos();
	return *this;
    }

    template <class BlockType, class... Indices>
    const typename GMIndex<BlockType,Indices...>::IndexPack& GMIndex<BlockType,Indices...>::pack() const
    {
	return mIPack;
    }

    template <class BlockType, class... Indices>
    template <class Index, class F, class G>
    constexpr decltype(auto) GMIndex<BlockType,Indices...>::zip(const Index& ind, const F& f, const G& g) const
    {
	static_assert(is_index<Index>::value, "got non-index type");
	if constexpr(has_static_sub<Index>::value){
	    static_assert(index_dim<Index>::value == NI,
			  "got static-dimensional index with wrong dimension");
	    if constexpr(std::is_same<G,NoF>::value or std::is_same<F,NoF>::value){
		iter<0,NI>( [&](auto i) { return std::get<i>(mIPack)->zip(*std::get<i>(ind.pack()), f); },
			    NoF {} );
		f(*this, ind);
		return;
	    }
	    else {
		return iter<0,NI>( [&](auto i) { return std::get<i>(mIPack)->zip(*std::get<i>(ind.pack()), f); },
				   [](auto... e) { return g(std::make_tuple(e...), f(*this, ind)); } );
	    }
	}
	else if constexpr(has_sub<Index>::value){
	    CXZ_ASSERT(ind.dim() == NI, "got index with wrong dimension = " << ind.dim()
		       << ", expected: " << NI);
	    if constexpr(std::is_same<G,NoF>::value or std::is_same<F,NoF>::value){
		iter<0,NI>( [&](auto i) { return std::get<i>(mIPack)->zip(*ind.pack()[i],f); },
			    NoF {} );
		f(*this, ind);
		return;
	    }
	    else {
		return iter<0,NI>( [&](auto i) { return std::get<i>(mIPack)->zip(*ind.pack()[i],f); },
				   [](auto... e) { return g(std::make_tuple(e...), f(*this, ind)); } );
	    }
	}
	else {
	    if constexpr(std::is_same<F,NoF>::value){
		f(*this, ind);
		return;
	    }
	    else {
		return f(*this, ind);
	    }
	}
    }

    template <class BlockType, class... Indices>
    const auto& GMIndex<BlockType,Indices...>::blockSizes() const
    {
	if constexpr(std::is_same<BlockType,None>::value){
	    return mLexBlockSizes;
	}
	else {
	    return mBlockSizes;
	}
    }

    template <class BlockType, class... Indices>
    const auto& GMIndex<BlockType,Indices...>::lexBlockSizes() const
    {
	return mLexBlockSizes;
    }

    template <class BlockType, class... Indices>
    GMIndex& GMIndex<BlockType,Indices...>::setBlockSizes(const BlockType& bs)
    {
	if constexpr(not std::is_same<BlockType,None>::value){
	    mBlockSizes = bs;
	}
	return *this;
    }

    template <class BT1, class BT2, class... Indices>
    decltype(auto) replaceBlockSize(const BT1& bs1, const Sptr<GMIndex<BT2,Indices...>>& gmi)
    {
	return iter<0,sizeof...(Indices)>
	    ( [&](auto i) { return std::get<i>(gmi.pack()); },
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
