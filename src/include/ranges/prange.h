// -*- C++ -*-
/**
   
   @file include/ranges/prange.h
   @brief ...


   Copyright (c) 2022 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_prange_h__
#define __cxz_prange_h__

#include "base/base.h"
#include "ranges/index_base.h"
#include "ranges/range_base.h"
#include "xpr/xpr.h"

namespace CNORXZ
{

    template <class IndexT>
    class PIndex : public IndexInterface<PIndex<IndexT>,typename IndexT::MetaType>
    {
    public:

	typedef IndexInterface<PIndex<IndexT>,typename IndexT::MetaType> IB;
	typedef PRange<typename IndexT::RangeType> RangeType;
	typedef typename IndexT::MetaType MetaType;

	PIndex(const RangePtr& range, SizeT pos = 0);
	
	PIndex& operator=(SizeT lexpos);
	PIndex& operator++();
	PIndex& operator--();
	PIndex operator+(Int n) const;
	PIndex operator-(Int n) const;
	SizeT operator-(const PIndex& i) const;
	PIndex& operator+=(Int n);
	PIndex& operator-=(Int n);

	SizeT lex() const;
	UPos pmax() const;
	UPos lmax() const;
	IndexId<0> id() const;
	
	decltype(auto) operator*() const;
	
	SizeT dim() const;
	Sptr<RangeType> range() const;

	template <SizeT I>
	UPos stepSize(const IndexId<I>& id) const;
	RangePtr prange(const PIndex<IndexT>& last) const;
	decltype(auto) deepFormat() const;
	decltype(auto) deepMax() const;

	/** @copydoc IndexInterface::reformat() */
	PIndex& reformat(const Vector<SizeT>& f, const Vector<SizeT>& s);

	String stringMeta() const;
	decltype(auto) meta() const;
	PIndex& at(const MetaType& metaPos);
	decltype(auto) xpr(const Sptr<PIndex<IndexT>>& _this) const;

	template <class Xpr, class F>
	decltype(auto) ifor(const Xpr& xpr, F&& f) const;

	PIndex& operator()();
	PIndex& operator()(const Sptr<IndexT>& i);
	const Sptr<IndexT>& orig() const;
	
	/** @copydoc IndexInterface::formatIsTrivial() */
	bool formatIsTrivial() const;

    private:
	Sptr<RangeType> mRangePtr;
	Sptr<IndexT> mOrig;

	void mkPos();
    };

    template <class I, class I1>
    decltype(auto) operator*(const Sptr<PIndex<I>>& a, const Sptr<I1>& b);

    template <class RangeT>
    class PRangeFactory : public RangeFactoryBase
    {
    public:
	PRangeFactory(const Sptr<RangeT>& range, const Vector<SizeT>& _parts);
	PRangeFactory(const RangePtr& range, const Vector<SizeT>& _parts);
	
    private:
	PRangeFactory() = default;
	virtual void make() override final;

	Sptr<RangeT> mRange;
	Vector<SizeT> mParts;
    };
    
    template <class RangeT>
    class PRange : public RangeInterface<PRange<RangeT>>
    {
    public:
	typedef RangeBase RB;
	typedef PIndex<typename RangeT::IndexType> IndexType;
	typedef typename RangeT::MetaType MetaType;
	
	friend PRangeFactory<RangeT>;

	virtual SizeT size() const override final;
	virtual SizeT dim() const override final;
	virtual String stringMeta(SizeT pos) const override final;
	virtual const TypeInfo& type() const override final;
	virtual const TypeInfo& metaType() const override final;
	virtual RangePtr extend(const RangePtr& r) const override final;

	Sptr<RangeT> orig() const;
	const Vector<SizeT>& parts() const;
	RangePtr derive() const;
	
    private:

	PRange() = delete;
	PRange(const PRange& in) = delete;
	PRange(const Sptr<RangeT>& range, const Vector<SizeT>& _parts);

	virtual Vector<Uuid> key() const override final;
	
	Sptr<RangeT> mRange;
	Vector<SizeT> mParts;
    };

    template <class RangeT>
    RangePtr prange(const Sptr<RangeT>& range, const Vector<SizeT>& parts);
    
} // namespace CNORXZ

#endif
