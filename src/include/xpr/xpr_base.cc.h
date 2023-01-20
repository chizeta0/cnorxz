
#ifndef __cxz_xpr_base_cc_h__
#define __cxz_xpr_base_cc_h__

#include "xpr_base.h"

namespace CNORXZ
{
    
    /************
     *   VXpr   *
     ************/
    
    template <typename T, class Xpr>
    VXpr<T,Xpr>::VXpr(const XprInterface<Xpr>& a) :
	Xpr(a.THIS())
    {}

    template <typename T, class Xpr>
    Uptr<VXprBase<T>> VXpr<T,Xpr>::copy() const
    {
	return std::make_unique<VXpr<T,Xpr>>(*this);
    }

    template <typename T, class Xpr>
    T VXpr<T,Xpr>::vexec(const DPos& last) const
    {
	CXZ_ERROR("IMPLEMENT!!!" << last.val());
	//(*this)(last);
	return T();
    }

    template <typename T, class Xpr>
    T VXpr<T,Xpr>::vexec() const
    {
	CXZ_ERROR("IMPLEMENT!!!");
	//(*this)();
	return T();
    }
    
    template <typename T, class Xpr>
    DPos VXpr<T,Xpr>::vrootSteps(const IndexId<0>& id) const
    {
	return DPos(this->rootSteps(id));
    }
    
    /************
     *   DXpr   *
     ************/

    template <typename T>
    template <class Xpr>
    DXpr<T>::DXpr(const Xpr& a) :
	ObjHandle<VXprBase<T>>(std::make_unique<VXpr<T,Xpr>>(a))
    {}

    template <typename T>
    inline T DXpr<T>::operator()(const DPos& last) const
    {
	return VB::mC->vexec(last);
    }
    
    template <typename T>
    inline T DXpr<T>::operator()() const
    {
	return VB::mC->vexec();
    }

    template <typename T>
    template <SizeT I>
    inline DPos DXpr<T>::rootSteps(const IndexId<I>& id) const
    {
	return VB::mC->vrootSteps(IndexId<0>(id.id()));
    }
}

#endif
