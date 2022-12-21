// -*- C++ -*-

#ifndef __cxz_index_base_h__
#define __cxz_index_base_h__

#include "base/base.h"
#include "range_base.h"
#include "xpr/index_id.h"
#include "xpr/xpr_base.h"
#include "xpr/func.h"

namespace CNORXZ
{
    template <class I, typename MetaType>
    class IndexInterface
    {
    public:
	~IndexInterface() = default; 
	
	I& THIS() { return static_cast<I&>(*this); }
	const I& THIS() const { return static_cast<const I&>(*this); }

	I& operator++() { return ++THIS(); }
	I& operator--() { return --THIS();}
	I operator+(Int n) const { return THIS() + n; }
	I operator-(Int n) const { return THIS() - n; }
	I& operator+=(Int n) { return THIS() += n; }
	I& operator-=(Int n) { return THIS() -= n; }
	Int operator-(const IndexInterface& i) const { return lex() - i.lex(); }
	
	SizeT pos() const; // 'memory' pos
	SizeT lex() const { return THIS().lex(); } // lexicographic pos
	SizeT pmax() const { return static_cast<SizeT>(THIS().pmax()); } // memory pos max
	SizeT lmax() const { return static_cast<SizeT>(THIS().lmax()); } // lexicographic pos max
	PtrId ptrId() const;
	decltype(auto) id() const { return THIS().id(); }

	bool operator==(const IndexInterface& in) const;
	bool operator!=(const IndexInterface& in) const;
	bool operator<(const IndexInterface& in) const;
	bool operator>(const IndexInterface& in) const;
	bool operator<=(const IndexInterface& in) const;
	bool operator>=(const IndexInterface& in) const;

	decltype(auto) operator*() const { return THIS().operator*(); }

	SizeT dim() const { return THIS().dim(); }
	decltype(auto) range() const { return THIS().range(); }

	template <SizeT J>
	decltype(auto) stepSize(const IndexId<J>& id) const { return THIS().stepSize(id); }
	
	String stringMeta() const { return THIS().stringMeta(); }
	decltype(auto) meta() const { return THIS().meta(); }
	I& at(const MetaType& meta) { return THIS().at(meta); }

	template <class Index>
	decltype(auto) format(const Sptr<Index>& ind) const { return THIS().format(ind); }

	template <class Index>
	decltype(auto) slice(const Sptr<Index>& ind) const { return THIS().slice(ind); }
	
	template <class Xpr, class F = NoF>
	decltype(auto) ifor(const Xpr& xpr, F&& f) const
	{ return THIS().ifor(xpr,std::forward<F>(f)); }
	
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

    template <class I>
    struct is_index
    { static constexpr bool value = std::is_base_of<IndexInterface<I,typename I::MetaType>,I>::value; };
    
    template <class I>
    struct index_has_const_size
    { static constexpr bool value = false; };

    template <class I>
    struct index_const_size
    { static constexpr SizeT value = 0; };

    template <class I>
    struct index_dim
    { static constexpr SizeT value = 1; };

    template <class T>
    struct has_sub
    { static constexpr bool value = false; };

    template <class T>
    struct has_static_sub
    { static constexpr bool value = false; };

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
