
#ifndef __cxz_xpr_base_h__
#define __cxz_xpr_base_h__

#include "base/base.h"

namespace CNORXZ
{

    template <class Xpr>
    class XprInterface
    {
    public:
	DEFAULT_MEMBERS(XprInterface);

	Xpr& THIS() { return static_cast<Xpr&>(*this); }
	const Xpr& THIS() const { return static_cast<const Xpr&>(*this); }

	template <class PosT>
	SizeT operator()(SizeT mlast, const PosT& last) const { return THIS()(mlast,last); } 

	SizeT operator()(SizeT mlast = 0) const { return THIS()(mlast); }

	auto rootSteps(PtrId ptrId) const { return THIS().rootSteps(ptrId); }

	template <SizeT N>
	constexpr auto staticRootSteps(PtrId ptrId) const { return THIS().staticRootSteps<N>(ptrId); }
    };

    class VXprBase
    {
    public:
	DEFAULT_MEMBERS(VXprBase);

	virtual Uptr<VXprBase> copy() const = 0;

	virtual SizeT vexec(SizeT mlast, const DPos& last) const = 0;
	virtual SizeT vexec(SizeT mlast) const = 0;

	virtual DPos vrootSteps(PtrId ptrId) const = 0;
    };

    template <class Xpr>
    class VXpr : public VXprBase, public Xpr
    {
    public:
	DEFAULT_MEMBERS(VXpr);
	VXpr(const XprInterface<Xpr>& a);

	virtual Uptr<VXprBase> copy() const override final;

	virtual SizeT vexec(SizeT mlast, const DPos& last) const override final;
	virtual SizeT vexec(SizeT mlast) const override final;

	virtual DPos vrootSteps(PtrId ptrId) const override final;
    };

    class DXpr : public ObjHandle<VXprBase>,
		 public XprInterface<DXpr>
    {
    public:
	DEFAULT_MEMBERS(DXpr);

	template <class Xpr>
	explicit DXpr(const Xpr& a);

	template <class PosT>
	inline SizeT operator()(SizeT mlast, const PosT& last) const;
	inline SizeT operator()(SizeT mlast) const;

	inline DPos rootSteps(PtrId ptrId) const;

	template <SizeT N>
	inline DPos staticRootSteps(PtrId ptrId) const; // fallback to rootSteps
    };
}

#endif
