
#ifndef __cxz_vpos_type_cc_h__
#define __cxz_vpos_type_cc_h__

#include "vpos_type.h"
#include "pos_type.h"

namespace CNORXZ
{
    /****************
     *   VPosBase   *
     ****************/

    template <SizeT N>
    inline Uptr<VPosBase> VPosBase::vextend(const SPos<N>& a) const
    {
	return this->vextend(UPos(N));
    }

    template <SizeT N, SizeT... Ms>
    inline Uptr<VPosBase> VPosBase::vextend(const SFPos<N,Ms...>& a) const
    {
	return this->vextend(static_cast<FPos>(a));
    }

    template <class PosT1, class PosT2>
    inline Uptr<VPosBase> VPosBase::vextend(const MPos<PosT1,PosT2>& a) const
    {
	return this->vextend(static_cast<const PosT1&>(a))->vextend(a.next());
    }

    inline Uptr<VPosBase> VPosBase::vextend(const DPos& a) const
    {
	Uptr<VPosBase> out = nullptr;
	if(a.F()){
	    out = this->vextend(*dynamic_cast<const FPos*>(a.get()));
	}
	else {
	    out = this->vextend(*dynamic_cast<const UPos*>(a.get()));
	}
	if(a.size() > 1){
	    return out->vextend(a.next());
	}
	else {
	    return out;
	}
    }
    
    inline Uptr<VPosBase> VPosBase::vextend(const DPosRef& a) const
    {
	Uptr<VPosBase> out = nullptr;
	if(a.F()){
	    out = this->vextend(*dynamic_cast<const FPos*>(a.get()));
	}
	else {
	    out = this->vextend(*dynamic_cast<const UPos*>(a.get()));
	}
	if(a.size() > 1){
	    return out->vextend(a.next());
	}
	else {
	    return out;
	}
    }

    /************
     *   VPos   *
     ************/

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
    bool VPos<PosT>::F() const
    {
	if constexpr(std::is_same<PosT,FPos>::value){
	    return true;
	}
	else {
	    return false;
	}
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

    template <class PosT>
    Uptr<VPosBase> VPos<PosT>::vextend(const UPos& a) const
    {
	typedef decltype(this->extend(a)) OPosT;
	return std::make_unique<VPos<OPosT>>(this->extend(a));
    } 

    template <class PosT>
    Uptr<VPosBase> VPos<PosT>::vextend(const FPos& a) const
    {
	typedef decltype(this->extend(a)) OPosT;
	return std::make_unique<VPos<OPosT>>(this->extend(a));
    }
    // .... probably I need to define a static instanciation limit...

    /******************
     *   VPos<MPos>   *
     ******************/

    template <class PosT1, class PosT2>
    VPos<MPos<PosT1,PosT2>>::VPos(const MPos<PosT1,PosT2>& a) :
	MPos<PosT1,PosT2>(a),
	mTRef(static_cast<const PosT1*>(this))
    {
	if constexpr(not std::is_same<PosT2,DPos>::value){
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
    bool VPos<MPos<PosT1,PosT2>>::F() const
    {
	if constexpr(std::is_same<PosT1,FPos>::value){
	    return true;
	}
	else {
	    return false;
	}
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
	if constexpr(not std::is_same<PosT2,DPos>::value){
	    return &mNRef;
	}
	else {
	    return this->next().get();
	}
    }
    
    template <class PosT1, class PosT2>
    Uptr<VPosBase> VPos<MPos<PosT1,PosT2>>::vplus(const VPosBase* a) const
    {
	typedef decltype((*this) + DPosRef(a)) OPosT;
	return std::make_unique<VPos<OPosT>>((*this) + DPosRef(a));
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

    template <class PosT1, class PosT2>
    Uptr<VPosBase> VPos<MPos<PosT1,PosT2>>::vextend(const UPos& a) const 
    {
	if constexpr(pos_depth<PosT2>::value < MAX_VMPOS_DEPTH-1){
	    typedef decltype(this->extend(a)) OPosT;
	    return std::make_unique<VPos<OPosT>>(this->extend(a));
	}
	else {
	    typedef decltype(this->extend(DPos(a))) OPosT;
	    return std::make_unique<VPos<OPosT>>(this->extend(DPos(a)));
	    //CXZ_ERROR("preliminary...");
	    //return nullptr;
	}
    }
    
    template <class PosT1, class PosT2>
    Uptr<VPosBase> VPos<MPos<PosT1,PosT2>>::vextend(const FPos& a) const 
    {
	if constexpr(pos_depth<PosT2>::value < MAX_VMPOS_DEPTH-1){
	    typedef decltype(this->extend(a)) OPosT;
	    return std::make_unique<VPos<OPosT>>(this->extend(a));
	}
	else {
	    CXZ_ERROR("preliminary...");
	    return nullptr;
	}
    }
    
    /***************
     *   VPosRef   *
     ***************/

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
    bool VPosRef<PosT>::F() const
    {
	if constexpr(std::is_same<PosT,FPos>::value){
	    return true;
	}
	else {
	    return false;
	}
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

    template <class PosT>
    Uptr<VPosBase> VPosRef<PosT>::vextend(const UPos& a) const 
    {
	typedef decltype(mC->extend(a)) OPosT;
	return std::make_unique<VPos<OPosT>>(mC->extend(a));
    }
    
    template <class PosT>
    Uptr<VPosBase> VPosRef<PosT>::vextend(const FPos& a) const 
    {
	typedef decltype(mC->extend(a)) OPosT;
	return std::make_unique<VPos<OPosT>>(mC->extend(a));
    }

    /*********************
     *   VPosRef<MPos>   *
     *********************/

    template <class PosT1, class PosT2>
    VPosRef<MPos<PosT1,PosT2>>::VPosRef(const MPos<PosT1,PosT2>* c) :
	mC(c),
	mTRef(static_cast<const PosT1*>(mC))
    {
	if constexpr(not std::is_same<PosT2,DPos>::value){
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
    bool VPosRef<MPos<PosT1,PosT2>>::F() const
    {
	if constexpr(std::is_same<PosT1,FPos>::value){
	    return true;
	}
	else {
	    return false;
	}
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
	if constexpr(not std::is_same<PosT2,DPos>::value){
	    return &mNRef;
	}
	else {
	    return mC->next().get();
	}
    }

    template <class PosT1, class PosT2>
    Uptr<VPosBase> VPosRef<MPos<PosT1,PosT2>>::vplus(const VPosBase* a) const
    {
	typedef decltype((*mC) + DPosRef(a)) OPosT;
	return std::make_unique<VPos<OPosT>>((*mC) + DPosRef(a));
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

    template <class PosT1, class PosT2>
    Uptr<VPosBase> VPosRef<MPos<PosT1,PosT2>>::vextend(const UPos& a) const
    {
	if constexpr(pos_depth<PosT2>::value < MAX_VMPOS_DEPTH-1){
	    typedef decltype(mC->extend(a)) OPosT;
	    return std::make_unique<VPos<OPosT>>(mC->extend(a));
	}
	else {
	    CXZ_ERROR("preliminary...");
	    return nullptr;
	}
    }

    template <class PosT1, class PosT2>
    Uptr<VPosBase> VPosRef<MPos<PosT1,PosT2>>::vextend(const FPos& a) const
    {
	if constexpr(pos_depth<PosT2>::value < MAX_VMPOS_DEPTH-1){
	    typedef decltype(mC->extend(a)) OPosT;
	    return std::make_unique<VPos<OPosT>>(mC->extend(a));
	}
	else {
	    CXZ_ERROR("preliminary...");
	    return nullptr;
	}
    }

}

#endif
