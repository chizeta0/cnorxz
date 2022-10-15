
#ifndef __cxz_xpr_base_h__
#define __cxz_xpr_base_h__

#include "base/base.h"
#include "pos_type.h"

namespace CNORXZ
{

    template <class Xpr>
    class XprInterface
    {
    public:
	DEFAULT_MEMBERS(XprInterface);

	inline Xpr& THIS() { return static_cast<Xpr&>(*this); }
	inline const Xpr& THIS() const { return static_cast<const Xpr&>(*this); }

	template <class PosT1, class PosT2>
	inline SizeT operator()(const PosT1& mlast, const PosT2& last) const { return THIS()(mlast,last); } 

	inline SizeT operator()() const { return THIS()(); }

	inline auto rootSteps(PtrId ptrId) const { return THIS().rootSteps(ptrId); }

	template <SizeT N>
	constexpr auto staticRootSteps(PtrId ptrId) const { return THIS().template staticRootSteps<N>(ptrId); }
    };

    class VXprBase
    {
    public:
	DEFAULT_MEMBERS(VXprBase);

	virtual Uptr<VXprBase> copy() const = 0;

	virtual SizeT vexec(const UPos& mlast, const DPos& last) const = 0;
	virtual SizeT vexec() const = 0;

	virtual DPos vrootSteps(PtrId ptrId) const = 0;
    };

    template <class Xpr>
    class VXpr : public VXprBase, public Xpr
    {
    public:
	DEFAULT_MEMBERS(VXpr);
	VXpr(const XprInterface<Xpr>& a);

	virtual Uptr<VXprBase> copy() const override final;

	virtual SizeT vexec(const UPos& mlast, const DPos& last) const override final;
	virtual SizeT vexec() const override final;

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
	inline SizeT operator()(const UPos& mlast, const PosT& last) const;
	inline SizeT operator()() const;

	inline DPos rootSteps(PtrId ptrId) const;

	template <SizeT N>
	inline DPos staticRootSteps(PtrId ptrId) const; // fallback to rootSteps
    };
}

#endif
