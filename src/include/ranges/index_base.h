// -*- C++ -*-

#ifndef __cxz_index_base_h__
#define __cxz_index_base_h__

#include "base/base.h"
#include "range_base.h"
#include "xpr/index_id.h"
#include "xpr/xpr_base.h"

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
	I& operator++() { return ++THIS(); }
	I& operator--() { return --THIS();}
	I operator+(Int n) const { return THIS() + n; }
	I operator-(Int n) const { return THIS() - n; }
	I& operator+=(Int n) { return THIS() += n; }
	I& operator-=(Int n) { return THIS() -= n; }
	Int operator-(const IndexInterface& i) const { return mPos - i.mPos; }
	
	SizeT pos() const;
	SizeT max() const { return THIS().max(); }
	PtrId ptrId() const;
	decltype(auto) id() const { return THIS().id(); }

	bool operator==(const IndexInterface& in) const;
	bool operator!=(const IndexInterface& in) const;
	bool operator<(const IndexInterface& in) const;
	bool operator>(const IndexInterface& in) const;
	bool operator<=(const IndexInterface& in) const;
	bool operator>=(const IndexInterface& in) const;
	decltype(auto) operator*() const { return THIS().operator*(); }
	decltype(auto) operator->() const { return THIS().operator->(); }

	Int pp(PtrId idxPtrNum) { return THIS().pp(idxPtrNum); }
	Int mm(PtrId idxPtrNum) { return THIS().mm(idxPtrNum); }
	
	SizeT dim() const { return THIS().dim(); }
	decltype(auto) range() const { return THIS().range(); }

	template <SizeT J>
	decltype(auto) stepSize(const IndexId<J>& id) const { return THIS().stepSize(id); }
	
	String stringMeta() const { return THIS().stringMeta(); }
	decltype(auto) meta() const { return THIS().meta(); }
	I& at(const MetaType& meta) { return THIS().at(meta); }
	
	template <class PosT, class Xpr>
	decltype(auto) ifor(const PosT& step, const Xpr& xpr) const { return THIS().ifor(step,xpr); }
	
    protected:
	SizeT mPos = 0;
	
    private:
	friend I; // why not protected???!!!
	
	// NO DEFAULT CONSTRUCTORS/ASSIGNMENTS!
	IndexInterface();
	IndexInterface(const IndexInterface& in);
	IndexInterface& operator=(const IndexInterface& in);
	IndexInterface(IndexInterface&& in);
	IndexInterface& operator=(IndexInterface&& in);
	IndexInterface(SizeT pos);

	PtrId mPtrId = 0;
    };

    template <class I, typename MetaType>
    IndexPtr<I,MetaType>& operator++(const IndexPtr<I,MetaType>& i);

    template <class I, typename MetaType>
    IndexPtr<I,MetaType>& operator--(const IndexPtr<I,MetaType>& i);
    
    // to define relative indices:
    template <class I, typename MetaType>
    IndexPtr<I,MetaType> operator+(const IndexPtr<I,MetaType>& i, Int n);

    template <class I, typename MetaType>
    IndexPtr<I,MetaType> operator-(const IndexPtr<I,MetaType>& i, Int n);

}

#endif
