// -*- C++ -*-
/**
   
   @file lib/ranges/yrange.cc
   @brief YRange and YIndex implementations.

   Copyright (c) 2022 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#include "ranges/ranges.h"
#include "array/array.h"

namespace CNORXZ
{
    /*=======================+
     |   YIndex (private)    |
     +=======================*/

    inline DPack YIndex::mkIndices() const
    {
	Vector<XIndexPtr> o(mRange->dim(), nullptr);
	for(SizeT i = 0; i != mRange->dim(); ++i){
	    auto rp = mRange->sub(i);
	    CXZ_ASSERT(rp != nullptr, "subranges not available");
	    o[i] = rp->begin().xptr();
	}
	return DPack(std::move(o));
    }
    
    inline Vector<SizeT> YIndex::mkFormat() const
    {
	Vector<SizeT> o(mIs.size());
	SizeT b = 1;
	for(SizeT i = o.size(); i != 0; --i){
	    const SizeT j = i-1;
	    o[j] = b;
	    b *= mIs[j]->pmax().val();
	}
	return o;
    }

    inline Vector<SizeT> YIndex::mkLexFormat() const
    {
	Vector<SizeT> o(mIs.size());
	SizeT b = 1;
	for(SizeT i = o.size(); i != 0; --i){
	    const SizeT j = i-1;
	    o[j] = b;
	    b *= mIs[j]->lmax().val();
	}
	return o;
    }

    inline Vector<RangePtr> YIndex::mkRangeVec(const Vector<XIndexPtr>& is) const
    {
	Vector<RangePtr> o(is.size());
	std::transform(is.begin(), is.end(), o.begin(), [](const auto& e) { return e->range(); });
	return o;
    }

    inline void YIndex::mkPos()
    {
	mLex = 0;
	IB::mPos = 0;
	bool outOfScope = false;
	for(SizeT i = 0; i != dim(); ++i){
	    outOfScope |= mIs[i]->lex() >= mIs[i]->lmax().val();
	    mLex += mIs[i]->lex() * mLexFormat[i].val();
	    IB::mPos += mIs[i]->pos() * mFormat[i].val();
	}
	if(outOfScope){
	    IB::mPos = pmax().val();
	    mLex = lmax().val();
	}
    }
    
    inline void YIndex::up(SizeT i)
    {
	auto& idx = mIs[i];
	// it is guaranteed that the last accessible position 
	// is one less than the max position (=end)
	if(i != 0 and idx->lex() == idx->lmax().val()-1){
	    IB::mPos -= mFormat[i].val() * idx->pos();
	    mLex -= mLexFormat[i].val() * idx->lex();
	    (*idx) = 0;
	    up(i-1);
	    return;
	}
	IB::mPos += mFormat[i].val();
	mLex += mLexFormat[i].val();
	++(*idx);
    }
    
    inline void YIndex::down(SizeT i)
    {
	auto& idx = mIs[i];
	if(i != 0 and idx->pos() == 0){
	    (*idx) = idx->lmax().val()-1;
	    IB::mPos += mFormat[i].val() * idx->pos();
	    mLex += mLexFormat[i].val() * idx->lex();
	    down(i-1);
	    return;
	}
	IB::mPos -= mFormat[i].val();
	mLex -= mLexFormat[i].val();
	--(*idx);
    }
    
    inline decltype(auto) YIndex::mkIFor(SizeT i, const DXpr<None>& xpr, NoF&& f) const
    {
	if(i == mIs.size()-1){
	    return mIs[i]->ifor( xpr, std::forward<NoF>(f) );
	}
	else {
	    auto f1 = f;
	    auto f2 = f1;
	    return mIs[i]->ifor( mkIFor(i+1, xpr, std::move(f1)), std::move(f2) );
	}
    }

    inline SizeT YIndex::mkPMax() const
    {
	SizeT o = 0;
	for(SizeT i = 0; i != mIs.size(); ++i){
	    o += (mIs[i]->pmax().val()-1) * mFormat[i].val();
	}
	return o+1;
    }
    
    inline SizeT YIndex::mkLMax() const
    {
	return std::accumulate(mIs.all().begin(), mIs.all().end(),1,
			       [](const auto& res, const auto& el) { return res * el->lmax().val(); } );
    }
    
    
    /*=============+
     |   YIndex    |
     +=============*/

    YIndex::YIndex(const YIndex& i) :
	IndexInterface<YIndex,Vector<DType>>(i),
	mRange(rangeCast<YRange>(i.range())),
	mIs(mkIndices()),
	mFormat(i.mFormat),
	mLexFormat(i.mLexFormat),
	mPMax(mkPMax()),
	mLMax(mkLMax())
    {
	*this = i.lex();
    }
    
    YIndex& YIndex::operator=(const YIndex& i)
    {
	IndexInterface<YIndex,Vector<DType>>::operator=(i);
	mRange = rangeCast<YRange>(i.range());
	mIs = mkIndices();
	mFormat = i.mFormat;
	mLexFormat = i.mLexFormat;
	mPMax = mkPMax();
	mLMax = mkLMax();
	return *this = i.lex();
    }

    YIndex::YIndex(const Vector<XIndexPtr>& is) :
	IndexInterface<YIndex,Vector<DType>>(0),
	mRange(std::dynamic_pointer_cast<YRange>(yrange(mkRangeVec(is)))),
	mIs(is),
	mFormat(mkFormat()),
	mLexFormat(mkLexFormat()),
	mPMax(mkPMax()),
	mLMax(mkLMax())
    {
	mkPos();
    }
	
    YIndex::YIndex(const YFormat& bs, const Vector<XIndexPtr>& is) :
	IndexInterface<YIndex,Vector<DType>>(0),
	mRange(std::dynamic_pointer_cast<YRange>(yrange(mkRangeVec(is)))),
	mIs(is),
	mFormat(bs),
	mLexFormat(mkLexFormat()),
	mPMax(mkPMax()),
	mLMax(mkLMax())
    {
	mkPos();
    }

    YIndex::YIndex(const RangePtr& range, SizeT lexpos) :
	IndexInterface<YIndex,Vector<DType>>(0),
	mRange(rangeCast<YRange>(range)),
	mIs(mkIndices()),
	mFormat(mkFormat()),
	mLexFormat(mkLexFormat()),
	mPMax(mkPMax()),
	mLMax(mkLMax())
    {
	*this = lexpos;
    }

    YIndex::YIndex(const RangePtr& range, const YFormat& bs, SizeT lexpos) :
	IndexInterface<YIndex,Vector<DType>>(0),
	mRange(rangeCast<YRange>(range)),
	mIs(mkIndices()),
	mFormat(bs),
	mLexFormat(mkLexFormat()),
	mPMax(mkPMax()),
	mLMax(mkLMax())
    {
	*this = lexpos;
    }

    YIndex& YIndex::operator=(SizeT lexpos)
    {
	if(lexpos >= lmax().val()){
	    mLex = lmax().val();
	    IB::mPos = pmax().val();
	    return *this;
	}
	mLex = lexpos;
	IB::mPos = 0;
	for(SizeT i = 0; i != mIs.size(); ++i){
	    *mIs[i] = (lex() / mLexFormat[i].val()) % mIs[i]->lmax().val();
	    IB::mPos += mFormat[i].val() * mIs[i]->pos();
	}
	return *this;
    }
    
    YIndex& YIndex::operator++()
    {
	if(lex() == lmax().val()-1){
	    return *this = lmax().val();
	}
	if(lex() != lmax().val()){
	    up(mIs.size()-1);
	}
	return *this;
    }
    
    YIndex& YIndex::operator--()
    {
	if(lex() == lmax().val()){
	    return *this = lmax().val()-1;
	}
	if(lex() != 0){
	    down(mIs.size()-1);
	}
	return *this;
    }
    
    YIndex YIndex::operator+(Int n) const
    {
	YIndex o(*this);
	return o += n;
    }
    
    YIndex YIndex::operator-(Int n) const
    {
	YIndex o(*this);
	return o -= n;
    }

    SizeT YIndex::operator-(const YIndex& i) const
    {
	return lex() - i.lex();
    }
    
    YIndex& YIndex::operator+=(Int n)
    {
	if(static_cast<Int>(lex()) + n < 0){
	    return *this = 0;
	}
	return *this = static_cast<Int>(lex()) + n;
    }
    
    YIndex& YIndex::operator-=(Int n)
    {
	if(static_cast<Int>(lex()) - n < 0){
	    return *this = 0;
	}
	return *this = static_cast<Int>(lex()) - n;
    }

    SizeT YIndex::lex() const
    {
	return mLex;
    }
    
    UPos YIndex::pmax() const
    {
	return mPMax;
    }

    UPos YIndex::lmax() const
    {
	return mLMax;
    }
    
    IndexId<0> YIndex::id() const
    {
	return IndexId<0>(this->ptrId());
    }

    Vector<DType> YIndex::operator*() const
    {
	return meta();
    }
    
    SizeT YIndex::dim() const
    {
	return mRange->dim();
    }

    Sptr<YRange> YIndex::range() const
    {
	return mRange;
    }
    
    UPos YIndex::stepSize(const IndexId<0> id) const
    {
	SizeT o = 0;
	for(SizeT i = 0; i != mIs.size(); ++i){
	    const auto u = mIs[i]->stepSize(id) * mFormat[i];
	    o += u.val();
	}
	return UPos(o);
    }

    String YIndex::stringMeta() const
    {
	const String blim = "[";
	const String elim = "]";
	const String dlim = ",";
	return blim +
	    std::accumulate(std::next(mIs.all().begin()), mIs.all().end(), mIs[0]->stringMeta(),
			    [&](const auto& s, const auto& e)
			    { return s + dlim + e->stringMeta(); } ) +
	    elim;
    }
   
    Vector<DType> YIndex::meta() const
    {
	Vector<DType> v(mIs.size());
	std::transform(mIs.all().begin(), mIs.all().end(), v.begin(),
		       [](const auto& x) { return x->meta(); });
	return v;
    }
    
    YIndex& YIndex::at(const Vector<DType>& meta)
    {
	CXZ_ASSERT(meta.size() == mIs.size(), "input meta size ("
		   << meta.size() << ") different from expected size ("
		   << mIs.size() << ")");
	for(SizeT i = 0; i != mIs.size(); ++i){
	    mIs[i]->at(meta[i]);
	}
	mkPos();
	return *this;
    }

    DXpr<None> YIndex::ifor(const DXpr<None>& xpr, NoF&& f) const
    {
	return mkIFor(0, xpr, std::forward<NoF>(f));
    }

    YIndex& YIndex::operator()(const Sptr<YIndex>& i)
    {
	mIs = i->pack();
	mkPos();
	return *this;
    }
    
    YIndex& YIndex::operator()()
    {
	mkPos();
	return *this;
    }
    
    const DPack& YIndex::pack() const
    {
	return mIs;
    }

    RangePtr YIndex::prange(const YIndex& last) const
    {
	CXZ_ASSERT(dim() == last.dim(), "end index has different number of dimensions ("
		   << last.dim() << ") than begin index (" << dim() << ")");
	Vector<RangePtr> v(dim());
	for(SizeT i = 0; i != dim(); ++i){
	    v[i] = mIs[i]->prange( last.pack()[i] );
	}
	return YRangeFactory(v).create();
    }

    Vector<SizeT> YIndex::deepFormat() const
    {
	Vector<Vector<SizeT>> dfv(mIs.size());
	SizeT osize = 0;
	for(SizeT j = 0; j != dfv.size(); ++j){
	    dfv[j] = mIs[j]->deepFormat();
	    std::for_each(dfv[j].begin(), dfv[j].end(),
	    		  [&](SizeT& x) { x *= mFormat[j].val(); } );
	    osize += dfv[j].size();
	}
	Vector<SizeT> o(osize);
	SizeT off = 0;
	for(SizeT j = 0; j != dfv.size(); ++j){
	    std::copy(dfv[j].begin(), dfv[j].end(), o.begin()+off);
	    off += dfv[j].size();
	}
	return o;
    }
    
    Vector<SizeT> YIndex::deepMax() const
    {
	Vector<Vector<SizeT>> dmv(mIs.size());
	SizeT osize = 0;
	for(SizeT j = 0; j != dmv.size(); ++j){
	    dmv[j] = mIs[j]->deepMax();
	    osize += dmv[j].size();
	}
	Vector<SizeT> o(osize);
	SizeT off = 0;
	for(SizeT j = 0; j != dmv.size(); ++j){
	    std::copy(dmv[j].begin(), dmv[j].end(), o.begin()+off);
	    off += dmv[j].size();
	}
	return o;
    }

    YIndex& YIndex::reformat(const Vector<SizeT>& f, const Vector<SizeT>& s)
    {
	// f: input format
	// s: input sizes
	CXZ_ASSERT(f.size() == s.size(), "input error: f.size() != s.size()");
	if(f.size() == 1){
	    CXZ_ASSERT(s[0] == lmax().val(), "got inconsistent size; expeected "
		       << lmax().val() << ", got " << s[0]);
	    return *this;
	}

	SizeT j = 0;
	SizeT j0 = 0;
	SizeT xi = 1;
	Vector<UPos> nformat;
	for(SizeT i = 0; i != dim(); ++i){
	    CXZ_ASSERT(j != s.size(), "incompatible index formats");
	    xi *= mIs[i]->lmax().val();
	    SizeT xj = s[j];
	    if(xi < xj) {
		CXZ_ERROR("reformating with lower-dimensional formats is not possible; use sub-indices instead");
		continue;
	    }
	    j0 = j;
	    while(xj < xi){
		++j;
		CXZ_ASSERT(j != s.size(), "incompatible index formats");
		xj *= s[j];
	    }
	    CXZ_ASSERT(xj == xi, "incompatible index formats");
	    xi = 1;
	    ++j;

	    Vector<SizeT> nf(j-j0);
	    Vector<SizeT> ns(j-j0);
	    std::copy(f.begin()+j0,f.begin()+j,nf.begin());
	    nformat[i] = *std::min_element(nf.begin(), nf.end());
	    std::for_each(nf.begin(), nf.end(), [&](SizeT& x)
	    { CXZ_ASSERT(x % nformat[i].val() == 0, "incompatible"); x /= nformat[i].val(); } );
	    std::copy(s.begin()+j0,s.begin()+j,ns.begin());
	    mIs[i]->reformat(nf,ns);
	}
	mFormat = YFormat(nformat);
	return *this;
    }

    bool YIndex::formatIsTrivial() const
    {
	for(SizeT i = 0; i != mIs.size(); ++i){
	    if(not mIs[i]->formatIsTrivial()){
		return false;
	    }
	}
	SizeT b = 1;
	for(SizeT i = mFormat.size(); i != 0; --i){
	    const SizeT j = i-1;
	    if(mFormat[j].val() != b){
		return false;
	    }
	    b *= mIs[j]->pmax().val();
	}
	return true;
    }
    
    const YFormat& YIndex::format() const
    {
	return mFormat;
    }
    
    const YFormat& YIndex::lexFormat() const
    {
	return mLexFormat;
    }

    YIndex& YIndex::setFormat(const YFormat& bs)
    {
	mFormat = bs;
	return *this;
    }

    YIndex& YIndex::setSub(SizeT ind, SizeT lex)
    {
	CXZ_ASSERT(ind < dim(), "got index number (" << ind << ") larger than dimension ("
		   << dim() << ")");
	auto& idx = mIs[ind];
	CXZ_ASSERT(lex < idx->lmax().val(), "tried to set sub-index position " << lex
		   << ", which is out of scope; maximum position in range is " << idx->lmax().val() );
	(*idx) = lex;
	(*this)();
	return *this;
    }


    /*==========================+
     |   non-member functions   |
     +==========================*/

    YIndex yindex(const DPack& pack)
    {
	return YIndex(pack.all());
    }

    YIndex yindex(const Vector<XIndexPtr>& is)
    {
	return YIndex(is);
    }

    Sptr<YIndex> yindexPtr(const DPack& pack)
    {
	return std::make_shared<YIndex>(pack.all());
    }

    Sptr<YIndex> yindexPtr(const Vector<XIndexPtr>& is)
    {
	return std::make_shared<YIndex>(is);
    }

    /*====================+
     |   YRangeFactory    |
     +====================*/

    YRangeFactory::YRangeFactory(const Vector<RangePtr>& rvec) :
	mRVec(rvec) {}
    
    YRangeFactory::YRangeFactory(Vector<RangePtr>&& rvec) :
	mRVec(std::forward<Vector<RangePtr>>(rvec)) {}
    
    YRangeFactory::YRangeFactory(const Vector<RangePtr>& rvec, const RangePtr& ref) :
	mRVec(rvec), mRef(ref) {}
    
    YRangeFactory::YRangeFactory(Vector<RangePtr>&& rvec, const RangePtr& ref) :
	mRVec(std::forward<Vector<RangePtr>>(rvec)), mRef(ref) {}
    
    void YRangeFactory::make()
    {
	Vector<Uuid> k(mRVec.size());
	std::transform(mRVec.begin(), mRVec.end(), k.begin(),
		       [&](const RangePtr& r) { return r->id(); } );
	mProd = this->fromCreated(typeid(YRange), k);
	if(mProd == nullptr){
	    mProd = std::shared_ptr<YRange>
		( new YRange( std::move(mRVec) ) );
	    this->addToCreated(typeid(YRange), k, mProd);
	}
    }

    /*=============+
     |   YRange    |
     +=============*/

    RangePtr YRange::sub(SizeT i) const
    {
	return mRVec[i];
    }

    MArray<RangePtr> YRange::sub() const
    {
	if(mRVec.size() == 0){
	    return MArray<RangePtr>();
	}
	return MArray<RangePtr>( CRangeFactory(mRVec.size()).create(), mRVec );
    }

    SizeT YRange::size() const
    {
	SizeT out = 1;
	for(auto& r: mRVec){
	    out *= r->size();
	}
	return out;
    }
    
    SizeT YRange::dim() const
    {
	return mRVec.size();
    }
    
    String YRange::stringMeta(SizeT pos) const
    {
	const String blim = "[";
	const String elim = "]";
	const String dlim = ",";
	String out = elim;
	for(auto rit = mRVec.end()-1;;--rit){
	    const SizeT cursize = (*rit)->size();
	    const SizeT curpos = pos % cursize;
	    out = (*rit)->stringMeta(curpos) + out;
	    pos /= cursize;
	    if(rit == mRVec.begin()){
		out = blim + out;
		break;
	    }
	    out = dlim + out;
	}
	return out;
    }
    
    const TypeInfo& YRange::type() const
    {
	return typeid(YRange);
    }

    const TypeInfo& YRange::metaType() const
    {
	return typeid(Vector<DType>);
    }

    RangePtr YRange::extend(const RangePtr& r) const
    {
	CXZ_ASSERT(r->dim() == this->dim(), "cannot extend range of dimension "
		   << this->dim() << " by range of dimension " << r->dim());
	Vector<RangePtr> rvec(this->dim());
	if(this->dim() == 1 and r->sub(0) == nullptr){
	    rvec[0] = mRVec[0]->extend( r );
	}
	else {
	    for(SizeT i = 0; i != this->dim(); ++i){
		rvec[i] = mRVec[i]->extend( r->sub(i) );
	    }
	}
	return YRangeFactory( rvec ).create();
    }
    
    Vector<Uuid> YRange::key() const
    {
	Vector<Uuid> k(mRVec.size());
	std::transform(mRVec.begin(), mRVec.end(), k.begin(),
		       [&](const RangePtr& r) { return r->id(); } );
	return k;
    }

    YRange::YRange(const Vector<RangePtr>& rvec) : mRVec(rvec) {}
    
    YRange::YRange(Vector<RangePtr>&& rvec) : mRVec(std::forward<Vector<RangePtr>>(rvec)) {}

    /*==========================+
     |   non-member functions   |
     +==========================*/

    RangePtr yrange(const Vector<RangePtr>& rs)
    {
	return YRangeFactory(rs).create();
    }

    Sptr<YRange> xplYrange(const Vector<RangePtr>& rs)
    {
	return std::dynamic_pointer_cast<YRange>( yrange( rs ) );
    }

    /*=================+
     |   Range Casts   |
     +=================*/

    Sptr<YRange> RangeCast<YRange>::func(const RangePtr& r)
    {
	return std::dynamic_pointer_cast<YRange>( YRangeFactory({r}).create() );
    }

}
