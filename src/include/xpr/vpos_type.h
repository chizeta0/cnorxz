
#ifndef __cxz_vpos_type_h__
#define __cxz_vpos_type_h__

#include "base/base.h"

namespace CNORXZ
{
    class VPosBase
    {
    public:
	DEFAULT_MEMBERS(VPosBase);

	virtual Uptr<VPosBase> copy() const = 0;
	virtual SizeT vsize() const = 0;
	virtual SizeT vval() const = 0;
	virtual const VPosBase* vget() const = 0; // strip away all MPos<...>
	virtual const VPosBase* vnext() const = 0;
	virtual Uptr<VPosBase> vplus(const VPosBase* a) const = 0;
	virtual Uptr<VPosBase> vtimes(const VPosBase* a) const = 0;
	virtual Uptr<VPosBase> vexec(const VPosBase* a) const = 0;
    };

    template <class PosT>
    class VPos : public VPosBase, public PosT
    {
    public:
	
	DEFAULT_MEMBERS(VPos);
	VPos(const PosT& a);
	
	virtual Uptr<VPosBase> copy() const override final;
	virtual const VPosBase* vget() const override final;
	virtual SizeT vsize() const override final;
	virtual SizeT vval() const override final;
	virtual const VPosBase* vnext() const override final;
	virtual Uptr<VPosBase> vplus(const VPosBase* a) const override final;
	virtual Uptr<VPosBase> vtimes(const VPosBase* a) const override final;
	virtual Uptr<VPosBase> vexec(const VPosBase* a) const override final;
    };

    template <class PosT1, class PosT2>
    class VPos<MPos<PosT1,PosT2>> : public VPosBase, public MPos<PosT1,PosT2>
    {
    private:
	VPosRef<PosT1> mTRef;
	VPosRef<PosT2> mNRef;

    public:
	DEFAULT_C(VPos);
	DEFAULT_MOVE(VPos);
	VPos(const VPos& a);
	VPos& operator=(const VPos& a);
	VPos(const MPos<PosT1,PosT2>& a);
	
	virtual Uptr<VPosBase> copy() const override final;
	virtual const VPosBase* vget() const override final;
	virtual SizeT vsize() const override final;
	virtual SizeT vval() const override final;
	virtual const VPosBase* vnext() const override final;
	virtual Uptr<VPosBase> vplus(const VPosBase* a) const override final;
	virtual Uptr<VPosBase> vtimes(const VPosBase* a) const override final;
	virtual Uptr<VPosBase> vexec(const VPosBase* a) const override final;
    };

    template <class PosT>
    class VPosRef : public VPosBase
    {
    private:
	const PosT* mC = nullptr;

    public:
	DEFAULT_MEMBERS(VPosRef);
	VPosRef(const PosT* c);

	virtual Uptr<VPosBase> copy() const override final;
	virtual const VPosBase* vget() const override final;
	virtual SizeT vsize() const override final;
	virtual SizeT vval() const override final;
	virtual const VPosBase* vnext() const override final;
	virtual Uptr<VPosBase> vplus(const VPosBase* a) const override final;
	virtual Uptr<VPosBase> vtimes(const VPosBase* a) const override final;
	virtual Uptr<VPosBase> vexec(const VPosBase* a) const override final;

    };


    template <class PosT1, class PosT2>
    class VPosRef<MPos<PosT1,PosT2>> : public VPosBase
    {
    private:
	const MPos<PosT1,PosT2>* mC = nullptr;
	VPosRef<PosT1> mTRef;
	VPosRef<PosT2> mNRef;

    public:
	DEFAULT_MEMBERS(VPosRef);
	VPosRef(const MPos<PosT1,PosT2>* c);

	virtual Uptr<VPosBase> copy() const override final;
	virtual const VPosBase* vget() const override final;
	virtual SizeT vsize() const override final;
	virtual SizeT vval() const override final;
	virtual const VPosBase* vnext() const override final;
	virtual Uptr<VPosBase> vplus(const VPosBase* a) const override final;
	virtual Uptr<VPosBase> vtimes(const VPosBase* a) const override final;
	virtual Uptr<VPosBase> vexec(const VPosBase* a) const override final;
    };

    
    // defined as empty since they should never instanciated
    template <SizeT N>
    class VPos<SPos<N>>
    {};

    // defined as empty since they should never instanciated
    template <SizeT N>
    class VPosRef<SPos<N>>
    {};

    // defined as empty since they should never instanciated
    template <SizeT N, SizeT... Ms>
    class VPos<SFPos<N,Ms...>>
    {};

    // defined as empty since they should never instanciated
    template <SizeT N, SizeT... Ms>
    class VPosRef<SFPos<N,Ms...>>
    {};

    // defined as empty since they should never instanciated
    template <>
    class VPos<DPos>
    {};

    // defined as empty since they should never instanciated
    template <>
    class VPosRef<DPos>
    {};

    // defined as empty since they should never instanciated
    template <>
    class VPos<DPosRef>
    {};

    // defined as empty since they should never instanciated
    template <>
    class VPosRef<DPosRef>
    {};

    template <class PosT>
    struct PosFromVPos
    {
	static inline decltype(auto) make(const VPosBase* a);
    };

    template <class PosT1, class PosT2>
    struct PosFromVPos<MPos<PosT1,PosT2>>
    {
	static inline decltype(auto) make(const VPosBase* a);
    };

}

#endif
