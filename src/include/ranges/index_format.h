// -*- C++ -*-
/**
   
   @file include/ranges/index_format.h
   @brief ...


   Copyright (c) 2022 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_index_format_h__
#define __cxz_index_format_h__

#include "base/base.h"
#include "xpr/xpr.h"

namespace CNORXZ
{
    template <class FormatT> struct is_static_format { CXZ_CVAL_FALSE; };
    
    template <SizeT N>
    class MFormat
    {
    public:
	typedef Arr<UPos,N> InputType;
	
	SP_DEFAULT_MEMBERS(constexpr,MFormat);
	explicit constexpr MFormat(const Arr<UPos,N>& b);

	template <class FormatT>
	constexpr MFormat(const FormatT& f);

	const Arr<UPos,N>& all() const;
	constexpr decltype(auto) size() const;
	
	template <SizeT I>
	constexpr decltype(auto) get(CSizeT<I> i) const;

	template <SizeT I>
	constexpr decltype(auto) operator[](CSizeT<I> i) const;

    private:
	Arr<UPos,N> mB;

    };

    template <SizeT N> struct is_static_format<MFormat<N>> { CXZ_CVAL_TRUE; };

    template <class... PosT>
    class GMFormat
    {
    public:
	typedef Tuple<PosT...> InputType;

	SP_DEFAULT_MEMBERS(constexpr,GMFormat);
	explicit constexpr GMFormat(const Tuple<PosT...>& b);
	explicit constexpr GMFormat(Tuple<PosT...>&& b);

	template <class FormatT>
	constexpr GMFormat(const FormatT& f);

	const Tuple<PosT...>& all() const;
	constexpr decltype(auto) size() const;

	template <SizeT I>
	constexpr decltype(auto) get(CSizeT<I> i) const;

	template <SizeT I>
	constexpr decltype(auto) operator[](CSizeT<I> i) const;
	
    private:
	Tuple<PosT...> mB;
    };

    template <class... PosT>
    constexpr decltype(auto) gmformat(const PosT&... ps);
    
    template <class... PosT> struct is_static_format<GMFormat<PosT...>> { CXZ_CVAL_TRUE; };

    class YFormat
    {
    public:
	typedef Vector<UPos> InputType;
	
	DEFAULT_MEMBERS(YFormat);
	explicit YFormat(const Vector<UPos>& b);

	template <class FormatT>
	YFormat(const FormatT& f);

	const Vector<UPos>& all() const;
	SizeT size() const;

	template <SizeT I>
	const UPos& get(CSizeT<I> i) const;
	
	template <SizeT I>
	const UPos& operator[](CSizeT<I> i) const;

	const UPos& get(SizeT i) const;
	const UPos& operator[](SizeT i) const;
	
    private:
	Vector<UPos> mB;
    };

    bool formatIsTrivial(const Vector<SizeT>& f, const Vector<SizeT>& s);
}

#endif
