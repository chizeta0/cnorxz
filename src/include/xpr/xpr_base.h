
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
	constexpr decltype(auto) operator()(const PosT& last) const { return THIS()(last); } 

	constexpr decltype(auto) operator()() const { return THIS()(); }

	template <SizeT I>
	constexpr decltype(auto) rootSteps(const IndexId<I>& id) const { return THIS().rootSteps(id); }
    };

    template <typename T>
    class VXprBase
    {
    public:
	DEFAULT_MEMBERS(VXprBase);

	virtual Uptr<VXprBase<T>> copy() const = 0;

	virtual T vexec(const DPos& last) const = 0;
	virtual T vexec() const = 0;

	virtual DPos vrootSteps(const IndexId<0>& id) const = 0;
    };

    template <typename T, class Xpr>
    class VXpr : public VXprBase<T>, public Xpr
    {
    public:
	DEFAULT_MEMBERS(VXpr);
	VXpr(const XprInterface<Xpr>& a);

	virtual Uptr<VXprBase<T>> copy() const override final;

	virtual T vexec(const DPos& last) const override final;
	virtual T vexec() const override final;

	virtual DPos vrootSteps(const IndexId<0>& id) const override final;
    };

    template <typename T>
    class DXpr : public ObjHandle<VXprBase<T>>,
		 public XprInterface<DXpr<T>>
    {
    public:
	typedef ObjHandle<VXprBase<T>> VB;
	
	DEFAULT_MEMBERS(DXpr);

	template <class Xpr>
	explicit DXpr(const Xpr& a);

	inline T operator()(const DPos& last) const;
	inline T operator()() const;

	template <SizeT I>
	inline DPos rootSteps(const IndexId<I>& id) const;
    };
}

#endif
