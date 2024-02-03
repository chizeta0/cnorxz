// -*- C++ -*-
/**
   
   @file include/xpr/vpos_type.cc.h
   @brief Dynamic position types template implementations.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_vpos_type_cc_h__
#define __cxz_vpos_type_cc_h__

#include "vpos_type.h"
#include "pos_type.h"

namespace CNORXZ
{
    /*==========+
     |   VPos   |
     +==========*/

    template <class PosT>
    VPos<PosT>::VPos(const PosT& a) :
	PosT(a)
    {}
	
    template <class PosT>
    Uptr<VPosBase> VPos<PosT>::copy() const
    {
	return std::make_unique<VPos<PosT>>(*this);
    }

    template <class PosT>
    const VPosBase* VPos<PosT>::vget() const
    {
	return this;
    }

    template <class PosT>
    SizeT VPos<PosT>::vsize() const
    {
	return this->size();
    }

    template <class PosT>
    SizeT VPos<PosT>::vval() const
    {
	return this->val();
    }

    template <class PosT>
    const VPosBase* VPos<PosT>::vnext() const
    {
	return nullptr;
    }

    template <class PosT>
    Uptr<VPosBase> VPos<PosT>::vplus(const VPosBase* a) const
    {
	typedef decltype((*this) + UPos(a->vval())) OPosT;
	return std::make_unique<VPos<OPosT>>((*this) + UPos(a->vval()));
    }
    
    template <class PosT>
    Uptr<VPosBase> VPos<PosT>::vtimes(const VPosBase* a) const
    {
	typedef decltype((*this) * UPos(a->vval())) OPosT;
	return std::make_unique<VPos<OPosT>>((*this) * UPos(a->vval()));
    }

    template <class PosT>
    Uptr<VPosBase> VPos<PosT>::vexec(const VPosBase* a) const
    {
	typedef decltype((*this)(UPos(a->vval()))) OPosT;
	return std::make_unique<VPos<OPosT>>( (*this)(UPos(a->vval())) );
    }

    /*================+
     |   VPos<MPos>   |
     +================*/

    template <class PosT1, class PosT2>
    VPos<MPos<PosT1,PosT2>>::VPos(const VPos& a) :
	MPos<PosT1,PosT2>(a),
	mTRef(static_cast<const PosT1*>(this))
    {
	if constexpr(IS_NOT_SAME(PosT2,DPos) and IS_NOT_SAME(PosT2,DPosRef)){
	    mNRef = VPosRef<PosT2>(&this->next());
	}
    }

    template <class PosT1, class PosT2>
    VPos<MPos<PosT1,PosT2>>& VPos<MPos<PosT1,PosT2>>::operator=(const VPos& a)
    {
	MPos<PosT1,PosT2>::operator=(a);
	mTRef = VPosRef<PosT1>(static_cast<const PosT1*>(this));
	if constexpr(IS_NOT_SAME(PosT2,DPos) and IS_NOT_SAME(PosT2,DPosRef)){
	    mNRef = VPosRef<PosT2>(&this->next());
	}
	return *this;
    }

    template <class PosT1, class PosT2>
    VPos<MPos<PosT1,PosT2>>::VPos(const MPos<PosT1,PosT2>& a) :
	MPos<PosT1,PosT2>(a),
	mTRef(static_cast<const PosT1*>(this))
    {
	if constexpr(IS_NOT_SAME(PosT2,DPos) and IS_NOT_SAME(PosT2,DPosRef)){
	    mNRef = VPosRef<PosT2>(&this->next());
	}
    }
	
    template <class PosT1, class PosT2>
    Uptr<VPosBase> VPos<MPos<PosT1,PosT2>>::copy() const 
    {
	return std::make_unique<VPos<MPos<PosT1,PosT2>>>(*this);
    }
    
    template <class PosT1, class PosT2>
    const VPosBase* VPos<MPos<PosT1,PosT2>>::vget() const
    {
	return &mTRef;
    }

    template <class PosT1, class PosT2>
    SizeT VPos<MPos<PosT1,PosT2>>::vsize() const 
    {
	return this->size();
    }
    
    template <class PosT1, class PosT2>
    SizeT VPos<MPos<PosT1,PosT2>>::vval() const 
    {
	return this->val();
    }
    
    template <class PosT1, class PosT2>
    const VPosBase* VPos<MPos<PosT1,PosT2>>::vnext() const 
    {
	if constexpr(IS_NOT_SAME(PosT2,DPos) and IS_NOT_SAME(PosT2,DPosRef)){
	    return &mNRef;
	}
	else {
	    return this->next().get();
	}
    }
    
    template <class PosT1, class PosT2>
    Uptr<VPosBase> VPos<MPos<PosT1,PosT2>>::vplus(const VPosBase* a) const
    {
	typedef decltype((*this) + PosFromVPos<MPos<PosT1,PosT2>>::make(a)) OPosT;
	return std::make_unique<VPos<OPosT>>((*this) + PosFromVPos<MPos<PosT1,PosT2>>::make(a));
    }
    
    template <class PosT1, class PosT2>
    Uptr<VPosBase> VPos<MPos<PosT1,PosT2>>::vtimes(const VPosBase* a) const
    {
	typedef decltype((*this) * UPos(a->vval())) OPosT;
	return std::make_unique<VPos<OPosT>>((*this) * UPos(a->vval()));
    }

    template <class PosT1, class PosT2>
    Uptr<VPosBase> VPos<MPos<PosT1,PosT2>>::vexec(const VPosBase* a) const
    {
	typedef decltype( (*this)(UPos(a->vval())) ) OPosT;
	return std::make_unique<VPos<OPosT>>( (*this)(UPos(a->vval())) );
    }

    /*=============+
     |   VPosRef   |
     +=============*/

    template <class PosT>
    VPosRef<PosT>::VPosRef(const PosT* c) :
	mC(c)
    {}

    template <class PosT>
    Uptr<VPosBase> VPosRef<PosT>::copy() const 
    {
	return std::make_unique<VPos<PosT>>(*mC);
    }
    
    template <class PosT>
    const VPosBase* VPosRef<PosT>::vget() const
    {
	return this;
    }

    template <class PosT>
    SizeT VPosRef<PosT>::vsize() const 
    {
	return mC->size();
    }
    
    template <class PosT>
    SizeT VPosRef<PosT>::vval() const 
    {
	return mC->val();
    }
    
    template <class PosT>
    const VPosBase* VPosRef<PosT>::vnext() const 
    {
	return nullptr;
    }
    
    template <class PosT>
    Uptr<VPosBase> VPosRef<PosT>::vplus(const VPosBase* a) const
    {
	typedef decltype((*mC) + UPos(a->vval())) OPosT;
	return std::make_unique<VPos<OPosT>>((*mC) + UPos(a->vval()));
    }
    
    template <class PosT>
    Uptr<VPosBase> VPosRef<PosT>::vtimes(const VPosBase* a) const
    {
	typedef decltype((*mC) * UPos(a->vval())) OPosT;
	return std::make_unique<VPos<OPosT>>((*mC) * UPos(a->vval()));
    }

    template <class PosT>
    Uptr<VPosBase> VPosRef<PosT>::vexec(const VPosBase* a) const
    {
	typedef decltype( (*mC)(UPos(a->vval())) ) OPosT;
	return std::make_unique<VPos<OPosT>>( (*mC)(UPos(a->vval())) );
    }

    /*===================+
     |   VPosRef<MPos>   |
     +===================*/

    template <class PosT1, class PosT2>
    VPosRef<MPos<PosT1,PosT2>>::VPosRef(const MPos<PosT1,PosT2>* c) :
	mC(c),
	mTRef(static_cast<const PosT1*>(mC))
    {
	if constexpr(IS_NOT_SAME(PosT2,DPos) and IS_NOT_SAME(PosT2,DPosRef)){
	    mNRef = VPosRef<PosT2>(&c->next());
	}
    }

    template <class PosT1, class PosT2>
    Uptr<VPosBase> VPosRef<MPos<PosT1,PosT2>>::copy() const
    {
	return std::make_unique<VPos<MPos<PosT1,PosT2>>>(*mC);
    }

    template <class PosT1, class PosT2>
    const VPosBase* VPosRef<MPos<PosT1,PosT2>>::vget() const
    {
	return &mTRef;
    }

    template <class PosT1, class PosT2>
    SizeT VPosRef<MPos<PosT1,PosT2>>::vsize() const
    {
	return mC->size();
    }

    template <class PosT1, class PosT2>
    SizeT VPosRef<MPos<PosT1,PosT2>>::vval() const
    {
	return mC->val();
    }

    template <class PosT1, class PosT2>
    const VPosBase* VPosRef<MPos<PosT1,PosT2>>::vnext() const
    {
	if constexpr(IS_NOT_SAME(PosT2,DPos) and IS_NOT_SAME(PosT2,DPosRef)){
	    return &mNRef;
	}
	else {
	    return mC->next().get();
	}
    }

    template <class PosT1, class PosT2>
    Uptr<VPosBase> VPosRef<MPos<PosT1,PosT2>>::vplus(const VPosBase* a) const
    {
	typedef decltype((*mC) + PosFromVPos<MPos<PosT1,PosT2>>::make(a)) OPosT;
	return std::make_unique<VPos<OPosT>>((*mC) + PosFromVPos<MPos<PosT1,PosT2>>::make(a));
    }
    
    template <class PosT1, class PosT2>
    Uptr<VPosBase> VPosRef<MPos<PosT1,PosT2>>::vtimes(const VPosBase* a) const
    {
	typedef decltype((*mC) * UPos(a->vval())) OPosT;
	return std::make_unique<VPos<OPosT>>((*mC) * UPos(a->vval()));
    }

    template <class PosT1, class PosT2>
    Uptr<VPosBase> VPosRef<MPos<PosT1,PosT2>>::vexec(const VPosBase* a) const
    {
	typedef decltype( (*mC)(UPos(a->vval())) ) OPosT;
	return std::make_unique<VPos<OPosT>>( (*mC)(UPos(a->vval())) );
    }
    
    /*=================+
     |   PosFromVPos   |
     +=================*/

    template <class PosT>
    inline decltype(auto) PosFromVPos<PosT>::make(const VPosBase* a)
    {
	return PosT(a->vval());
    }

    template <class PosT1, class PosT2>
    inline decltype(auto) PosFromVPos<MPos<PosT1,PosT2>>::make(const VPosBase* a)
    {
#if CXZ_DEBUG
	CXZ_ASSERT(a->vnext() != nullptr, "VPos does not match required size");
#endif
	return MPos<PosT1,PosT2>(a->vval(), PosFromVPos<PosT2>::make(a->vnext()));
    }
}

#endif
