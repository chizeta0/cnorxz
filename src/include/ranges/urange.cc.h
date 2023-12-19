// -*- C++ -*-
/**
   
   @file include/ranges/urange.cc.h
   @brief ...


   Copyright (c) 2022 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_urange_cc_h__
#define __cxz_urange_cc_h__

#include <functional>
#include <algorithm>

#include "urange.h"
#include "prange.h"
#include "index_mul.h"
#include "xpr/for.h"
#include "operation/op_types.h"
#include "operation/op_types.cc.h"

namespace CNORXZ
{
    /*****************
     *    UIndex     *	     
     *****************/

    template <typename MetaT>
    UIndex<MetaT>::UIndex(const RangePtr& range, SizeT pos) :
	IndexInterface<UIndex<MetaT>,MetaT>(pos),
	mRangePtr(rangeCast<RangeType>(range)),
	mMetaPtr(&mRangePtr->get(0))
    {}

    template <typename MetaT>
    UIndex<MetaT>& UIndex<MetaT>::operator=(size_t lexpos)
    {
	IB::mPos = lexpos;
	return *this;
    }

    template <typename MetaT>
    UIndex<MetaT>& UIndex<MetaT>::operator++()
    {
	++IB::mPos;
	return *this;
    }

    template <typename MetaT>
    UIndex<MetaT>& UIndex<MetaT>::operator--()
    {
	--IB::mPos;
	return *this;
    }

    template <typename MetaT>
    UIndex<MetaT> UIndex<MetaT>::operator+(Int n) const
    {
	return UIndex(mRangePtr, IB::mPos + n);
    }
    
    template <typename MetaT>
    UIndex<MetaT> UIndex<MetaT>::operator-(Int n) const
    {
	return UIndex(mRangePtr, IB::mPos - n);
    }

    template <typename MetaT>
    SizeT UIndex<MetaT>::operator-(const UIndex& i) const
    {
	return lex() - i.lex();
    }
    
    template <typename MetaT>
    UIndex<MetaT>& UIndex<MetaT>::operator+=(Int n)
    {
	IB::mPos += n;
	return *this;
    }
    
    template <typename MetaT>
    UIndex<MetaT>& UIndex<MetaT>::operator-=(Int n)
    {
	IB::mPos -= n;
	return *this;
    }

    template <typename MetaT>
    SizeT UIndex<MetaT>::lex() const
    {
	return IB::mPos;
    }
    
    template <typename MetaT>
    UPos UIndex<MetaT>::pmax() const
    {
	return UPos(mRangePtr->size());
    }

    template <typename MetaT>
    UPos UIndex<MetaT>::lmax() const
    {
	return UPos(mRangePtr->size());
    }

    template <typename MetaT>
    IndexId<0> UIndex<MetaT>::id() const
    {
	return IndexId<0>(this->ptrId());
    }

    template <typename MetaT>
    const MetaT& UIndex<MetaT>::operator*() const
    {
	return mMetaPtr[IB::mPos];
    }
    
    template <typename MetaT>
    String UIndex<MetaT>::stringMeta() const
    {
	return toString(this->meta());
    }
    
    template <typename MetaT>
    const MetaT& UIndex<MetaT>::meta() const
    {
	return mMetaPtr[IB::mPos];
    }
    
    template <typename MetaT>
    UIndex<MetaT>& UIndex<MetaT>::at(const MetaT& metaPos)
    {
	(*this) = mRangePtr->getMeta(metaPos);
	return *this;
    }

    template <typename MetaT>
    decltype(auto) UIndex<MetaT>::xpr(const Sptr<UIndex<MetaT>>& _this) const
    {
	return coproot(mMetaPtr,_this);
    }

    template <typename MetaT>
    RangePtr UIndex<MetaT>::prange(const UIndex<MetaT>& last) const
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
    SizeT UIndex<MetaT>::deepFormat() const
    {
	return 1;
    }

    template <typename MetaT>
    SizeT UIndex<MetaT>::deepMax() const
    {
	return lmax().val();
    }

    template <typename MetaT>
    UIndex<MetaT> UIndex<MetaT>::reformat(const Vector<SizeT>& f, const Vector<SizeT>& s) const
    {
	// can also get multi dim stuff, but:
	// * overall extension must match
	// * f must be trivial
	/*
	CXZ_ASSERT(f.size() == 1, "expected format of dimension 1, got " << toString(f));
	CXZ_ASSERT(s.size() == 1, "expected sizes of dimension 1, got " << toString(s));
	CXZ_ASSERT(f[0] == 1, "trivial format ([1]), got " << toString(f));
	CXZ_ASSERT(s[0] == lmax().val(), "expected size to be equal to index size (" << 
		   lmax().val() << "), got " << s[0]);
	*/
	return *this;
    }

    template <typename MetaT>
    size_t UIndex<MetaT>::dim() const // = 1
    {
	return 1;
    }

    template <typename MetaT>
    Sptr<URange<MetaT>> UIndex<MetaT>::range() const
    {
	return mRangePtr;
    }

    template <typename MetaT>
    template <SizeT I>
    decltype(auto) UIndex<MetaT>::stepSize(const IndexId<I>& id) const
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
    decltype(auto) UIndex<MetaT>::ifor(const Xpr& xpr, F&& f) const
    {
	return For<0,Xpr,F>(this->pmax().val(), this->id(), xpr, std::forward<F>(f));
    }

    template <typename MetaT>
    bool UIndex<MetaT>::formatIsTrivial() const
    {
	return true;
    }
    
    template <typename MetaT, class I1>
    decltype(auto) operator*(const Sptr<UIndex<MetaT>>& a, const Sptr<I1>& b)
    {
	return iptrMul(a, b);
    }

    /**********************
     *   URangeFactory    *
     **********************/

    template <typename MetaT>
    URangeFactory<MetaT>::URangeFactory(const Vector<MetaT>& space) :
	mSpace(space) {}

    template <typename MetaT>
    URangeFactory<MetaT>::URangeFactory(Vector<MetaT>&& space) :
	mSpace(space) {}

    template <typename MetaT>
    URangeFactory<MetaT>::URangeFactory(const Vector<MetaT>& space, const RangePtr& ref) :
	mSpace(space), mRef(ref) {}

    template <typename MetaT>
    URangeFactory<MetaT>::URangeFactory(Vector<MetaT>&& space, const RangePtr& ref) :
	mSpace(space), mRef(ref) {}

    template <typename MetaT>
    void URangeFactory<MetaT>::make()
    {
	const auto& info = typeid(URange<MetaT>);
	if(mRef != nullptr) {
	    mProd = this->fromCreated(info, {mRef->id()});
	}
	if(mProd == nullptr){
	    RangePtr key = mProd = std::shared_ptr<URange<MetaT>>
		( new URange<MetaT>( std::move(mSpace) ) );
	    if(mRef != nullptr) { key = mRef; }
	    this->addToCreated(info, { key->id() }, mProd);
	}
    }
    
    /***************
     *   URange    *
     ***************/
    
    template <typename MetaT>
    URange<MetaT>::URange(const Vector<MetaT>& space) :
	RangeInterface<URange<MetaT>>(),
	mSpace(space)
    {
	std::sort(mSpace.begin(), mSpace.end(), std::less<MetaT>());
	auto itdupl = std::adjacent_find(mSpace.begin(), mSpace.end());
	CXZ_ASSERT(itdupl == mSpace.end(), "found duplicate: " << *itdupl);
    }

    template <typename MetaT>
    URange<MetaT>::URange(Vector<MetaT>&& space) :
	RangeInterface<URange<MetaT>>(),
	mSpace(space)
    {
	std::sort(mSpace.begin(), mSpace.end(), std::less<MetaT>());
	auto itdupl = std::adjacent_find(mSpace.begin(), mSpace.end());
	CXZ_ASSERT(itdupl == mSpace.end(), "found duplicate: " << toString(*itdupl));
    }

        
    template <typename MetaT>
    const MetaT& URange<MetaT>::get(SizeT pos) const
    {
	return mSpace[pos];
    }

    template <typename MetaT>
    const MetaT* URange<MetaT>::get() const
    {
	return mSpace.data();
    }

    template <typename MetaT>
    SizeT URange<MetaT>::getMeta(const MetaT& meta) const
    {
	auto b = mSpace.begin();
	auto e = mSpace.end();
	return std::lower_bound(b, e, meta, std::less<MetaT>()) - b;
    }

    template <typename MetaT>
    SizeT URange<MetaT>::size() const
    {
	return mSpace.size();
    }

    template <typename MetaT>
    SizeT URange<MetaT>::dim() const
    {
	return 1;
    }

    template <typename MetaT>
    String URange<MetaT>::stringMeta(SizeT pos) const
    {
	return toString(this->get(pos));
    }
    
    template <typename MetaT>
    const TypeInfo& URange<MetaT>::type() const
    {
	return typeid(URange<MetaT>);
    }
    
    template <typename MetaT>
    const TypeInfo& URange<MetaT>::metaType() const
    {
	return typeid(MetaT);
    }

    template <typename MetaT>
    RangePtr URange<MetaT>::extend(const RangePtr& r) const
    {
	auto rx = rangeCast<URange<MetaT>>(r);
	auto space = mSpace;
	space.insert(space.end(), rx->mSpace.begin(), rx->mSpace.end());
	return URangeFactory<MetaT>( space ).create();
    }

    template <typename MetaT>
    Vector<Uuid> URange<MetaT>::key() const
    {
	return Vector<Uuid> { this->id() };
    }

    /*******************
     *   Range Casts   *
     *******************/

    template <typename MetaT>
    struct URangeCast
    {
	template <typename T>
	static inline Sptr<URange<MetaT>> transform(const RangePtr& r)
	{
	    if(r->type() == typeid(URange<T>)){
		auto rr = std::dynamic_pointer_cast<URange<T>>(r);
		Vector<MetaT> v(rr->size());
		std::transform(rr->begin(), rr->end(), v.begin(),
			       [](const T& x) { return static_cast<MetaT>(x); } );
		return std::dynamic_pointer_cast<URange<MetaT>>
		    ( URangeFactory<MetaT>(std::move(v)).create() );
	    }
	    else {
		return nullptr;
	    }
	}
	
	static inline Sptr<URange<MetaT>> cast(const RangePtr& r)
	{
	    static_assert(std::is_fundamental<MetaT>::value, "got non-fundamental type");
	    CXZ_ASSERT(r->dim() == 1, "range cast into URange<Int>: source range must have dim = 1, got " << r->dim());
	    Sptr<URange<MetaT>> o = nullptr;
	    o = transform<SizeT>(r); if(o) return o;
	    o = transform<Int>(r); if(o) return o;
	    o = transform<LInt>(r); if(o) return o;
	    o = transform<Double>(r); if(o) return o;
	    // else general transform using DType (better than nothing), to be implemented!!!
	    CXZ_ERROR("no range cast available for input range '" << r->type().name() << "'");
	    return nullptr;
	}
    };

    template <typename T> struct is_vector : std::false_type {};
    template <typename T, typename A> struct is_vector<std::vector<T,A>> : std::true_type {};

    template <typename U>
    struct URangeCast<Vector<U>>
    {
	template <typename T>
	static inline Sptr<URange<Vector<U>>> transform(const RangePtr& r)
	{
	    if(r->type() == typeid(URange<T>)){
		auto rr = std::dynamic_pointer_cast<URange<T>>(r);
		Vector<Vector<U>> v(rr->size());
		std::transform(rr->begin(), rr->end(), v.begin(),
			       [](const T& x) { return Vector<U> { static_cast<U>(x) }; } );
		return std::dynamic_pointer_cast<URange<Vector<U>>>
		    ( URangeFactory<Vector<U>>(std::move(v)).create() );
	    }
	    else {
		return nullptr;
	    }
	}

	template <typename T, SizeT N>
	static inline Sptr<URange<Vector<U>>> atransform(const RangePtr& r)
	{
	    if(r->type() == typeid(URange<Arr<T,N>>)){
		auto rr = std::dynamic_pointer_cast<URange<Arr<T,N>>>(r);
		Vector<Vector<U>> v(rr->size());
		std::transform(rr->begin(), rr->end(), v.begin(),
			       [](const Arr<T,N>& x) {
				   return iter<0,N>( [&](auto i) { return static_cast<U>(x[i]); },
						     [](const auto&... e) { return Vector<U>{ e... }; });
			       } );
		return std::dynamic_pointer_cast<URange<Vector<U>>>
		    ( URangeFactory<Vector<U>>(std::move(v)).create() );
	    }
	    else {
		return nullptr;
	    }
	}

	static inline Sptr<URange<Vector<U>>> cast(const RangePtr& r)
	{
	    Sptr<URange<Vector<U>>> o = nullptr;
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
    Sptr<URange<MetaT>> RangeCast<URange<MetaT>>::func(const RangePtr& r)
    {
	if constexpr(std::is_fundamental<MetaT>::value or is_vector<MetaT>::value){
	    return URangeCast<MetaT>::cast(r);
	}
	else {
	    CXZ_ERROR("no range cast available for input range '" << r->type().name() << "'");
	    return nullptr;
	}
    }

    template <typename MetaT>
    RangePtr urange(const Vector<MetaT>& space)
    {
	return URangeFactory<MetaT>(space).create();
    }
    
}

#endif
