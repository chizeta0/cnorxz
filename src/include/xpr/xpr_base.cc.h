// -*- C++ -*-
/**
   
   @file include/xpr/xpr_base.cc.h
   @brief Xpression base template implementations.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_xpr_base_cc_h__
#define __cxz_xpr_base_cc_h__

#include "xpr_base.h"

namespace CNORXZ
{
    // TODO: move to a better place!!!
    template <class PosT>
    struct PosFromDPos
    {
	static constexpr decltype(auto) mk(const DPos& pos)
	{
	    if constexpr(std::is_same<PosT,DPos>::value or std::is_same<PosT,DPosRef>::value){
		return pos;
	    }
	    else {
		return UPos(pos.val());
	    }
	}
    };

    template <class BPosT, class NPosT>
    struct PosFromDPos<MPos<BPosT,NPosT>>
    {
	static constexpr decltype(auto) mk(const DPos& pos)
	{
	    return mkMPos( PosFromDPos<BPosT>::mk(pos), PosFromDPos<NPosT>::mk( pos.sub() ) );
	}
    };
    
    template <class PosT>
    constexpr decltype(auto) mkMPosFromDPos(const DPos& pos)
    {
	return PosFromDPos<PosT>::mk(pos);
    }
    
    /*==========+
     |   VXpr   |
     +==========*/
    
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
	typedef typename std::remove_reference<decltype(this->rootSteps(IndexId<0>{}))>::type Ext;
	typedef decltype((*this)()) RetT;
	if constexpr(std::is_same<RetT,void>::value){
	    (*this)(mkMPosFromDPos<Ext>(last));
	    return None {};
	}
	else {
	    return (*this)(mkMPosFromDPos<Ext>(last));
	}
    }

    template <typename T, class Xpr>
    T VXpr<T,Xpr>::vexec() const
    {
	typedef decltype((*this)()) RetT;
	if constexpr(std::is_same<RetT,void>::value){
	    (*this)();
	    return None {};
	}
	else {
	    return (*this)();
	}
    }
    
    template <typename T, class Xpr>
    DPos VXpr<T,Xpr>::vrootSteps(const IndexId<0>& id) const
    {
	return DPos(this->rootSteps(id));
    }
    
    /*==========+
     |   DXpr   |
     +==========*/

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
