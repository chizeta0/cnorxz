
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

	constexpr For(SizeT size, const IndexId<L>& id, SizeT step, const Xpr& xpr);

	template <class PosT1, class PosT2>
	inline SizeT operator()(const PosT1& mlast, const PosT2& last) const;
	
	inline SizeT operator()() const;

	template <SizeT I>
	inline decltype(auto) rootSteps(const IndexId<I>& id) const;

    private:
	SizeT mSize = 0;
	IndexId<L> mId;
	Xpr mXpr;
	UPos mStep;
	typedef decltype(mXpr.rootSteps(mId)) PosT;
	PosT mExt;
	
    };

    // unrolled loop:
    template <SizeT N, SizeT L, SizeT S, class Xpr>
    class SLFor : public XprInterface<SLFor<N,L,S,Xpr>>
    {
    public:
	DEFAULT_MEMBERS(SLFor);

	constexpr SLFor(const IndexId<L>& id, const Xpr& xpr);

	template <class PosT1, class PosT2>
	constexpr SizeT operator()(const PosT1& mlast, const PosT2& last) const;
	
	constexpr SizeT operator()() const;

	template <SizeT I>
	constexpr decltype(auto) rootSteps(const IndexId<I>& id) const;
	
    private:

	template <SizeT I, class PosT1, class PosT2>
	constexpr SizeT exec(const PosT1& mlast, const PosT2& last) const;

	template <SizeT I>
	constexpr SizeT exec() const;

	IndexId<L> mId;
	Xpr mXpr;
	typedef decltype(mXpr.RootSteps(mId)) PosT;
	PosT mExt;
	
    };


    // multi-threading
    template <SizeT L, class Xpr>
    class TFor : public XprInterface<TFor<L,Xpr>>
    {
    public:
	DEFAULT_MEMBERS(TFor);

	constexpr TFor(SizeT size, const IndexId<L>& id, SizeT step, const Xpr& xpr);

	template <class PosT1, class PosT2>
	inline SizeT operator()(const PosT1& mlast, const PosT2& last) const;
	
	inline SizeT operator()() const;

	template <SizeT I>
	inline decltype(auto) rootSteps(const IndexId<I>& id) const;
	
    private:
	SizeT mSize = 0;
	IndexId<L> mId = 0;
	Xpr mXpr;
	UPos mStep;
	typedef decltype(mXpr.rootSteps(mId)) PosT;
	PosT mExt;
	
    };

    
    
}

#endif
