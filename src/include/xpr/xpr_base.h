
#ifndef __cxz_xpr_base_h__
#define __cxz_xpr_base_h__

#include "base/base.h"
#include "pos_type.h"
#include "index_id.h"

namespace CNORXZ
{

    template <class Xpr>
    class XprInterface
    {
    public:
	DEFAULT_MEMBERS(XprInterface);

	inline Xpr& THIS() { return static_cast<Xpr&>(*this); }
	inline const Xpr& THIS() const { return static_cast<const Xpr&>(*this); }

	template <class PosT>
	inline SizeT operator()(const PosT& last) const { return THIS()(last); } 

	inline SizeT operator()() const { return THIS()(); }

	template <SizeT I>
	constexpr decltype(auto) rootSteps(const IndexId<I>& id) const { return THIS().rootSteps(id); }
    };

    class VXprBase
    {
    public:
	DEFAULT_MEMBERS(VXprBase);

	virtual Uptr<VXprBase> copy() const = 0;

	virtual SizeT vexec(const DPos& last) const = 0;
	virtual SizeT vexec() const = 0;

	virtual DPos vrootSteps(const IndexId<0>& id) const = 0;
    };

    template <class Xpr>
    class VXpr : public VXprBase, public Xpr
    {
    public:
	DEFAULT_MEMBERS(VXpr);
	VXpr(const XprInterface<Xpr>& a);

	virtual Uptr<VXprBase> copy() const override final;

	virtual SizeT vexec(const DPos& last) const override final;
	virtual SizeT vexec() const override final;

	virtual DPos vrootSteps(const IndexId<0>& id) const override final;
    };

    class DXpr : public ObjHandle<VXprBase>,
		 public XprInterface<DXpr>
    {
    public:
	DEFAULT_MEMBERS(DXpr);

	template <class Xpr>
	explicit DXpr(const Xpr& a);

	inline SizeT operator()(const DPos& last) const;
	inline SizeT operator()() const;

	template <SizeT I>
	inline DPos rootSteps(const IndexId<I>& id) const;
    };
}

#endif
