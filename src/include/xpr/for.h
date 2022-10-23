
#ifndef __cxz_for_h__
#define __cxz_for_h__

#include "base/base.h"
#include "xpr_base.h"

namespace CNORXZ
{
    class ZeroF
    {
    public:
	template <typename... T>
	constexpr decltype(auto) operator()(const T&... as) const;
    };

    class NoF
    {
    public:
	template <typename... T>
	constexpr decltype(auto) operator()(const T&... as) const;
    };

    template <SizeT L, class Xpr, class F = NoF>
    class For : public XprInterface<For<L,Xpr,F>>
    {
    public:
	DEFAULT_MEMBERS(For);

	constexpr For(SizeT size, const IndexId<L>& id, const Xpr& xpr, F&& f);

	template <class PosT>
	inline decltype(auto) operator()(const PosT& last) const;
	
	inline decltype(auto) operator()() const;

	template <SizeT I>
	inline decltype(auto) rootSteps(const IndexId<I>& id) const;

    private:
	SizeT mSize = 0;
	IndexId<L> mId;
	Xpr mXpr;
	typedef decltype(mXpr.rootSteps(mId)) XPosT;
	XPosT mExt;
	F mF;
    };

    
    // unrolled loop:
    template <SizeT N, SizeT L, class Xpr, class F = NoF>
    class SFor : public XprInterface<SFor<N,L,Xpr,F>>
    {
    public:
	DEFAULT_MEMBERS(SFor);

	constexpr SFor(const IndexId<L>& id, const Xpr& xpr, F&& f);

	template <class PosT>
	constexpr decltype(auto) operator()(const PosT& last) const;
	
	constexpr decltype(auto) operator()() const;

	template <SizeT I>
	constexpr decltype(auto) rootSteps(const IndexId<I>& id) const;
	
    private:

	template <SizeT I, class PosT>
	constexpr decltype(auto) exec(const PosT& last) const;

	template <SizeT I>
	constexpr decltype(auto) exec() const;

	IndexId<L> mId;
	Xpr mXpr;
	typedef decltype(mXpr.RootSteps(mId)) XPosT;
	XPosT mExt;
	F mF;
	
    };


    // multi-threading
    template <SizeT L, class Xpr, class F = NoF>
    class TFor : public XprInterface<TFor<L,Xpr,F>>
    {
    public:
	DEFAULT_MEMBERS(TFor);

	constexpr TFor(SizeT size, const IndexId<L>& id, const Xpr& xpr, F&& f);

	template <class PosT>
	inline decltype(auto) operator()(const PosT& last) const;
	
	inline decltype(auto) operator()() const;

	template <SizeT I>
	inline decltype(auto) rootSteps(const IndexId<I>& id) const;
	
    private:
	SizeT mSize = 0;
	IndexId<L> mId;
	Xpr mXpr;
	typedef decltype(mXpr.rootSteps(mId)) XPosT;
	XPosT mExt;
	F mF;
    };

    // Extension For (Vectorization)
    template <SizeT N, SizeT L, class Xpr>
    class EFor : public XprInterface<EFor<N,L,Xpr>>
    {
    public:
	DEFAULT_MEMBERS(EFor);

	constexpr EFor(const IndexId<L>& id, const Xpr& xpr);

	template <class PosT>
	constexpr decltype(auto) operator()(const PosT& last) const;

	constexpr decltype(auto) operator()() const;

	template <SizeT I>
	constexpr decltype(auto) rootSteps(const IndexId<I>& id) const;

    private:
	IndexId<L> mId;
	Xpr mXpr;
	typedef decltype(mXpr.rootSteps(mId)) XPosT;
	XPosT mExt;
    };
    
}

#endif
