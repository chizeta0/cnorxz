
#include "ranges/ranges.h"

namespace CNORXZ
{
    /*************************
     *   YIndex (private)    *
     *************************/

    inline Vector<XIndexPtr> YIndex::mkIndices() const
    {
	Vector<XIndexPtr> o(mRange->dim(), nullptr);
	for(SizeT i = 0; i != mRange->dim(); ++i){
	    auto rp = mRange->sub(i);
	    CXZ_ASSERT(rp != nullptr, "subranges not available");
	    o[i] = rp->begin().xptr();
	}
	return o;
    }
    
    inline Vector<SizeT> YIndex::mkBlockSizes() const
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

    inline Vector<SizeT> YIndex::mkLexBlockSizes() const
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
    
    inline void YIndex::up(SizeT i)
    {
	auto& idx = mIs[i];
	// it is guaranteed that the last accessible position 
	// is one less than the max position (=end)
	if(i != 0 and idx->lex() == idx->lmax().val()-1){
	    IB::mPos -= mBlockSizes[i] * idx->pos();
	    mLex -= mLexBlockSizes[i] * idx->lex();
	    (*idx) = 0;
	    up(i-1);
	    return;
	}
	IB::mPos += mBlockSizes[i];
	mLex += mLexBlockSizes[i];
	++(*idx);
    }
    
    inline void YIndex::down(SizeT i)
    {
	auto& idx = mIs[i];
	if(i != 0 and idx->pos() == 0){
	    (*idx) = idx->lmax().val()-1;
	    IB::mPos += mBlockSizes[i] * idx->pos();
	    mLex += mLexBlockSizes[i] * idx->lex();
	    down(i-1);
	    return;
	}
	IB::mPos -= mBlockSizes[i];
	mLex -= mLexBlockSizes[i];
	--(*idx);
    }
    
    inline decltype(auto) YIndex::mkIFor(SizeT i, const DXpr<SizeT>& xpr,
					 const std::function<SizeT(SizeT,SizeT)>& f) const
    {
	if(i == mIs.size()-1){
	    return mIs[i]->ifor( xpr, f );
	}
	else {
	    return mIs[i]->ifor( mkIFor(i+1, xpr, f), f );
	}
    }

    inline SizeT YIndex::mkPMax() const
    {
	SizeT o = 0;
	for(SizeT i = 0; i != mIs.size(); ++i){
	    o += (mIs[i]->pmax().val()-1) * mBlockSizes[i];
	}
	return o+1;
    }
    
    inline SizeT YIndex::mkLMax() const
    {
	return std::accumulate(mIs.begin(), mIs.end(),1,
			       [](const auto& res, const auto& el) { return res * el->lmax().val(); } );
    }
    
    
    /***************
     *   YIndex    *
     ***************/

    YIndex::YIndex(const YIndex& i) :
	IndexInterface<YIndex,DType>(i),
	mRange(rangeCast<YRange>(i.range())),
	mIs(mkIndices()),
	mBlockSizes(mkBlockSizes()),
	mLexBlockSizes(mkLexBlockSizes()),
	mPMax(mkPMax()),
	mLMax(mkLMax())
    {
	*this = i.lex();
    }
    
    YIndex& YIndex::operator=(const YIndex& i)
    {
	IndexInterface<YIndex,DType>::operator=(i);
	mRange = rangeCast<YRange>(i.range());
	mIs = mkIndices();
	mBlockSizes = mkBlockSizes();
	mLexBlockSizes = mkLexBlockSizes();
	mPMax = mkPMax();
	mLMax = mkLMax();
	return *this = i.lex();
    }

    YIndex::YIndex(const RangePtr& range, SizeT lexpos) :
	IndexInterface<YIndex,DType>(0),
	mRange(rangeCast<YRange>(range)),
	mIs(mkIndices()),
	mBlockSizes(mkBlockSizes()),
	mLexBlockSizes(mkLexBlockSizes()),
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
	    *mIs[i] = (lex() / mLexBlockSizes[i]) % mIs[i]->lmax().val();
	    IB::mPos += mBlockSizes[i] * mIs[i]->pos();
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

    DType YIndex::operator*() const
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
	    const auto u = mIs[i]->stepSize(id) * UPos(mBlockSizes[i]);
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
	    std::accumulate(std::next(mIs.begin()), mIs.end(), mIs[0]->stringMeta(),
			    [&](const auto& s, const auto& e)
			    { return s + dlim + e->stringMeta(); } ) +
	    elim;
    }
   
    DType YIndex::meta() const
    {
	Vector<DType> v(mIs.size());
	std::transform(mIs.begin(), mIs.end(), v.begin(),
		       [](const auto& x) { return x->meta(); });
	return DType(v);
    }
    
    YIndex& YIndex::at(const DType& meta)
    {
	auto& v = std::any_cast<const Vector<DType>&>(meta.get());
	assert(v.size() == mIs.size());
	IB::mPos = 0;
	for(SizeT i = 0; i != mIs.size(); ++i){
	    mIs[i]->at(v[i]);
	    IB::mPos += mIs[i]->pos() * mBlockSizes[i];
	}
	return *this;
    }

    DXpr<SizeT> YIndex::ifor(const DXpr<SizeT>& xpr, const std::function<SizeT(SizeT,SizeT)>& f) const
    {
	return mkIFor(0, xpr, f);
    }
    
    /**********************
     *   YRangeFactory    *
     **********************/

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
	Vector<PtrId> key(mRVec.size());
	std::transform(mRVec.begin(), mRVec.end(), key.begin(),
		       [&](const RangePtr& r) { return r->id(); } );
	mProd = this->fromCreated(typeid(YRange), key);
	if(mProd == nullptr){
	    mProd = std::shared_ptr<YRange>
		( new YRange( std::move(mRVec) ) );
	    this->addToCreated(typeid(YRange), key, mProd);
	}
    }

    /***************
     *   YRange    *
     ***************/

    RangePtr YRange::sub(SizeT i) const
    {
	return mRVec[i];
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
	return typeid(DType);
    }

    YRange::YRange(const Vector<RangePtr>& rvec) : mRVec(rvec) {}
    
    YRange::YRange(Vector<RangePtr>&& rvec) : mRVec(std::forward<Vector<RangePtr>>(rvec)) {}

    /*******************
     *   Range Casts   *
     *******************/

    Sptr<YRange> RangeCast<YRange>::func(const RangePtr& r)
    {
	return std::dynamic_pointer_cast<YRange>( YRangeFactory({r}).create() );
    }
}
