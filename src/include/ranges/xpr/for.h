
#ifndef __cxz_for_h__
#define __cxz_for_h__

#include "base/base.h"
#include "xpr_base.h"

namespace CNORXZ
{
    /*
    template <class ForT, class Xpr>
    class ForInterface : public XprInterface<ForInterface<ForT,Xpr>>
    {
    public:
	DEFAULT_MEMBERS(ForInterface);

	inline auto rootSteps(PtrId ptrId) const;

	template <SizeT L>
	constexpr auto staticRootSteps(PtrId ptrId) const;
    };
    */
    template <class Xpr>
    class For : public XprInterface<For<Xpr>>
    {
    public:
	DEFAULT_MEMBERS(For);

	constexpr For(SizeT size, PtrId iptrId, SizeT step, const Xpr& xpr);

	template <class PosT1, class PosT2>
	inline SizeT operator()(const PosT1& mlast, const PosT2& last) const;
	
	inline SizeT operator()() const;

	inline auto rootSteps(PtrId ptrId) const;

	template <SizeT L>
	constexpr auto staticRootSteps(PtrId ptrId) const;
	
    private:
	SizeT mSize = 0;
	PtrId mIptrId = 0;
	Xpr mXpr;
	UPos mStep;
	typedef decltype(mXpr.rootSteps(0)) PosT;
	PosT mExt;
	
    };

    // unrolled loop:
    template <SizeT N, SizeT L, SizeT S, class Xpr>
    class SLFor : public XprInterface<SLFor<N,L,S,Xpr>>
    {
    public:
	DEFAULT_MEMBERS(SLFor);

	constexpr SLFor(PtrId iptrId, const Xpr& xpr);

	template <class PosT1, class PosT2>
	constexpr SizeT operator()(const PosT1& mlast, const PosT2& last) const;
	
	constexpr SizeT operator()() const;

	constexpr auto rootSteps(PtrId ptrId) const;

	template <SizeT L2>
	constexpr auto staticRootSteps(PtrId ptrId) const;
	
    private:

	template <SizeT I, class PosT1, class PosT2>
	constexpr SizeT exec(const PosT1& mlast, const PosT2& last) const;

	template <SizeT I>
	constexpr SizeT exec() const;
	
	PtrId mIptrId = 0;
	Xpr mXpr;
	typedef decltype(mXpr.template staticRootSteps<L>(0)) PosT;
	PosT mExt;
	
    };


    // multi-threading
    template <class Xpr>
    class TFor : public XprInterface<TFor<Xpr>>
    {
    public:
	DEFAULT_MEMBERS(TFor);

	constexpr TFor(SizeT size, PtrId iptrId, SizeT step, const Xpr& xpr);

	template <class PosT1, class PosT2>
	inline SizeT operator()(const PosT1& mlast, const PosT2& last) const;
	
	inline SizeT operator()() const;

	inline auto rootSteps(PtrId ptrId) const;

	template <SizeT L2>
	inline auto staticRootSteps(PtrId ptrId) const;
	
    private:
	SizeT mSize = 0;
	PtrId mIptrId = 0;
	Xpr mXpr;
	UPos mStep;
	typedef decltype(mXpr.rootSteps(0)) PosT;
	PosT mExt;
	
    };

    
    
}

#endif
