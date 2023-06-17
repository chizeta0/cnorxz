
#ifndef __cxz_urange_cc_h__
#define __cxz_urange_cc_h__

#include <functional>
#include <algorithm>

#include "urange.h"
#include "index_mul.h"
#include "xpr/for.h"
#include "operation/op_types.h"
#include "operation/op_types.cc.h"

namespace CNORXZ
{
    /*****************
     *    UIndex     *	     
     *****************/

    template <typename MetaType>
    UIndex<MetaType>::UIndex(const RangePtr& range, SizeT pos) :
	IndexInterface<UIndex<MetaType>,MetaType>(pos),
	mRangePtr(rangeCast<RangeType>(range)),
	mMetaPtr(&mRangePtr->get(0))
    {}

    template <typename MetaType>
    UIndex<MetaType>& UIndex<MetaType>::operator=(size_t lexpos)
    {
	IB::mPos = lexpos;
	return *this;
    }

    template <typename MetaType>
    UIndex<MetaType>& UIndex<MetaType>::operator++()
    {
	++IB::mPos;
	return *this;
    }

    template <typename MetaType>
    UIndex<MetaType>& UIndex<MetaType>::operator--()
    {
	--IB::mPos;
	return *this;
    }

    template <typename MetaType>
    UIndex<MetaType> UIndex<MetaType>::operator+(Int n) const
    {
	return UIndex(mRangePtr, IB::mPos + n);
    }
    
    template <typename MetaType>
    UIndex<MetaType> UIndex<MetaType>::operator-(Int n) const
    {
	return UIndex(mRangePtr, IB::mPos - n);
    }
    
    template <typename MetaType>
    UIndex<MetaType>& UIndex<MetaType>::operator+=(Int n)
    {
	IB::mPos += n;
	return *this;
    }
    
    template <typename MetaType>
    UIndex<MetaType>& UIndex<MetaType>::operator-=(Int n)
    {
	IB::mPos -= n;
	return *this;
    }

    template <typename MetaType>
    SizeT UIndex<MetaType>::lex() const
    {
	return IB::mPos;
    }
    
    template <typename MetaType>
    UPos UIndex<MetaType>::pmax() const
    {
	return UPos(mRangePtr->size());
    }

    template <typename MetaType>
    UPos UIndex<MetaType>::lmax() const
    {
	return UPos(mRangePtr->size());
    }

    template <typename MetaType>
    IndexId<0> UIndex<MetaType>::id() const
    {
	return IndexId<0>(this->ptrId());
    }

    template <typename MetaType>
    const MetaType& UIndex<MetaType>::operator*() const
    {
	return mMetaPtr[IB::mPos];
    }
    
    template <typename MetaType>
    String UIndex<MetaType>::stringMeta() const
    {
	return toString(this->meta());
    }
    
    template <typename MetaType>
    const MetaType& UIndex<MetaType>::meta() const
    {
	return mMetaPtr[IB::mPos];
    }
    
    template <typename MetaType>
    UIndex<MetaType>& UIndex<MetaType>::at(const MetaType& metaPos)
    {
	(*this) = mRangePtr->getMeta(metaPos);
	return *this;
    }

    template <typename MetaType>
    decltype(auto) UIndex<MetaType>::xpr(const Sptr<UIndex<MetaType>>& _this) const
    {
	return coproot(mMetaPtr,_this);
    }

    template <typename MetaType>
    size_t UIndex<MetaType>::dim() const // = 1
    {
	return 1;
    }

    template <typename MetaType>
    Sptr<URange<MetaType>> UIndex<MetaType>::range() const
    {
	return mRangePtr;
    }

    template <typename MetaType>
    template <SizeT I>
    decltype(auto) UIndex<MetaType>::stepSize(const IndexId<I>& id) const
    {
	if constexpr(I != 0){
	    return SPos<0>();
	}
	else {
	    return UPos(id == this->id() ? 1 : 0);
	}
    }

    template <typename MetaType>
    template <class Index>
    decltype(auto) UIndex<MetaType>::reformat(const Sptr<Index>& ind) const
    {
	return ind;
    }

    template <typename MetaType>
    template <class Index>
    decltype(auto) UIndex<MetaType>::slice(const Sptr<Index>& ind) const
    {
	if(ind != nullptr){
	    if(ind->dim() != 0) {
		return Sptr<UIndex<MetaType>>();
	    }
	}
	return std::make_shared<UIndex<MetaType>>(*this);
    }
    
    template <typename MetaType>
    template <class Xpr, class F>
    decltype(auto) UIndex<MetaType>::ifor(const Xpr& xpr, F&& f) const
    {
	return For<0,Xpr,F>(this->pmax().val(), this->id(), xpr, std::forward<F>(f));
    }
    
    template <typename MetaType, class I1>
    decltype(auto) operator*(const Sptr<UIndex<MetaType>>& a, const Sptr<I1>& b)
    {
	return iptrMul(a, b);
    }

    /**********************
     *   URangeFactory    *
     **********************/

    template <typename MetaType>
    URangeFactory<MetaType>::URangeFactory(const Vector<MetaType>& space) :
	mSpace(space) {}

