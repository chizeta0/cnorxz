// -*- C++ -*-
/**
   
   @file opt/mpi/include/rrange.cc.h
   @brief RRange and RIndex declaration.
   
   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_mpi_rrange_cc_h__
#define __cxz_mpi_rrange_cc_h__

#include "rrange.h"
#include "mpi_wrappers.h"

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
	    mI(std::make_shared<Index>(mRange->local())),
	    mK(std::make_shared<YIndex>(mRange->geom()))
	{
	    *this = in.lex();
	}

	template <class IndexI, class IndexK>
	RIndex& RIndex<IndexI,IndexK>::operator=(const RIndex& in)
	{
	    mRange = in.mRange;
	    mI = std::make_shared<Index>(mRange->local());
	    mK = std::make_shared<YIndex>(mRange->geom());
	    *this = in.lex();
	    return *this;
	}

	template <class IndexI, class IndexK>
	RIndex<IndexI,IndexK>::RIndex(const RangePtr& global, SizeT lexpos = 0) :
	    mRange(rangeCast<RangeType>(global)),
	    mI(std::make_shared<IndexI>(mRange->local())),
	    mK(std::make_shared<IndexK>(mRange->geom()))
	{
	    *this = lexpos;
	}

	template <class IndexI, class IndexK>
	RIndex<IndexI,IndexK>& RIndex<IndexI,IndexK>::operator=(SizeT pos)
	{
	    IB::mPos = pos; // = lex
	    if(lexpos >= lmax().val()){
		IB::mPos = pmax().val();
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
		    const SizeT jmu = (IB::mPos / ilf[mu].val()*klf[mu].val()) %
			i[mu]->lmax().val()*k[mu]->lmax().val();
		    r += ( jmu / i[mu]->lmax().val() ) * klf[mu].val();
		    l += ( jmu % i[mu]->lmax().val() ) * ilf[mu].val();
		}, NoF{} );
	    }
	    else if constexpr( has_static_sub<IndexK>::value){
		constexpr SizeT NI = index_dim<IndexK>::value;
		iter<0,NI>( [&](auto mu) {
		    const SizeT jmu = (IB::mPos / ilf[mu].val()*klf[mu].val()) %
			i[mu]->lmax().val()*k[mu]->lmax().val();
		    r += ( jmu / i[mu]->lmax().val() ) * klf[mu].val();
		    l += ( jmu % i[mu]->lmax().val() ) * ilf[mu].val();
		}, NoF{} );
	    }
	    else {
		const SizeT NI = mI->dim();
		for(SizeT mu = 0; mu != NI; ++mu){
		    const SizeT jmu = (IB::mPos / ilf[mu].val()*klf[mu].val()) %
			i[mu]->lmax().val()*k[mu]->lmax().val();
		    r += ( jmu / i[mu]->lmax().val() ) * klf[mu].val();
		    l += ( jmu % i[mu]->lmax().val() ) * ilf[mu].val();
		}
	    }
	    *mI = l;
	    *mK = r;
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
	    return IB::mPos;
	}

	template <class IndexI, class IndexK>
	constexpr RIndex<IndexI,IndexK>::decltype(auto) pmax() const
	{
	    return mK->lmax().val() * mI->lmax().val();
	}

	template <class IndexI, class IndexK>
	constexpr RIndex<IndexI,IndexK>::decltype(auto) lmax() const
	{
	    return mK->lmax().val() * mI->lmax().val();
	}

	template <class IndexI, class IndexK>
	IndexId<0> RIndex<IndexI,IndexK>::id() const
	{
	    return IndexId<0>(this->ptrId());
	}

	template <class IndexI, class IndexK>
	MetaType RIndex<IndexI,IndexK>::operator*() const
	{
	    return meta();
	}
	
	template <class IndexI, class IndexK>
	constexpr SizeT RIndex<IndexI,IndexK>::dim() const
	{
	    return mI->dim();
	}

	template <class IndexI, class IndexK>
	Sptr<RangeType> RIndex<IndexI,IndexK>::range() const
	{
	    return mRange;
	}

	template <class IndexI, class IndexK>
	template <SizeT I>
	decltype(auto) RIndex<IndexI,IndexK>::stepSize(const IndexId<I>& id) const
	{
	    return mK->stepSize(id) * mI->lmax().val() + mI->stepSize(id);
	}

	template <class IndexI, class IndexK>
	String RIndex<IndexI,IndexK>::stringMeta() const
	{
	    const SizeT r = mK->lex();
	    String o;
	    broadcast(r, mI->stringMeta(), &o);
	    return o;
	}

	template <class IndexI, class IndexK>
	MetaType RIndex<IndexI,IndexK>::meta() const
	{
	    const SizeT r = mK->lex();
	    MetaType o;
	    broadcast(r, mI->meta(), &o);
	    return o;
	}

	template <class IndexI, class IndexK>
	RIndex<IndexI,IndexK>& RIndex<IndexI,IndexK>::at(const MetaType& metaPos)
	{
	    CXZ_ERROR("not implemented");
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
	    CXZ_ERROR("not implemented");
	    return 0;
	}

	template <class IndexI, class IndexK>
	bool RIndex<IndexI,IndexK>::formatIsTrivial() const
	{
	    return mI->formatIsTrivial();
	}

	template <class IndexI, class IndexK>
	decltype(auto) RIndex<IndexI,IndexK>::xpr(const Sptr<RIndex<IndexI,IndexK>>& _this) const
	{
	    CXZ_ERROR("not implemented");
	    return 0;
	}

	template <class IndexI, class IndexK>
	SizeT RIndex<IndexI,IndexK>::rank() const
	{
	    return mK->lex();
	}

	template <class IndexI, class IndexK>
	Sptr<Index> RIndex<IndexI,IndexK>::local() const
	{
	    return mI;
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
		static_assert(typename RangeI::NR == typename RangeK::NR,
			      "ranges have to be of same dimension");
	    }
	    else {
		CXZ_ASSERT(ri->dim() == rk->dim(), "ranges have to be of same dimension, got "
			   << ri->dim() << " and " << rk->dim());
	    }
	}

	template <class RangeI, class RangeK>
	void RRangeFactory<RangeI,RangeK>::make()
	{
	    Vector<Uuid> key = { mRI->key(), mRK->key() };
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
	    return mGeom->dim() + mLocal->dim();
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
	const MetaType RRange<RangeI,RangeK>::get(SizeT pos) const
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

	/*===========================+
	 |    non-member functions   |
	 +===========================*/

	template <class RangeJ, class RangeK>
	RangePtr rrange(const Sptr<RangeJ>& global, const Sptr<RangeK>& geom)
	{
	    if constexpr(has_static_sub<typename RangeJ::IndexType>::value and
			 has_static_sub<typename RangeK::IndexType>::value) {
		static_assert(typename RangeI::NR == typename RangeK::NR,
			      "ranges have to be of same dimension");

		constexpr SizeT N = typename RangeI::NR;
		auto mr = ifor<0,N>( [&](auto mu) {
		    return split( global->space()[CSizeT<mu>{}], geom->space()[CSizeT<mu>{}] );
		}, [](const auto&... r) { return xplMrange(r,...); } );
		typedef std::remove_reference<decltype(mr)>::type RangeI;
		return RRangeFactory<RangeI,RangeK>(mr, geom).create(); // explicit range type!!!
	    }
	    // other cases!!!
	    else {
		CXZ_ASSERT(global->dim() == geom->dim(),
			   "ranges have to be of same dimension, got "
			   << global->dim() << " and " << geom->dim());
		Vector<RangePtr> o(global->dim());
		for(SizeT mu = 0; mu != global->dim(); ++mu){
		    o[mu] = split( global->space()[mu], geom->space()[mu] );
		}
		const Sptr<YRange> yr = xplYrange(o);
		return RRangeFactory<YRange,RangeK>(yr, geom).create();
	    }
	}
	
	
    } // namespace mpi
} // namespace CNORXZ

#endif
