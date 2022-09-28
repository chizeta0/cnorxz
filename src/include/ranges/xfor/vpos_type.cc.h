
#ifndef __cxz_vpos_type_cc_h__
#define __cxz_vpos_type_cc_h__

#include "vpos_type.h"

namespace CNORXZ
{
    /****************
     *   VPosBase   *
     ****************/

    template <SizeT N>
    Uptr<VPosBase> VPosBase::vextend(const SPos<N>& a) const
    {
	return this->vextend(UPos(N));
    }

    /************
     *   VPos   *
     ************/

    template <class PosT>
    VPos<PosT>::VPos(const CPosInterface<PosT>& a) :
	PosT(a.THIS())
    {}
	
    template <class PosT>
    Uptr<VPosBase> VPos<PosT>::copy() const
    {
	return std::make_unique<VPos<PosT>>(*this);
    }

    template <class PosT>
    SizeT VPos<PosT>::vsize() const
    {
	return PosT::THIS().size();
    }

    template <class PosT>
    SizeT VPos<PosT>::vval() const
    {
	return PosT::THIS().val();
    }

    template <class PosT>
    const VPosBase* VPos<PosT>::vget() const
    {
	return this;
    }

    template <class PosT>
    const VPosBase* VPos<PosT>::vnext() const
    {
	return nullptr;
    }

    template <class PosT>
    Uptr<VPosBase> VPos<PosT>::vplus(const VPosBase* a) const
    {
	return std::make_unique<VPos<PosT>>((*this) + DPosRef(a));
    }
    
    template <class PosT>
    Uptr<VPosBase> VPos<PosT>::vtimes(const VPosBase* a) const
    {
	return std::make_unique<VPos<PosT>>((*this) * UPos(a->vval()));
    }
    
    template <class PosT>
    Uptr<VPosBase> VPos<PosT>::vextend(const DPos& a) const
    {
	return std::make_unique<MPos<PosT,DPos>>(*this,a);
    }

    template <class PosT>
    Uptr<VPosBase> VPos<PosT>::vextend(const DPosRef& a) const
    {
	return std::make_unique<MPos<PosT,DPosRef>>(*this,a);
    }

    template <class PosT>
    Uptr<VPosBase> VPos<PosT>::vextend(const UPos& a) const
    {
	return std::make_unique<MPos<PosT,UPos>>(*this,a);
    } 

    template <class PosT>
    Uptr<VPosBase> VPos<PosT>::vextend(const FPos& a) const
    {
	return std::make_unique<MPos<PosT,FPos>>(*this,a);
    }
    // .... probably I need to define a static instanciation limit...

    /******************
     *   VPos<MPos>   *
     ******************/

    template <class PosT1, class PosT2>
    VPos<MPos<PosT1,PosT2>>::VPos(const CPosInterface<MPos<PosT1,PosT2>>& a) :
	MPos<PosT1,PosT2>(a.THIS()),
	mFRef(&this->first()), mNRef(&this->next())
    {}
	