    template <typename MetaType>
    URangeFactory<MetaType>::URangeFactory(Vector<MetaType>&& space) :
	mSpace(space) {}

    template <typename MetaType>
    URangeFactory<MetaType>::URangeFactory(const Vector<MetaType>& space, const RangePtr& ref) :
	mSpace(space), mRef(ref) {}

    template <typename MetaType>
    URangeFactory<MetaType>::URangeFactory(Vector<MetaType>&& space, const RangePtr& ref) :
	mSpace(space), mRef(ref) {}

    template <typename MetaType>
    void URangeFactory<MetaType>::make()
    {
	const auto& info = typeid(URange<MetaType>);
	if(mRef != nullptr) {
	    mProd = this->fromCreated(info, {mRef->id()});
	}
	if(mProd == nullptr){
	    RangePtr key = mProd = std::shared_ptr<URange<MetaType>>
		( new URange<MetaType>( std::move(mSpace) ) );
	    if(mRef != nullptr) { key = mRef; }
	    this->addToCreated(info, { key->id() }, mProd);
	}
    }
    
    /***************
     *   URange    *
     ***************/
    
    template <typename MetaType>
    URange<MetaType>::URange(const Vector<MetaType>& space) :
	RangeInterface<URange<MetaType>>(),
	mSpace(space)
    {
	std::sort(mSpace.begin(), mSpace.end(), std::less<MetaType>());
	auto itdupl = std::adjacent_find(mSpace.begin(), mSpace.end());
	CXZ_ASSERT(itdupl == mSpace.end(), "found duplicate: " << *itdupl);
    }

    template <typename MetaType>
    URange<MetaType>::URange(Vector<MetaType>&& space) :
	RangeInterface<URange<MetaType>>(),
	mSpace(space)
    {
	std::sort(mSpace.begin(), mSpace.end(), std::less<MetaType>());
	auto itdupl = std::adjacent_find(mSpace.begin(), mSpace.end());
	CXZ_ASSERT(itdupl == mSpace.end(), "found duplicate: " << toString(*itdupl));
    }

        
    template <typename MetaType>
    const MetaType& URange<MetaType>::get(SizeT pos) const
    {
	return mSpace[pos];
    }

    template <typename MetaType>
    const MetaType* URange<MetaType>::get() const
    {
	return mSpace.data();
    }

    template <typename MetaType>
    SizeT URange<MetaType>::getMeta(const MetaType& meta) const
    {
	auto b = mSpace.begin();
	auto e = mSpace.end();
	return std::lower_bound(b, e, meta, std::less<MetaType>()) - b;
    }

    template <typename MetaType>
    SizeT URange<MetaType>::size() const
    {
	return mSpace.size();
    }

    template <typename MetaType>
    SizeT URange<MetaType>::dim() const
    {
	return 1;
    }

    template <typename MetaType>
    String URange<MetaType>::stringMeta(SizeT pos) const
    {
	return toString(this->get(pos));
    }
    
    template <typename MetaType>
    const TypeInfo& URange<MetaType>::type() const
    {
	return typeid(URange<MetaType>);
    }
    
    template <typename MetaType>
    const TypeInfo& URange<MetaType>::metaType() const
    {
	return typeid(MetaType);
    }

    template <typename MetaType>
    RangePtr URange<MetaType>::extend(const RangePtr& r) const
    {
	auto rx = rangeCast<URange<MetaType>>(r);
	auto space = mSpace;
	space.insert(space.end(), rx->mSpace.begin(), rx->mSpace.end());
	return URangeFactory<MetaType>( space ).create();
    }

    template <typename MetaType>
    Vector<Uuid> URange<MetaType>::key() const
    {
	return Vector<Uuid> { this->id() };
    }

    /*******************
     *   Range Casts   *
     *******************/

    template <typename MetaType>
    struct URangeCast
    {
	template <typename T>
	static inline Sptr<URange<MetaType>> transform(const RangePtr& r)
	{
	    if(r->type() == typeid(URange<T>)){
		auto rr = std::dynamic_pointer_cast<URange<T>>(r);
		Vector<MetaType> v(rr->size());
		std::transform(rr->begin(), rr->end(), v.begin(),
			       [](const T& x) { return static_cast<MetaType>(x); } );
		return std::dynamic_pointer_cast<URange<MetaType>>
		    ( URangeFactory<MetaType>(std::move(v)).create() );
	    }
	    else {
		return nullptr;
	    }
	}
	
	static inline Sptr<URange<MetaType>> cast(const RangePtr& r)
	{
	    static_assert(std::is_fundamental<MetaType>::value, "got non-fundamental type");
	    CXZ_ASSERT(r->dim() == 1, "range cast into URange<Int>: source range must have dim = 1, got " << r->dim());
	    Sptr<URange<MetaType>> o = nullptr;
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

    
    template <typename MetaType>
    Sptr<URange<MetaType>> RangeCast<URange<MetaType>>::func(const RangePtr& r)
    {
	if constexpr(std::is_fundamental<MetaType>::value or is_vector<MetaType>::value){
	    return URangeCast<MetaType>::cast(r);
	}
	else {
	    CXZ_ERROR("no range cast available for input range '" << r->type().name() << "'");
	    return nullptr;
	}
    }
    
}

#endif
