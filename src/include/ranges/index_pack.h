// -*- C++ -*-
/**
   
   @file include/ranges/index_pack.h
   @brief ...


   Copyright (c) 2022 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_index_pack_h__
#define __cxz_index_pack_h__

#include "base/base.h"
#include "xindex.h"

namespace CNORXZ
{
    template <class... Indices>
    class SPack
    {
    public:
	SP_DEFAULT_MEMBERS(constexpr,SPack);
	constexpr SPack(const Sptr<Indices>&... is);
	constexpr SPack(const Tuple<Sptr<Indices>...>& is);

	constexpr const Tuple<Sptr<Indices>...>& all() const;

	constexpr SizeT size() const;
	
	template <SizeT I>
	constexpr decltype(auto) get(CSizeT<I> i) const;

	template <SizeT I>
	constexpr decltype(auto) operator[](CSizeT<I> i) const;

	template <class Index>
	constexpr decltype(auto) rmul(const Sptr<Index>& i) const;

	template <class Index>
	constexpr decltype(auto) lmul(const Sptr<Index>& i) const;

	template <class... Indices2>
	constexpr decltype(auto) mul(const SPack<Indices2...>& p) const;

	decltype(auto) mkRange() const;

	SizeT lex() const;
	SizeT pos() const;
	
    private:
	Tuple<Sptr<Indices>...> mIs;
    };

    template <class... Indices>
    constexpr decltype(auto) spack(const Indices&... inds);

    template <class... Indices>
    constexpr decltype(auto) spackp(const Sptr<Indices>&... inds);

    class DPack
    {
    public:
	DEFAULT_MEMBERS(DPack);
	explicit DPack(const Vector<XIndexPtr>& is);
	explicit DPack(Vector<XIndexPtr>&& is);

	template <class... Indices>
	explicit DPack(const SPack<Indices...>& p);
	
	const Vector<XIndexPtr>& all() const;
	SizeT size() const;
	const XIndexPtr& get(SizeT i) const;
	const XIndexPtr& operator[](SizeT i) const;
	DPack rmul(const Sptr<DIndex>& i) const;
	DPack lmul(const Sptr<DIndex>& i) const;
	DPack mul(const DPack& p) const;
	RangePtr mkRange() const;
	SizeT lex() const;
	SizeT pos() const;
	
    private:
	Vector<XIndexPtr> mIs;
    };

    template <class... Indices>
    DPack dpack(const Indices&... inds);

    template <class... Indices>
    DPack dpackp(const Sptr<Indices>&... inds);
}

#endif