    template <class PosT1, class PosT2>
    Uptr<VPosBase> VPos<MPos<PosT1,PosT2>>::copy() const 
    {
	return std::make_unique<VPos<MPos<PosT1,PosT2>>>(*this);
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
    const VPosBase* VPos<MPos<PosT1,PosT2>>::vget() const 
    {
	return &mFRef;
    }
    
    template <class PosT1, class PosT2>
    const VPosBase* VPos<MPos<PosT1,PosT2>>::vnext() const 
    {
	return &mNRef;
    }
    
    template <class PosT1, class PosT2>
    Uptr<VPosBase> VPos<MPos<PosT1,PosT2>>::vplus(const VPosBase* a) const
    {
	return std::make_unique<MPos<PosT1,PosT2>>((*this) + DPosRef(a));
    }
    
    template <class PosT1, class PosT2>
    Uptr<VPosBase> VPos<MPos<PosT1,PosT2>>::vtimes(const VPosBase* a) const
    {
	return std::make_unique<MPos<PosT1,PosT2>>((*this) * UPos(a->vval()));
    }
    
    template <class PosT1, class PosT2>
    Uptr<VPosBase> VPos<MPos<PosT1,PosT2>>::vextend(const DPos& a) const 
    {
	return std::make_unique<MPos<MPosT,DPos>>(*this,a);
    }

    template <class PosT1, class PosT2>
    Uptr<VPosBase> VPos<MPos<PosT1,PosT2>>::vextend(const DPosRef& a) const 
    {
	return std::make_unique<MPos<MPosT,DPosRef>>(*this,a);
    }

    template <class PosT1, class PosT2>
    Uptr<VPosBase> VPos<MPos<PosT1,PosT2>>::vextend(const UPos& a) const 
    {
	return std::make_unique<MPos<MPosT,UPos>>(*this,a);
    }
    
    template <class PosT1, class PosT2>
    Uptr<VPosBase> VPos<MPos<PosT1,PosT2>>::vextend(const FPos& a) const 
    {
	return std::make_unique<MPos<MPosT,FPos>>(*this,a);
    }
    
    /***************
     *   VPosRef   *
     ***************/

    template <class PosT>
    VPosRef<PosT>::VPosRef(const CPosInterface<PosT>* c) :
	mC(&c->THIS())
    {}

    template <class PosT>
    Uptr<VPosBase> VPosRef<PosT>::copy() const 
    {
	return std::make_unique<VPos<PosT>>(*mC);
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
    const VPosBase* VPosRef<PosT>::vget() const 
    {
	return this;
    }

    template <class PosT>
    const VPosBase* VPosRef<PosT>::vnext() const 
    {
	return nullptr;
    }
    
    template <class PosT>
    Uptr<VPosBase> VPosRef<PosT>::vplus(const VPosBase* a) const
    {
	return std::make_unique<VPos<PosT>>((*mC) + DPosRef(a));
    }
    
    template <class PosT>
    Uptr<VPosBase> VPosRef<PosT>::vtimes(const VPosBase* a) const
    {
	return std::make_unique<VPos<PosT>>((*mC) * UPos(a->vval()));
    }
    
    template <class PosT>
    Uptr<VPosBase> VPosRef<PosT>::vextend(const DPos& a) const 
    {
	return std::make_unique<MPos<PosT,DPos>>(*mC,a);
    }

    template <class PosT>
    Uptr<VPosBase> VPosRef<PosT>::vextend(const DPosRef& a) const 
    {
	return std::make_unique<MPos<PosT,DPosRef>>(*mC,a);
    }

    template <class PosT>
    Uptr<VPosBase> VPosRef<PosT>::vextend(const UPos& a) const 
    {
	return std::make_unique<MPos<PosT,UPos>>(*mC,a);
    }
    
    template <class PosT>
    Uptr<VPosBase> VPosRef<PosT>::vextend(const FPos& a) const 
    {
	return std::make_unique<MPos<PosT,FPos>>(*mC,a);
    }

    /*********************
     *   VPosRef<MPos>   *
     *********************/

    template <class PosT1, class PosT2>
    VPosRef<MPos<PosT1,PosT2>>::VPosRef(const CPosInterface<MPos<PosT1,PosT2>>* c) :
	mC(c), mFRef(&c->get()), mNRef(&c->next())
    {}

    template <class PosT1, class PosT2>
    Uptr<VPosBase> VPosRef<MPos<PosT1,PosT2>>::copy() const
    {
	return std::make_unique<VPos<MPos<PosT1,PosT2>>>(*mFRef,*mNRef);
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
    const VPosBase* VPosRef<MPos<PosT1,PosT2>>::vget() const
    {
	return mFRef;
    }

    template <class PosT1, class PosT2>
    const VPosBase* VPosRef<MPos<PosT1,PosT2>>::vnext() const
    {
	return mNRef;
    }

    template <class PosT1, class PosT2>
    Uptr<VPosBase> VPosRef<MPos<PosT1,PosT2>>::vplus(const VPosBase* a) const
    {
	return std::make_unique<MPos<PosT1,PosT2>>((*mC) + DPosRef(a));
    }
    
    template <class PosT1, class PosT2>
    Uptr<VPosBase> VPosRef<MPos<PosT1,PosT2>>::vtimes(const VPosBase* a) const
    {
	return std::make_unique<MPos<PosT1,PosT2>>((*mC) * UPos(a->vval()));
    }
    
    template <class PosT1, class PosT2>
    Uptr<VPosBase> VPosRef<MPos<PosT1,PosT2>>::vextend(const DPos& a) const
    {
	return std::make_unique<MPos<MPosT,DPos>>(*mC,a);
    }

    template <class PosT1, class PosT2>
    Uptr<VPosBase> VPosRef<MPos<PosT1,PosT2>>::vextend(const DPosRef& a) const
    {
	return std::make_unique<MPos<MPosT,DPosRef>>(*mC,a);
    }

    template <class PosT1, class PosT2>
    Uptr<VPosBase> VPosRef<MPos<PosT1,PosT2>>::vextend(const UPos& a) const
    {
	return std::make_unique<MPos<MPosT,UPos>>(*mC,a);
    }

    template <class PosT1, class PosT2>
    Uptr<VPosBase> VPosRef<MPos<PosT1,PosT2>>::vextend(const FPos& a) const
    {
	return std::make_unique<MPos<MPosT,FPos>>(*mC,a);
    }
}

#endif
