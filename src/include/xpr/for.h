
#ifndef __cxz_for_h__
#define __cxz_for_h__

#include "base/base.h"
#include "xpr_base.h"

namespace CNORXZ
{

    template <SizeT L, class Xpr>
    class For : public XprInterface<For<L,Xpr>>
    {
    public:
	DEFAULT_MEMBERS(For);

	constexpr For(SizeT size, const IndexId<L>& id, const Xpr& xpr);

	template <class PosT>
	inline SizeT operator()(const PosT& last) const;
	
	inline SizeT operator()() const;

	template <SizeT I>
	inline decltype(auto) rootSteps(const IndexId<I>& id) const;

    private:
	SizeT mSize = 0;
	IndexId<L> mId;
	Xpr mXpr;
	typedef decltype(mXpr.rootSteps(mId)) XPosT;
	XPosT mExt;
	
    };

    // unrolled loop:
    template <SizeT N, SizeT L, class Xpr>
    class SFor : public XprInterface<SFor<N,L,Xpr>>
    {
    public:
	DEFAULT_MEMBERS(SFor);

	constexpr SFor(const IndexId<L>& id, const Xpr& xpr);

	template <class PosT>
	constexpr SizeT operator()(const PosT& last) const;
	
	constexpr SizeT operator()() const;

	template <SizeT I>
	constexpr decltype(auto) rootSteps(const IndexId<I>& id) const;
	
    private:

	template <SizeT I, class PosT>
	constexpr SizeT exec(const PosT& last) const;

	template <SizeT I>
	constexpr SizeT exec() const;

	IndexId<L> mId;
	Xpr mXpr;
	typedef decltype(mXpr.RootSteps(mId)) XPosT;
	XPosT mExt;
	
    };


    // multi-threading
    template <SizeT L, class Xpr>
    class TFor : public XprInterface<TFor<L,Xpr>>
    {
    public:
	DEFAULT_MEMBERS(TFor);

	constexpr TFor(SizeT size, const IndexId<L>& id, const Xpr& xpr);

	template <class PosT>
	inline SizeT operator()(const PosT& last) const;
	
	inline SizeT operator()() const;

	template <SizeT I>
	inline decltype(auto) rootSteps(const IndexId<I>& id) const;
	
    private:
	SizeT mSize = 0;
	IndexId<L> mId;
	Xpr mXpr;
	typedef decltype(mXpr.rootSteps(mId)) XPosT;
	XPosT mExt;
	
    };

    // Extension For (Vectorization)
    template <SizeT N, SizeT L, class Xpr>
    class EFor : public XprInterface<EFor<N,L,Xpr>>
    {
    public:
	DEFAULT_MEMBERS(EFor);

	constexpr EFor(const IndexId<L>& id, const Xpr& xpr);

	template <class PosT>
	constexpr SizeT operator()(const PosT& last) const;

	constexpr SizeT operator()() const;

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
