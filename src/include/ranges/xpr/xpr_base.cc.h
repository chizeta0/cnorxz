
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
    DPos VXpr<Xpr>::vrootSteps(const IndexId<0>& id) const
    {
	return DPos(this->rootSteps(id));
    }
    
    /************
     *   DXpr   *
     ************/

    template <class Xpr>
    DXpr::DXpr(const Xpr& a) :
	ObjHandle<VXprBase>(std::make_unique<VXpr<Xpr>>(a))
    {}

    inline SizeT DXpr::operator()(const UPos& mlast, const DPos& last) const
    {
	return mC->vexec(mlast, last);
    }
    
    inline SizeT DXpr::operator()() const
    {
	return mC->vexec();
    }

    template <SizeT I>
    inline DPos DXpr::rootSteps(const IndexId<I>& id) const
    {
	return mC->vrootSteps(IndexId<0>(id.id()));
    }
}

#endif
