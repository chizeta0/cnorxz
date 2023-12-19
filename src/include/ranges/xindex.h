// -*- C++ -*-
/**
   
   @file include/ranges/xindex.h
   @brief ...


   Copyright (c) 2022 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_xindex_h__
#define __cxz_xindex_h__

#include "base/base.h"
#include "range_base.h"
#include "index_base.h"
#include "xpr/xpr.h"

namespace CNORXZ
{
    class XIndexBase
    {
    public:

	//typedef DType MetaType;
	
	DEFAULT_MEMBERS(XIndexBase);
	virtual ~XIndexBase() = default;
	virtual XIndexPtr copy() const = 0;
	virtual SizeT pos() const = 0;
	
	virtual XIndexBase& operator=(SizeT lexpos) = 0;
	virtual XIndexBase& operator++() = 0;
	virtual XIndexBase& operator--() = 0;
	virtual XIndexPtr operator+(Int n) const = 0;
	virtual XIndexPtr operator-(Int n) const = 0;
	virtual SizeT operator-(const XIndexBase& i) const = 0;
	virtual XIndexBase& operator+=(Int n) = 0;
	virtual XIndexBase& operator-=(Int n) = 0;

	virtual SizeT lex() const = 0;
	virtual UPos pmax() const = 0;
	virtual UPos lmax() const = 0;
	virtual IndexId<0> id() const = 0;
	
	virtual DType operator*() const = 0;

	virtual SizeT dim() const = 0;
	virtual RangePtr range() const = 0;
	virtual UPos stepSize(const IndexId<0>& id) const = 0;
	virtual RangePtr prange(const XIndexPtr& last) const = 0;
	virtual Vector<SizeT> deepFormat() const = 0;
	virtual Vector<SizeT> deepMax() const = 0;
	virtual XIndexPtr reformat(const Vector<SizeT>& f, const Vector<SizeT>& s) const = 0;

	virtual String stringMeta() const = 0;
	virtual DType meta() const = 0;
	virtual XIndexBase& at(const DType& meta) = 0;

	virtual DXpr<None> ifor(const DXpr<None>& xpr, NoF&& f) const = 0;

	virtual bool formatIsTrivial() const = 0;
    };

    //Sptr<XIndexBase>& operator++(Sptr<XIndexBase>& i);
    //Sptr<XIndexBase>& operator--(Sptr<XIndexBase>& i);
    
    // MultiIndex Wrapper:
    template <class Index, typename Meta>
    class XIndex : public XIndexBase
    {
    public:

	DEFAULT_C(XIndex);
	// no default copy/assignment (have to copy objects in shared ptr)
	XIndex(const XIndex& i);
	XIndex(XIndex&& i);
	XIndex& operator=(const XIndex& i);
	XIndex& operator=(XIndex&& i);
	XIndex(const IndexPtr<Index,Meta>& i);
	XIndex(const IndexInterface<Index,Meta>& i);

	virtual XIndexPtr copy() const override final;
	virtual SizeT pos() const override final;

	virtual XIndex& operator=(SizeT lexpos) override final;
	virtual XIndex& operator++() override final;
	virtual XIndex& operator--() override final;
	virtual XIndexPtr operator+(Int n) const override final;
	virtual XIndexPtr operator-(Int n) const override final;
	virtual SizeT operator-(const XIndexBase& i) const override final;
	virtual XIndex& operator+=(Int n) override final;
	virtual XIndex& operator-=(Int n) override final;

	virtual SizeT lex() const override final;
	virtual UPos pmax() const override final;
	virtual UPos lmax() const override final;
	virtual IndexId<0> id() const override final;
	
	virtual DType operator*() const override final;

	virtual SizeT dim() const override final;
	virtual RangePtr range() const override final;
	virtual UPos stepSize(const IndexId<0>& id) const override final;
	virtual RangePtr prange(const XIndexPtr& last) const override final;
	virtual Vector<SizeT> deepFormat() const override final;
	virtual Vector<SizeT> deepMax() const override final;
	virtual XIndexPtr reformat(const Vector<SizeT>& f, const Vector<SizeT>& s) const override final;

	virtual String stringMeta() const override final;
	virtual DType meta() const override final;
	virtual XIndexBase& at(const DType& meta) override final;

	virtual DXpr<None> ifor(const DXpr<None>& xpr, NoF&& f) const override final;

	virtual bool formatIsTrivial() const override final;

	Index& get();
	const Index& get() const;

    private:
	IndexPtr<Index,Meta> mI;

    };

    template <>
    struct has_sub<XIndexBase>
    { static constexpr bool value = true; };
    
    template <class Index>
    inline XIndexPtr xindexPtr(const Sptr<Index>& i);

    template <>
    inline XIndexPtr xindexPtr<XIndexBase>(const Sptr<XIndexBase>& i);

}

#endif
