// -*- C++ -*-

#ifndef __cxz_index_base_h__
#define __cxz_index_base_h__

#include "base/base.h"
#include "range_base.h"
#include "xfor/xfor.h"

namespace CNORXZ
{
    
    template <class I, typename MetaType>
    class IndexInterface
    {
    public:
	~IndexInterface() = default; 
	
	I& THIS() { return static_cast<I&>(*this); }
	const I& THIS() const { return static_cast<const I&>(*this); }

	I& operator=(SizeT pos) { return THIS() = pos; }
	I& operator++() { return THIS()++; }
	I& operator--() { return THIS()--;}
	I operator+(Int n) const { return THIS() + n; }
	I operator-(Int n) const { return THIS() - n; }
	I& operator+=(Int n) { return THIS() += n; }
	I& operator-=(Int n) { return THIS() -= n; }
	Int operator-(const IndexInterface& i) const { return mPos - i.mPos; }
	
	SizeT pos() const;
	SizeT max() const;
	PtrId ptrId() const;

	bool operator==(const IndexInterface& in) const;
	bool operator!=(const IndexInterface& in) const;
	bool operator<(const IndexInterface& in) const;
	bool operator>(const IndexInterface& in) const;
	bool operator<=(const IndexInterface& in) const;
	bool operator>=(const IndexInterface& in) const;
	auto operator*() const { return THIS().operator*(); }
	auto operator->() const { return THIS().operator->(); }

	Int pp(PtrId idxPtrNum) { return THIS().pp(idxPtrNum); }
	Int mm(PtrId idxPtrNum) { return THIS().mm(idxPtrNum); }
	
	SizeT dim() const { return THIS().dim(); }
	auto range() const { return THIS().range(); }
	SizeT getStepSize(SizeT n) const { return THIS().getStepSize(n); }
	
	String stringMeta() const { return THIS().stringMeta(); }
	MetaType meta() const { return THIS().meta(); }
	I& at(const MetaType& meta) { return THIS().at(meta); }

	template <class Expr>
	auto ifor(SizeT step, const Expr ex) const
	{ return THIS().template ifor<Expr>(step,ex); }
	
	template <class Expr>
	auto iforh(SizeT step, const Expr ex) const
	{ return THIS().template iforh<Expr>(step,ex); }

    private:

	friend I;
	
	// NO DEFAULT CONSTRUCTORS/ASSIGNMENTS!
	IndexInterface();
	IndexInterface(const IndexInterface& in);
	IndexInterface& operator=(const IndexInterface& in);
	IndexInterface(IndexInterface&& in);
	IndexInterface& operator=(IndexInterface&& in);
	IndexInterface(const RangePtr& range, SizeT pos);

	IndexPtr mRel = nullptr;
	SizeT mPos = 0;
	SizeT mMax = 0;
	PtrId mPtrId = 0;
    };

    // to define relative indices:
    template <class I, typename MetaType>
    IndexPtr<I,MetaType> operator+(const IndexPtr<I,MetaType>& i, Int n);

    template <class I, typename MetaType>
    IndexPtr<I,MetaType> operator-(const IndexPtr<I,MetaType>& i, Int n);

}

#endif
