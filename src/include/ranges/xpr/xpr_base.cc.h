
#ifndef __cxz_xpr_base_cc_h__
#define __cxz_xpr_base_cc_h__

#include "xpr_base.h"

namespace CNORXZ
{
    
    /************
     *   VXpr   *
     ************/
    
    template <class Xpr>
    VXpr<Xpr>::VXpr(const XprInterface<Xpr>& a) :
	Xpr(a.THIS())
    {}

    template <class Xpr>
    Uptr<VXprBase> VXpr<Xpr>::copy() const
    {
	return std::make_unique<VXpr<Xpr>>(*this);
    }

    template <class Xpr>
    SizeT VXpr<Xpr>::vexec(const UPos& mlast, const DPos& last) const
    {
	return (*this)(mlast, last);
    }

    template <class Xpr>
    SizeT VXpr<Xpr>::vexec() const
    {
	return (*this)();
    }
    
    template <class Xpr>
    DPos VXpr<Xpr>::vrootSteps(PtrId ptrId) const
    {
	return DPos(this->rootSteps(ptrId));
    }
    
    /************
     *   DXpr   *
     ************/

    template <class Xpr>
    explicit DXpr::DXpr(const Xpr& a) :
	ObjHandle<VXprBase>(VXpr<Xpr>(a))
    {}

    template <class PosT>
    inline SizeT DXpr::operator()(const UPos& mlast, const PosT& last) const
    {
	DPosRef dlast(&last);
	return mC->vexec(mlast, dlast);
    }
    
    inline SizeT DXpr::operator()() const
    {
	return mC->vexec();
    }

    inline DPos DXpr::rootSteps(PtrId ptrId) const
    {
	return mC->rootSteps(ptrId);
    }

    template <SizeT N>
    inline DPos DXpr::staticRootSteps(PtrId ptrId) const
    {
	return this->rootSteps(ptrId);
    }
}

#endif
