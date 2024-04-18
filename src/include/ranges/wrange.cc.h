// -*- C++ -*-
/**
   
   @file include/ranges/wrange.cc.h
   @brief WRange, WRangeFactory and WIndex implementations.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_wrange_cc_h__
#define __cxz_wrange_cc_h__

#include <functional>
#include <algorithm>

#include "wrange.h"
#include "prange.h"
#include "index_mul.h"
#include "xpr/for.h"
#include "operation/op_types.h"
#include "operation/op_types.cc.h"

namespace CNORXZ
{
    /*===============+
     |    WIndex     |	     
     +===============*/

    template <typename MetaT>
    WIndex<MetaT>::WIndex(const RangePtr& range, SizeT pos) :
	IndexInterface<WIndex<MetaT>,MetaT>(pos),
	mRangePtr(rangeCast<RangeType>(range)),
	mMetaPtr(mRangePtr->get()),
	mSpaceToPos(mRangePtr->spaceToPos()),
	mPosToSpace(mRangePtr->posToSpace())
    {}

    template <typename MetaT>
    WIndex<MetaT>& WIndex<MetaT>::operator=(size_t lexpos)
    {
	IB::mPos = lexpos;
	return *this;
    }

    template <typename MetaT>
    WIndex<MetaT>& WIndex<MetaT>::operator++()
    {
	++IB::mPos;
	return *this;
    }

    template <typename MetaT>
    WIndex<MetaT>& WIndex<MetaT>::operator--()
    {
	--IB::mPos;
	return *this;
    }

    template <typename MetaT>
    WIndex<MetaT> WIndex<MetaT>::operator+(Int n) const
    {
	return WIndex(mRangePtr, IB::mPos + n);
    }
    
    template <typename MetaT>
    WIndex<MetaT> WIndex<MetaT>::operator-(Int n) const
    {
	return WIndex(mRangePtr, IB::mPos - n);
    }

    template <typename MetaT>
    SizeT WIndex<MetaT>::operator-(const WIndex& i) const
    {
	return lex() - i.lex();
    }
    
    template <typename MetaT>
    WIndex<MetaT>& WIndex<MetaT>::operator+=(Int n)
    {
	IB::mPos += n;
	return *this;
    }
    
    template <typename MetaT>
    WIndex<MetaT>& WIndex<MetaT>::operator-=(Int n)
    {
	IB::mPos -= n;
	return *this;
    }

    template <typename MetaT>
    SizeT WIndex<MetaT>::lex() const
    {
	return IB::mPos;
    }
    
    template <typename MetaT>
    UPos WIndex<MetaT>::pmax() const
    {
	return UPos(mRangePtr->size());
    }

    template <typename MetaT>
    UPos WIndex<MetaT>::lmax() const
    {
	return UPos(mRangePtr->size());
    }

    template <typename MetaT>
    IndexId<0> WIndex<MetaT>::id() const
    {
	return IndexId<0>(this->ptrId());
    }

    template <typename MetaT>
    const MetaT& WIndex<MetaT>::operator*() const
    {
	return meta();
    }
    
    template <typename MetaT>
    String WIndex<MetaT>::stringMeta() const
    {
	return toString(this->meta());
    }
    
    template <typename MetaT>
    const MetaT& WIndex<MetaT>::meta() const
    {
	return mMetaPtr[mPosToSpace[IB::mPos]];
    }
    
    template <typename MetaT>
    WIndex<MetaT>& WIndex<MetaT>::at(const MetaT& metaPos)
    {
	(*this) = mRangePtr->getMeta(metaPos);
	return *this;
    }

    template <typename MetaT>
    decltype(auto) WIndex<MetaT>::xpr(const Sptr<WIndex<MetaT>>& _this) const
    {
	CXZ_ERROR("not implemented"); // TODO: Operation root with reordered meta data access!!!
	return coproot(mMetaPtr,_this);
    }

    template <typename MetaT>
    RangePtr WIndex<MetaT>::prange(const WIndex<MetaT>& last) const
    {
	CXZ_ASSERT(last >= *this, "got last index position (" << last.lex()
		   << ") smaller than begin index position (" << lex() << ")");
	const SizeT beginPos = lex();
	Vector<SizeT> parts(last.lex()-beginPos+1);
	for(auto i = *this; i != last+1; ++i){
	    parts[i.lex()-beginPos] = i.lex();
	}
	return CNORXZ::prange(mRangePtr, parts);
    }

    template <typename MetaT>
    SizeT WIndex<MetaT>::deepFormat() const
    {
	return 1;
    }

    template <typename MetaT>
    SizeT WIndex<MetaT>::deepMax() const
    {
	return lmax().val();
    }

    template <typename MetaT>
    WIndex<MetaT>& WIndex<MetaT>::reformat(const Vector<SizeT>& f, const Vector<SizeT>& s)
    {
	CXZ_ASSERT(f[0]*s[0] == lmax().val(), "got wrong extension: " << f[0]*s[0]
		   << " vs " << lmax().val());
	CXZ_ASSERT(CNORXZ::formatIsTrivial(f,s), "format is not trivial: f = " << toString(f)
		   << ", s = " << toString(s));
	return *this;
    }

    template <typename MetaT>
    size_t WIndex<MetaT>::dim() const // = 1
    {
	return 1;
    }

    template <typename MetaT>
    Sptr<WRange<MetaT>> WIndex<MetaT>::range() const
    {
	return mRangePtr;
    }

    template <typename MetaT>
    template <SizeT I>
    decltype(auto) WIndex<MetaT>::stepSize(const IndexId<I>& id) const
    {
	if constexpr(I != 0){
	    return SPos<0>();
	}
	else {
	    return UPos(id == this->id() ? 1 : 0);
	}
    }

    template <typename MetaT>
    template <class Xpr, class F>
    decltype(auto) WIndex<MetaT>::ifor(const Xpr& xpr, F&& f) const
    {
	return For<0,Xpr,F>(this->pmax().val(), this->id(), xpr, std::forward<F>(f));
    }

    template <typename MetaT>
    bool WIndex<MetaT>::formatIsTrivial() const
    {
	return true;
    }
    
    template <typename MetaT, class I1>
    decltype(auto) operator*(const Sptr<WIndex<MetaT>>& a, const Sptr<I1>& b)
    {
	return iptrMul(a, b);
    }

    /*====================+
     |   WRangeFactory    |
     +====================*/

    template <typename MetaT>
    WRangeFactory<MetaT>::WRangeFactory(const Vector<MetaT>& space) :
	mSpace(space) {}

    template <typename MetaT>
    WRangeFactory<MetaT>::WRangeFactory(Vector<MetaT>&& space) :
	mSpace(space) {}

    template <typename MetaT>
    WRangeFactory<MetaT>::WRangeFactory(const Vector<MetaT>& space, const RangePtr& ref) :
	mSpace(space), mRef(ref) {}

    template <typename MetaT>
    WRangeFactory<MetaT>::WRangeFactory(Vector<MetaT>&& space, const RangePtr& ref) :
	mSpace(space), mRef(ref) {}

    template <typename MetaT>
    void WRangeFactory<MetaT>::make()
    {
	const auto& info = typeid(WRange<MetaT>);
	if(mRef != nullptr) {
	    mProd = this->fromCreated(info, {mRef->id()});
	}
	if(mProd == nullptr){
	    RangePtr key = mProd = std::shared_ptr<WRange<MetaT>>
		( new WRange<MetaT>( std::move(mSpace) ) );
	    if(mRef != nullptr) { key = mRef; }
	    this->addToCreated(info, { key->id() }, mProd);
	}
    }
    
    /*=============+
     |   WRange    |
     +=============*/

    template <typename MetaT>
    void WRange<MetaT>::setupSpace(const Vector<MetaT>& space)
    {
	mSpace = space;
	const SizeT s = mSpace.size();
	mSpaceToPos.resize(s);
	mPosToSpace.resize(s);
	std::sort(mSpace.begin(), mSpace.end(), std::less<MetaT>());
	auto itdupl = std::adjacent_find(mSpace.begin(), mSpace.end());
	CXZ_ASSERT(itdupl == mSpace.end(), "found duplicate: " << *itdupl);
	for(SizeT lx = 0; lx != s; ++lx){
	    for(SizeT sp = 0; sp != s; ++sp){
		if(space[lx] == mSpace[sp]){
		    mPosToSpace[lx] = sp;
		    mSpaceToPos[sp] = lx;
		    break;
		}
	    }
	}
    }
    
    template <typename MetaT>
    WRange<MetaT>::WRange(const Vector<MetaT>& space) :
	RangeInterface<WRange<MetaT>>()
    {
	setupSpace(space);
    }

    template <typename MetaT>
    WRange<MetaT>::WRange(Vector<MetaT>&& space) :
	RangeInterface<WRange<MetaT>>()
    {
	setupSpace(space);
    }

        
    template <typename MetaT>
    const MetaT& WRange<MetaT>::get(SizeT pos) const
    {
	return mSpace[mPosToSpace[pos]];
    }

    template <typename MetaT>
    const MetaT* WRange<MetaT>::get() const
    {
	return mSpace.data();
    }

    template <typename MetaT>
    SizeT WRange<MetaT>::getMeta(const MetaT& meta) const
    {
	auto b = mSpace.begin();
	auto e = mSpace.end();
	auto i = std::lower_bound(b, e, meta, std::less<MetaT>());
	if(i == e){
	    return size();
	}
	if(*i != meta){
	    return size();
	}
	return mSpaceToPos[i - b];
    }

    template <typename MetaT>
    SizeT WRange<MetaT>::size() const
    {
	return mSpace.size();
    }

    template <typename MetaT>
    SizeT WRange<MetaT>::dim() const
    {
	return 1;
    }

    template <typename MetaT>
    String WRange<MetaT>::stringMeta(SizeT pos) const
    {
	return toString(this->get(pos));
    }
    
    template <typename MetaT>
    const TypeInfo& WRange<MetaT>::type() const
    {
	return typeid(WRange<MetaT>);
    }
    
    template <typename MetaT>
    const TypeInfo& WRange<MetaT>::metaType() const
    {
	return typeid(MetaT);
    }

    template <typename MetaT>
    RangePtr WRange<MetaT>::extend(const RangePtr& r) const
    {
	auto rx = rangeCast<WRange<MetaT>>(r);
	Vector<MetaT> nspace(mSpace.size()+rx->size());
	SizeT i = 0;
	for(; i != mSpace.size(); ++i){
	    nspace[i] = mSpace[mPosToSpace[i]];
	}
	for(SizeT j = 0; j != rx->mSpace.size(); ++j, ++i){
	    nspace[i] = rx->mSpace[rx->mPosToSpace[j]];
	}
	return WRangeFactory<MetaT>( nspace ).create();
    }

    template <typename MetaT>
    Vector<Uuid> WRange<MetaT>::key() const
    {
	return Vector<Uuid> { this->id() };
    }

    /*=================+
     |   Range Casts   |
     +=================*/

    template <typename MetaT>
    struct WRangeCast
    {
	template <typename T>
	static inline Sptr<WRange<MetaT>> transform(const RangePtr& r)
	{
	    if(r->type() == typeid(PRange<WRange<T>>)){
		return transform<T>( std::dynamic_pointer_cast<PRange<WRange<T>>>(r)->derive() );
	    }
	    else if(r->type() == typeid(WRange<T>)){
		auto rr = std::dynamic_pointer_cast<WRange<T>>(r);
		Vector<MetaT> v(rr->size());
		std::transform(rr->begin(), rr->end(), v.begin(),
			       [](const T& x) { return static_cast<MetaT>(x); } );
		return std::dynamic_pointer_cast<WRange<MetaT>>
		    ( WRangeFactory<MetaT>(std::move(v)).create() );
	    }
	    else {
		return nullptr;
	    }
	}
	
	static inline Sptr<WRange<MetaT>> cast(const RangePtr& r)
	{
	    static_assert(std::is_fundamental<MetaT>::value, "got non-fundamental type");
	    CXZ_ASSERT(r->dim() == 1, "range cast into WRange<Int>: source range must have dim = 1, got " << r->dim());
	    Sptr<WRange<MetaT>> o = nullptr;
	    // TODO: cast from CRange!!!
	    o = transform<SizeT>(r); if(o) return o;
	    o = transform<Int>(r); if(o) return o;
	    o = transform<LInt>(r); if(o) return o;
	    o = transform<Double>(r); if(o) return o;
	    // else general transform using DType (better than nothing), to be implemented!!!
	    CXZ_ERROR("no range cast available for input range '" << r->type().name() << "'");
	    return nullptr;
	}
    };

    template <typename U>
    struct WRangeCast<Vector<U>>
    {
	template <typename T>
	static inline Sptr<WRange<Vector<U>>> transform(const RangePtr& r)
	{
	    if(r->type() == typeid(WRange<T>)){
		auto rr = std::dynamic_pointer_cast<WRange<T>>(r);
		Vector<Vector<U>> v(rr->size());
		std::transform(rr->begin(), rr->end(), v.begin(),
			       [](const T& x) { return Vector<U> { static_cast<U>(x) }; } );
		return std::dynamic_pointer_cast<WRange<Vector<U>>>
		    ( WRangeFactory<Vector<U>>(std::move(v)).create() );
	    }
	    else {
		return nullptr;
	    }
	}

	template <typename T, SizeT N>
	static inline Sptr<WRange<Vector<U>>> atransform(const RangePtr& r)
	{
	    if(r->type() == typeid(WRange<Arr<T,N>>)){
		auto rr = std::dynamic_pointer_cast<WRange<Arr<T,N>>>(r);
		Vector<Vector<U>> v(rr->size());
		std::transform(rr->begin(), rr->end(), v.begin(),
			       [](const Arr<T,N>& x) {
				   return iter<0,N>( [&](auto i) { return static_cast<U>(x[i]); },
						     [](const auto&... e) { return Vector<U>{ e... }; });
			       } );
		return std::dynamic_pointer_cast<WRange<Vector<U>>>
		    ( WRangeFactory<Vector<U>>(std::move(v)).create() );
	    }
	    else {
		return nullptr;
	    }
	}

	static inline Sptr<WRange<Vector<U>>> cast(const RangePtr& r)
	{
	    Sptr<WRange<Vector<U>>> o = nullptr;
	    if constexpr(std::is_fundamental<U>::value){
		o = transform<SizeT>(r); if(o) return o;
		o = transform<Int>(r); if(o) return o;
		o = transform<LInt>(r); if(o) return o;
		o = transform<Double>(r); if(o) return o;
		o = atransform<SizeT,2>(r); if(o) return o;
		o = atransform<Int,2>(r); if(o) return o;
		o = atransform<LInt,2>(r); if(o) return o;
		o = atransform<Double,2>(r); if(o) return o;
		o = atransform<SizeT,3>(r); if(o) return o;
		o = atransform<Int,3>(r); if(o) return o;
		o = atransform<LInt,3>(r); if(o) return o;
		o = atransform<Double,3>(r); if(o) return o;
	    }
	    // else general transform using DType (better than nothing), to be implemented!!!
	    CXZ_ERROR("no range cast available for input range '" << r->type().name() << "'");
	}
    };

    
    template <typename MetaT>
    Sptr<WRange<MetaT>> RangeCast<WRange<MetaT>>::func(const RangePtr& r)
    {
	if constexpr(std::is_fundamental<MetaT>::value or is_vector<MetaT>::value){
	    return WRangeCast<MetaT>::cast(r);
	}
	else {
	    CXZ_ERROR("no range cast available for input range '" << r->type().name() << "'");
	    return nullptr;
	}
    }

    template <typename MetaT>
    RangePtr wrange(const Vector<MetaT>& space)
    {
	return WRangeFactory<MetaT>(space).create();
    }
    
}

#endif
