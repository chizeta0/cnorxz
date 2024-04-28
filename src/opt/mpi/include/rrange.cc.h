// -*- C++ -*-
/**
   
   @file opt/mpi/include/rrange.cc.h
   @brief RRange and RIndex template implementation.
   
   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_mpi_rrange_cc_h__
#define __cxz_mpi_rrange_cc_h__

#include "rrange.h"
#include "mpi_wrappers.h"
#include "mpi_wrappers.cc.h"

namespace CNORXZ
{
    namespace mpi
    {

	/*==============+
	 |    RIndex    |
	 +==============*/

	template <class IndexI, class IndexK>
	RIndex<IndexI,IndexK>::RIndex(const RIndex& in) :
	    mRange(in.mRange),
	    mI(std::make_shared<IndexI>(mRange->local())),
	    mK(std::make_shared<IndexK>(mRange->geom()))
	{
	    *this = in.lex();
	}

	template <class IndexI, class IndexK>
	RIndex<IndexI,IndexK>& RIndex<IndexI,IndexK>::operator=(const RIndex& in)
	{
	    mRange = in.mRange;
	    mI = std::make_shared<IndexI>(mRange->local());
	    mK = std::make_shared<IndexK>(mRange->geom());
	    *this = in.lex();
	    return *this;
	}

	template <class IndexI, class IndexK>
	RIndex<IndexI,IndexK>::RIndex(const RangePtr& global, SizeT lexpos) :
	    mRange(rangeCast<RangeType>(global)),
	    mI(std::make_shared<IndexI>(mRange->local())),
	    mK(std::make_shared<IndexK>(mRange->geom()))
	{
	    *this = lexpos;
	}

	template <class IndexI, class IndexK>
	RIndex<IndexI,IndexK>::RIndex(const Sptr<IndexI>& i, const Sptr<IndexK>& k) :
	    mRange(rangeCast<RangeType>( RRangeFactory(i->range(), k->range()).create() )),
	    mI(i),
	    mK(k)
	{
	    (*this)();
	}

	template <class IndexI, class IndexK>
	RIndex<IndexI,IndexK>& RIndex<IndexI,IndexK>::operator=(SizeT pos)
	{
	    mLex = pos; // = lex
	    if(mLex >= lmax().val()){
		mLex = lmax().val();
		return *this;
	    }
	    // pos is the lexicographic position of the global range.
	    // Hence, have to consider the rank geometry.
	    const auto& i = mI->pack();
	    const auto& k = mK->pack();
	    const auto& ilf = mI->lexFormat();
	    const auto& klf = mK->lexFormat();
	    SizeT r = 0;
	    SizeT l = 0;
	    if constexpr(has_static_sub<IndexI>::value){
		constexpr SizeT NI = index_dim<IndexI>::value;
		iter<0,NI>( [&](auto mu) {
		    const SizeT jmu = (mLex / (ilf[mu].val()*klf[mu].val())) %
			(i[mu]->lmax().val() * k[mu]->lmax().val());
		    r += ( jmu / i[mu]->lmax().val() ) * klf[mu].val();
		    l += ( jmu % i[mu]->lmax().val() ) * ilf[mu].val();
		}, NoF{} );
	    }
	    else if constexpr( has_static_sub<IndexK>::value){
		constexpr SizeT NI = index_dim<IndexK>::value;
		iter<0,NI>( [&](auto mu) {
		    const SizeT jmu = (mLex / (ilf[mu].val()*klf[mu].val())) %
			(i[mu]->lmax().val() * k[mu]->lmax().val());
		    r += ( jmu / i[mu]->lmax().val() ) * klf[mu].val();
		    l += ( jmu % i[mu]->lmax().val() ) * ilf[mu].val();
		}, NoF{} );
	    }
	    else {
		const SizeT NI = mI->dim();
		for(SizeT mu = 0; mu != NI; ++mu){
		    const SizeT jmu = (mLex / (ilf[mu].val()*klf[mu].val())) %
			(i[mu]->lmax().val() * k[mu]->lmax().val());
		    r += ( jmu / i[mu]->lmax().val() ) * klf[mu].val();
		    l += ( jmu % i[mu]->lmax().val() ) * ilf[mu].val();
		}
	    }
	    *mI = l;
	    *mK = r;
	    IB::mPos = mK->pos() * mI->pmax().val() + mI->pos();
	    return *this;
	}

	template <class IndexI, class IndexK>
	RIndex<IndexI,IndexK>& RIndex<IndexI,IndexK>::operator++()
	{
	    *this = lex() + 1; // room for optimization
	    return *this;
	}

	template <class IndexI, class IndexK>
	RIndex<IndexI,IndexK>& RIndex<IndexI,IndexK>::operator--()
	{
	    *this = lex() - 1; // room for optimization
	    return *this;
	}

	template <class IndexI, class IndexK>
	RIndex<IndexI,IndexK> RIndex<IndexI,IndexK>::operator+(Int n) const
	{
	    RIndex<IndexI,IndexK> o(*this);
	    return o += n;
	}

	template <class IndexI, class IndexK>
	RIndex<IndexI,IndexK> RIndex<IndexI,IndexK>::operator-(Int n) const
	{
	    RIndex<IndexI,IndexK> o(*this);
	    return o -= n;
	}

	template <class IndexI, class IndexK>
	SizeT RIndex<IndexI,IndexK>::operator-(const RIndex& i) const
	{
	    return lex() - i.lex();
	}

	template <class IndexI, class IndexK>
	RIndex<IndexI,IndexK>& RIndex<IndexI,IndexK>::operator+=(Int n)
	{
	    *this = lex() + n;
	    return *this;
	}

	template <class IndexI, class IndexK>
	RIndex<IndexI,IndexK>& RIndex<IndexI,IndexK>::operator-=(Int n)
	{
	    *this = lex() - n;
	    return *this;
	}

	template <class IndexI, class IndexK>
	SizeT RIndex<IndexI,IndexK>::lex() const
	{
	    return mLex;
	}

	template <class IndexI, class IndexK>
	constexpr decltype(auto) RIndex<IndexI,IndexK>::pmax() const
	{
	    return UPos(mK->pmax().val() * mI->pmax().val());
	}

	template <class IndexI, class IndexK>
	constexpr decltype(auto) RIndex<IndexI,IndexK>::lmax() const
	{
	    return UPos(mK->lmax().val() * mI->lmax().val());
	}

	template <class IndexI, class IndexK>
	IndexId<0> RIndex<IndexI,IndexK>::id() const
	{
	    return IndexId<0>(this->ptrId());
	}

	template <class IndexI, class IndexK>
	typename RIndex<IndexI,IndexK>::MetaType RIndex<IndexI,IndexK>::operator*() const
	{
	    return meta();
	}
	
	template <class IndexI, class IndexK>
	constexpr SizeT RIndex<IndexI,IndexK>::dim() const
	{
	    return mI->dim();
	}

	template <class IndexI, class IndexK>
	Sptr<typename RIndex<IndexI,IndexK>::RangeType> RIndex<IndexI,IndexK>::range() const
	{
	    return mRange;
	}

	template <class IndexI, class IndexK>
	template <SizeT I>
	decltype(auto) RIndex<IndexI,IndexK>::stepSize(const IndexId<I>& id) const
	{
	    const auto own = [&]() {
		if constexpr(I != 0){ return SPos<0> {}; }
		else { return UPos(id == this->id() ? 1 : 0); }
	    };
	    return mI->stepSize(id) + own();
	    //return getRankStepSize(id);
	}

	template <class IndexI, class IndexK>
	String RIndex<IndexI,IndexK>::stringMeta() const
	{
	    const SizeT r = mK->lex();
	    String o;
	    if(r == getRankNumber()){
		o = mI->stringMeta();
	    }
	    bcast(o, r);
	    return o;
	}

	template <class IndexI, class IndexK>
	typename RIndex<IndexI,IndexK>::MetaType RIndex<IndexI,IndexK>::meta() const
	{
	    MetaType o;
	    if constexpr(Typemap<MetaType>::exists){
		const SizeT r = mK->lex();
		if(r == getRankNumber()){
		    o = mI->meta();
		}
		bcast(o, r);
	    }
	    else {
		// TODO: In general don't allow RIndices if broadcast for MetaType
		// does not exitst (once DType broadcast is implemented)!!!
		CXZ_ERROR("no broadcast implementation for given meta type ("
			  << typeid(MetaType).name() << ") available");
	    }
	    return o;
	}

	template <class IndexI, class IndexK>
	RIndex<IndexI,IndexK>& RIndex<IndexI,IndexK>::at(const MetaType& metaPos)
	{
	    mI->at(metaPos);
	    const size_t lex = mI->lex();
	    Vector<size_t> lexs(mK->lmax().val());
	    MPI_Allgather(&lex, 1, MPI_UNSIGNED_LONG, lexs.data(), 1, MPI_UNSIGNED_LONG,
			  MPI_COMM_WORLD);
	    SizeT root = 0;
	    for(; root != lexs.size(); ++root) {
		if(lexs[root] != mI->lmax().val()){
		    break;
		}
	    }
	    if(root == lexs.size()){ // metaPos not in rrange
		*this = lmax().val();
		VCHECK(toString(metaPos));
		assert(0);
	    }
	    else {
		*mK = root;
		*mI = lexs[root];
		(*this)();
	    }
	    return *this;
	}

	template <class IndexI, class IndexK>
	RangePtr RIndex<IndexI,IndexK>::prange(const RIndex<IndexI,IndexK>& last) const
	{
	    CXZ_ERROR("not implemented");
	    return nullptr;
	}

	template <class IndexI, class IndexK>
	auto RIndex<IndexI,IndexK>::deepFormat() const
	{
	    return concat( mul(mK->deepFormat(), mI->lmax().val() ), mI->deepFormat() );
	}

	template <class IndexI, class IndexK>
	auto RIndex<IndexI,IndexK>::deepMax() const
	{
	    return concat( mK->deepMax(), mI->deepMax() );
	}

	template <class IndexI, class IndexK>
	RIndex<IndexI,IndexK>& RIndex<IndexI,IndexK>::reformat(const Vector<SizeT>& f, const Vector<SizeT>& s)
	{
	    CXZ_ERROR("not implemented");
	    return *this;
	}

	template <class IndexI, class IndexK>
	template <class Xpr, class F>
	constexpr decltype(auto) RIndex<IndexI,IndexK>::ifor(const Xpr& xpr, F&& f) const
	{
	    return mI->ifor(xpr, std::forward<F>(f));
	}

	template <class IndexI, class IndexK>
	bool RIndex<IndexI,IndexK>::formatIsTrivial() const
	{
	    return mI->formatIsTrivial();
	}

	template <class IndexI, class IndexK>
	decltype(auto) RIndex<IndexI,IndexK>::xpr(const Sptr<RIndex<IndexI,IndexK>>& _this) const
	{
	    return _this->local()->xpr( _this->local() );
	}

	template <class IndexI, class IndexK>
	RIndex<IndexI,IndexK>& RIndex<IndexI,IndexK>::operator()(const Sptr<IndexI>& i)
	{
	    mI = i;
	    return (*this)();
	}
	
	template <class IndexI, class IndexK>
	RIndex<IndexI,IndexK>& RIndex<IndexI,IndexK>::operator()()
	{
	    if(mI->lex() >= mI->lmax().val()){
		mLex = lmax().val();
	    }
	    if constexpr(has_static_sub<IndexI>::value){
		constexpr SizeT NI = index_dim<IndexI>::value;
		mLex = iter<0,NI>
		    ([&](auto i) {
			return mK->pack()[i]->lex() * mK->lexFormat()[i].val() *
			    mI->lexFormat()[i].val() * mI->pack()[i]->lmax().val() +
			    mI->pack()[i]->lex() * mI->lexFormat()[i].val() *
			    mK->lexFormat()[i].val();
		    }, [](const auto&... e) { return (e + ...); });
	    }
	    else if constexpr( has_static_sub<IndexK>::value){
		constexpr SizeT NI = index_dim<IndexK>::value;
		mLex = iter<0,NI>
		    ([&](auto i) {
			return mK->pack()[i]->lex() * mK->lexFormat()[i].val() *
			    mI->lexFormat()[i].val() * mI->pack()[i]->lmax().val() +
			    mI->pack()[i]->lex() * mI->lexFormat()[i].val() *
			    mK->lexFormat()[i].val();
		    }, [](const auto&... e) { return (e + ...); });
	    }
	    else {
		const SizeT NI = mI->dim();
		mLex = 0;
		for(SizeT i = 0; i != NI; ++i){
		    mLex += mK->pack()[i]->lex() * mK->lexFormat()[i].val() *
			mI->lexFormat()[i].val() * mI->pack()[i]->lmax().val() +
			mI->pack()[i]->lex() * mI->lexFormat()[i].val() *
			mK->lexFormat()[i].val();
		}
	    }
	    IB::mPos = mK->pos() * mI->pmax().val() + mI->pos();
	    return *this;
	}

	template <class IndexI, class IndexK>
	SizeT RIndex<IndexI,IndexK>::rank() const
	{
	    return mK->lex();
	}

	template <class IndexI, class IndexK>
	Sptr<IndexI> RIndex<IndexI,IndexK>::local() const
	{
	    return mI;
	}
	
	template <class IndexI, class IndexK>
	Sptr<IndexK> RIndex<IndexI,IndexK>::rankI() const
	{
	    return mK;
	}

	template <class IndexI, class IndexK, class I1>
	decltype(auto) operator*(const Sptr<RIndex<IndexI,IndexK>>& a, const Sptr<I1>& b)
	{
	    return iptrMul(a, b);
	}

	/*=====================+
	 |    RRangeFactory    |
	 +=====================*/

	template <class RangeI, class RangeK>
	RRangeFactory<RangeI,RangeK>::RRangeFactory(const Sptr<RangeI>& ri,
						    const Sptr<RangeK>& rk):
	    mRI(ri),
	    mRK(rk)
	{
	    int s = 0;
	    MPI_Comm_size(MPI_COMM_WORLD, &s);
	    CXZ_ASSERT(rk->size() == static_cast<SizeT>(s),
		       "geometry rank size ( = " << rk->size()
		       << ") does not match number of ranks ( = " << s <<  ")");
	    if constexpr(has_static_sub<typename RangeI::IndexType>::value and
			 has_static_sub<typename RangeK::IndexType>::value) {
		constexpr SizeT NRI = RangeI::NR;
		constexpr SizeT NRK = RangeK::NR;
		static_assert(NRI == NRK, "ranges have to be of same dimension");
	    }
	    else {
		CXZ_ASSERT(ri->dim() == rk->dim(), "ranges have to be of same dimension, got "
			   << ri->dim() << " and " << rk->dim());
	    }
	}

	template <class RangeI, class RangeK>
	void RRangeFactory<RangeI,RangeK>::make()
	{
	    Vector<Uuid> key = { mRI->id(), mRK->id() };
	    const auto& info = typeid(RRange<RangeI,RangeK>);
	    mProd = this->fromCreated(info, key);
	    if(mProd == nullptr) {
		mProd = std::shared_ptr<RRange<RangeI,RangeK>>
		    ( new RRange<RangeI,RangeK>(mRI, mRK) );
		this->addToCreated(info, key, mProd);
	    }
	}

	/*==============+
	 |    RRange    |
	 +==============*/

	template <class RangeI, class RangeK>
	RangePtr RRange<RangeI,RangeK>::sub(SizeT num) const
	{
	    if(num == 0){
		return mGeom;
	    }
	    else if(num == 1){
		return mLocal;
	    }
	    else {
		CXZ_ERROR("out of range: " << num);
		return nullptr;
	    }
	}
	
	template <class RangeI, class RangeK>
	MArray<RangePtr> RRange<RangeI,RangeK>::sub() const
	{
	    RangePtr sr = SRangeFactory<SizeT,2>( Arr<SizeT,2> { 0, 1 } ).create();
	    return MArray<RangePtr>( sr, Vector<RangePtr>( { mGeom, mLocal } ) );
	}
	
	template <class RangeI, class RangeK>
	SizeT RRange<RangeI,RangeK>::size() const
	{
	    return mGeom->size() * mLocal->size();
	}
	
	template <class RangeI, class RangeK>
	SizeT RRange<RangeI,RangeK>::dim() const
	{
	    return 2;
	}
	
	template <class RangeI, class RangeK>
	String RRange<RangeI,RangeK>::stringMeta(SizeT pos) const
	{
	    return (this->begin()+pos).stringMeta();
	}
	
	template <class RangeI, class RangeK>
	const TypeInfo& RRange<RangeI,RangeK>::type() const
	{
	    return typeid(RRange<RangeI,RangeK>);
	}
	
	template <class RangeI, class RangeK>
	const TypeInfo& RRange<RangeI,RangeK>::metaType() const
	{
	    return typeid(MetaType);
	}
	
	template <class RangeI, class RangeK>
	RangePtr RRange<RangeI,RangeK>::extend(const RangePtr& r) const
	{
	    CXZ_ERROR("not implemented");
	    return nullptr;
	}

	template <class RangeI, class RangeK>
	Sptr<RangeI> RRange<RangeI,RangeK>::local() const
	{
	    return mLocal;
	}

	template <class RangeI, class RangeK>
	Sptr<RangeK> RRange<RangeI,RangeK>::geom() const
	{
	    return mGeom;
	}
	    
	template <class RangeI, class RangeK>
	const typename RRange<RangeI,RangeK>::MetaType RRange<RangeI,RangeK>::get(SizeT pos) const
	{
	    return (this->begin()+pos)->meta();
	}

	template <class RangeI, class RangeK>
	SizeT RRange<RangeI,RangeK>::getMeta(const MetaType& metaPos) const
	{
	    auto i = this->begin();
	    return i.at(metaPos).pos();
	}

	template <class RangeI, class RangeK>
	SizeT RRange<RangeI,RangeK>::getRank(SizeT pos) const
	{
	    return (this->begin()+pos)->rank();
	}
	
	/*=========================+
	 |    RRange (protected)   |
	 +=========================*/

	template <class RangeI, class RangeK>
	RRange<RangeI,RangeK>::RRange(const Sptr<RangeI>& loc, const Sptr<RangeK>& geom) :
	    mLocal(loc),
	    mGeom(geom)
	{}

	template <class RangeI, class RangeK>
	Vector<Uuid> RRange<RangeI,RangeK>::key() const
	{
	    return Vector<Uuid> { mLocal->id(), mGeom->id() };
	}

	/*============================+
	 |    non-member functions    |
	 +============================*/

    } // namespace mpi

    template <class RangeI, class RangeK>
    Sptr<mpi::RRange<RangeI,RangeK>> RangeCast<mpi::RRange<RangeI,RangeK>>::func(const RangePtr& r)
    {
	CXZ_ASSERT(r->dim() == 2, "expected RRange");
	Sptr<RangeI> loc = rangeCast<RangeI>(r->sub(1));
	Sptr<RangeK> geom = rangeCast<RangeK>(r->sub(0));
	return std::dynamic_pointer_cast<mpi::RRange<RangeI,RangeK>>
	    ( mpi::RRangeFactory( loc, geom ).create() );
    }
	
} // namespace CNORXZ

#endif
