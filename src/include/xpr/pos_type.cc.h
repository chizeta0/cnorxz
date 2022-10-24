
#ifndef __cxz_pos_type_cc_h__
#define __cxz_pos_type_cc_h__

#include "pos_type.h"
#include "vpos_type.h"

namespace CNORXZ
{

    /************
     *   SPos   *
     ************/

    template <SizeT N>
    constexpr SizeT SPos<N>::size() const
    {
	return 1;
    }
    
    template <SizeT N>
    constexpr SizeT SPos<N>::val() const
    {
	return N;
    }

    template <SizeT N>
    template <SizeT N1>
    constexpr auto SPos<N>::operator+(const SPos<N1>& a) const
    {
	return SPos<N+N1>();
    }
    
    template <SizeT N>
    template <SizeT N1>
    constexpr auto SPos<N>::operator*(const SPos<N1>& a) const
    {
	return SPos<N*N1>();
    }

    template <SizeT N>
    template <SizeT N1>
    constexpr auto SPos<N>::operator()(const SPos<N1>& a) const
    {
	return SPos<N*N1>();
    }

    template <SizeT N>
    constexpr auto SPos<N>::operator+(const UPos& a) const
    {
	return UPos(N+a.val());
    }
    
    template <SizeT N>
    constexpr auto SPos<N>::operator*(const UPos& a) const
    {
	return UPos(N*a.val());
    }

    template <SizeT N>
    constexpr auto SPos<N>::operator()(const UPos& a) const
    {
	return UPos(N*a.val());
    }

    template <SizeT N>
    template <class PosT>
    constexpr auto SPos<N>::extend(const PosT& a) const
    {
	return MPos<SPos<N>,PosT>(*this,a);
    }

    template <SizeT N>
    constexpr SPos<N>::operator UPos() const
    {
	return UPos(N);
    }

    /************
     *   UPos   *
     ************/

    constexpr UPos::UPos(SizeT ext) : mExt(ext) {}
    
    constexpr SizeT UPos::size() const
    {
	return 1;
    }
    
    constexpr const SizeT& UPos::val() const
    {
	return mExt;
    }
    
    template <class PosT>
    constexpr UPos UPos::operator+(const PosT& in) const
    {
	return UPos(mExt + in.val());
    }

    constexpr SPos<0> UPos::operator*(const SPos<0>& a) const
    {
	return SPos<0>();
    }
    
    template <class PosT>
    constexpr UPos UPos::operator*(const PosT& in) const
    {
	return UPos(mExt * in.val());
    }

    constexpr SPos<0> UPos::operator()(const SPos<0>& a) const
    {
	return SPos<0>();
    }
	
    template <class PosT>
    constexpr UPos UPos::operator()(const PosT& in) const
    {
	return UPos(mExt * in.val());
    }
	
    template <class PosT>
    constexpr auto UPos::extend(const PosT& p1) const
    {
	return MPos<UPos,PosT>(*this, p1);
    }

    /************
     *   FPos   *
     ************/

    inline FPos::FPos(SizeT ext, const SizeT* map) : mExt(ext), mMap(map) {}
    
    constexpr SizeT FPos::size() const
    {
	return 1;
    }
    
    constexpr const SizeT& FPos::val() const
    {
	return mExt;
    }
	
    template <class PosT1>
    constexpr UPos FPos::operator+(const PosT1& a) const
    {
	return UPos(mExt + a.val());
    }

    template <class PosT>
    constexpr FPos FPos::operator*(const PosT& in) const
    {
	return FPos(mExt * in.val(), mMap);
    }

    template <class PosT1>
    constexpr UPos FPos::operator()(const PosT1& a) const
    {
	return UPos(mExt * mMap[a.val()]);
    }

    template <class PosT1>
    constexpr auto FPos::extend(const PosT1& a) const
    {
	return MPos<FPos,PosT1>(*this,a);
    }

    /*************
     *   SFPos   *
     *************/

    template <SizeT N, SizeT... Ms>
    Arr<SizeT,sizeof...(Ms)> SFPos<N,Ms...>::sMs = { Ms... };
    
    template <SizeT N, SizeT... Ms>
    constexpr SizeT SFPos<N,Ms...>::size() const
    {
	return 1;
    }
    
    template <SizeT N, SizeT... Ms>
    constexpr SizeT SFPos<N,Ms...>::val() const
    {
	return N;
    }

    template <SizeT N, SizeT... Ms>
    template <SizeT N1>
    constexpr auto SFPos<N,Ms...>::operator+(const SPos<N1>& a) const
    {
	return SPos<N+N1>();
    }
    
    template <SizeT N, SizeT... Ms>
    template <SizeT N1>
    constexpr auto SFPos<N,Ms...>::operator*(const SPos<N1>& a) const
    {
	return SFPos<N*N1,Ms...>();
    }

    template <SizeT N, SizeT... Ms>
    template <SizeT N1>
    constexpr auto SFPos<N,Ms...>::operator()(const SPos<N1>& a) const
    {
	constexpr Arr<SizeT,sizeof...(Ms)> ms({ Ms... });
	return SPos<N*std::get<a.val()>(ms)>();
    }

    template <SizeT N, SizeT... Ms>
    constexpr auto SFPos<N,Ms...>::operator+(const UPos& a) const
    {
	return UPos(N + a.val());
    }
    
    template <SizeT N, SizeT... Ms>
    constexpr auto SFPos<N,Ms...>::operator*(const UPos& a) const
    {
	return FPos(N * a.val(), &sMs[0]);
    }

    template <SizeT N, SizeT... Ms>
    constexpr auto SFPos<N,Ms...>::operator()(const UPos& a) const
    {
	constexpr Arr<SizeT,sizeof...(Ms)> ms({ Ms... });
	return UPos(N * ms[a.val()]);
    }

    template <SizeT N, SizeT... Ms>
    template <class PosT>
    constexpr auto SFPos<N,Ms...>::extend(const PosT& a) const
    {
	return MPos<SFPos<N,Ms...>,PosT>(*this,a);
    }

    template <SizeT N, SizeT... Ms>
    constexpr SFPos<N,Ms...>::operator FPos() const
    {
	return FPos(N, &sMs[0]);
    }

    /************
     *   MPos   *
     ************/

    template <class BPosT, class NPosT>
    constexpr MPos<BPosT,NPosT>::MPos()
    {
	static_assert(is_scalar_pos_type<BPosT>::value,
		      "MPos has to be derived from scalar pos type");
    }

    template <class BPosT, class NPosT>
    constexpr MPos<BPosT,NPosT>::MPos(const BPosT& b, const NPosT& n) :
	BPosT(b), mNext(n)
    {
	static_assert(is_scalar_pos_type<BPosT>::value,
		      "MPos has to be derived from scalar pos type");
    }

    template <class BPosT, class NPosT>
    template <typename... Args>
    constexpr MPos<BPosT,NPosT>::MPos(Args&&... args, const NPosT& next) :
	BPosT(args...), mNext(next)
    {
	static_assert(is_scalar_pos_type<BPosT>::value,
		      "MPos has to be derived from scalar pos type");
    }

    template <class BPosT, class NPosT>
    template <typename... Args>
    constexpr MPos<BPosT,NPosT>::MPos(Args&&... args, NPosT&& next) :
	BPosT(args...), mNext(next)
    {
	static_assert(is_scalar_pos_type<BPosT>::value,
		      "MPos has to be derived from scalar pos type");
    }

    template <class BPosT, class NPosT>
    constexpr SizeT MPos<BPosT,NPosT>::size() const
    {
	return BPosT::size() + mNext.size();
    }

    template <class BPosT, class NPosT>
    constexpr const NPosT& MPos<BPosT,NPosT>::next() const
    {
	return mNext;
    }

    template <class BPosT, class NPosT>
    template <class PosT>
    constexpr auto MPos<BPosT,NPosT>::operator+(const PosT& a) const
    {
	typedef decltype(BPosT::operator+(a)) OBPosT;
	typedef decltype(mNext + a.next()) ONPosT;
	return MPos<OBPosT,ONPosT>( BPosT::operator+(a), mNext + a.next() );
    }

    template <class BPosT, class NPosT>
    template <class PosT>
    constexpr auto MPos<BPosT,NPosT>::operator*(const PosT& a) const
    {
	typedef decltype(BPosT::operator*(a)) OBPosT;
	typedef decltype(mNext * a) ONPosT;
	return MPos<OBPosT,ONPosT>( BPosT::operator*(a), mNext * a );
    }

    template <class BPosT, class NPosT>
    template <class PosT>
    constexpr auto MPos<BPosT,NPosT>::operator()(const PosT& a) const
    {
	typedef decltype(BPosT::operator()(a)) OBPosT;
	typedef decltype(mNext(a)) ONPosT;
	return MPos<OBPosT,ONPosT>( BPosT::operator()(a), mNext(a) );
    }
	
    template <class BPosT, class NPosT>
    template <class PosT>
    constexpr auto MPos<BPosT,NPosT>::extend(const PosT& a) const
    {
	typedef decltype(mNext.extend(a)) ONPosT;
	return MPos<BPosT,ONPosT>(*this, mNext.extend(a));
    }

    /************
     *   DPos   *
     ************/

    inline DPos::DPos(Uptr<VPosBase>&& a) :
	ObjHandle<VPosBase>(std::forward<Uptr<VPosBase>>(a))
    {}

    inline DPos::DPos(const DPosRef& a) :
	ObjHandle<VPosBase>(std::forward<Uptr<VPosBase>>(a.vpos()->copy()))
    {}

    template <class PosT>
    inline DPos::DPos(const PosT& a) :
	ObjHandle<VPosBase>
	(std::make_unique<VPos<typename std::remove_reference<PosT>::type>>(a))
    {
	static_assert(is_pos_type<typename std::remove_reference<PosT>::type>::value,
		      "DPos can only be constructed from pos types");
    }
    
    template <class PosT>
    inline DPos::DPos(PosT&& a) :
	ObjHandle<VPosBase>
	(std::make_unique<VPos<typename std::remove_reference<PosT>::type>>(a))
    {
	static_assert(is_pos_type<typename std::remove_reference<PosT>::type>::value,
		      "DPos can only be constructed from pos types");
    }

    inline const VPosBase* DPos::get() const
    {
	return mC->vget();
    }

    inline const VPosBase* DPos::vpos() const
    {
	return mC.get();
    }

    inline SizeT DPos::size() const
    {
	return mC->vsize();
    }
    
    inline SizeT DPos::val() const
    {
	return mC->vval();
    }
    
    //inline DPosRef DPos::next() const
    inline DPosRef DPos::sub() const
    {
	return DPosRef(mC->vnext());
    }

    template <class PosT>
    inline DPos DPos::operator+(const PosT& a) const
    {
	if constexpr(std::is_same<PosT,DPos>::value or std::is_same<PosT,DPosRef>::value){
	    return DPos(mC->vplus( a.vpos() ));
	}
	else {
	    VPosRef<PosT> b(&a);
	    return DPos(mC->vplus( &b ));
	}
    }

    template <class PosT>
    inline DPos DPos::operator*(const PosT& a) const
    {
	if constexpr(std::is_same<PosT,DPos>::value or std::is_same<PosT,DPosRef>::value){
	    return DPos(mC->vtimes( a.vpos() ));
	}
	else {
	    VPosRef<PosT> b(&a);
	    return DPos(mC->vtimes( &b ));
	}
    }

    // same as operator*, except for FPos/SFPos, where map is executed
    template <class PosT>
    inline DPos DPos::operator()(const PosT& a) const
    {
	if constexpr(std::is_same<PosT,DPos>::value or std::is_same<PosT,DPosRef>::value){
	    return DPos(mC->vexec( a.vpos() ));
	}
	else {
	    VPosRef<PosT> b(&a);
	    return DPos(mC->vexec( &b ));
	}
    }
	
    template <class PosT>
    inline decltype(auto) DPos::extend(const PosT& a) const
    {
	//return DPos(mC->vextend( a ));
	return MPos<DPos,PosT>(*this,a);
    }

    /***************
     *   DPosRef   *
     ***************/

    inline DPosRef::DPosRef(const VPosBase* p) : mP(p) {}

    inline const VPosBase* DPosRef::get() const
    {
	return mP->vget();
    }

    inline const VPosBase* DPosRef::vpos() const
    {
	return mP;
    }

    inline SizeT DPosRef::size() const
    {
	return mP->vsize();
    }
    
    inline SizeT DPosRef::val() const
    {
	return mP->vval();
    }
    
    //inline DPosRef DPosRef::next() const
    inline DPosRef DPosRef::sub() const
    {
	return DPosRef(mP->vnext());
    }

    template <class PosT>
    inline DPos DPosRef::operator+(const PosT& a) const
    {
	if constexpr(std::is_same<PosT,DPos>::value or std::is_same<PosT,DPosRef>::value){
	    return DPos(mP->vplus( a.vpos() ));
	}
	else {
	    VPosRef<PosT> b(&a);
	    return DPos(mP->vplus( &b ));
	}
    }

    template <class PosT>
    inline DPos DPosRef::operator*(const PosT& a) const
    {
	if constexpr(std::is_same<PosT,DPos>::value or std::is_same<PosT,DPosRef>::value){
	    return DPos(mP->vtimes( a.vpos() ));
	}
	else {
	    VPosRef<PosT> b(&a);
	    return DPos(mP->vtimes( &b ));
	}
    }

    // same as operator*, except for FPos/SFPos, where map is executed
    template <class PosT>
    inline DPos DPosRef::operator()(const PosT& a) const
    {
	if constexpr(std::is_same<PosT,DPos>::value or std::is_same<PosT,DPosRef>::value){
	    return DPos(mP->vexec( a.vpos() ));
	}
	else {
	    VPosRef<PosT> b(&a);
	    return DPos(mP->vexec( &b ));
	}
    }	

    template <class PosT>
    inline decltype(auto) DPosRef::extend(const PosT& a) const
    {
	//return DPos(mP->vextend( a ));
	return MPos<DPos,PosT>(*this,a);
    }
    
    /************
     *   EPos   *
     ************/

    template <class BPosT, class... OPosTs>
    constexpr EPos<BPosT,OPosTs...>::EPos(const BPosT& b, const OPosTs&... os) :
	BPosT(b),
	mP(os...)
    {}

    template <class BPosT, class... OPosTs>
    constexpr EPos<BPosT,OPosTs...>::EPos(BPosT&& b, OPosTs&&... os) :
	BPosT(b),
	mP(os...)
    {}

    template <class BPosT, class... OPosTs>
    constexpr decltype(auto) EPos<BPosT,OPosTs...>::val() const
    {
	return ival(std::index_sequence_for<OPosTs...>{});
    }

    template <class BPosT, class... OPosTs>
    constexpr decltype(auto) EPos<BPosT,OPosTs...>::next() const
    {
	return inext(std::index_sequence_for<OPosTs...>{});
    }

    template <class BPosT, class... OPosTs>
    template <SizeT I>
    constexpr decltype(auto) EPos<BPosT,OPosTs...>::get() const
    {
	return std::get<I>(mP);
    }

    template <class BPosT, class... OPosTs>
    template <SizeT... Is>
    constexpr decltype(auto) EPos<BPosT,OPosTs...>::ival(std::index_sequence<Is...> is) const
    {
	if constexpr(is_static_pos_type<EPos<BPosT,OPosTs...>>::value){
	    return std::index_sequence<std::get<Is>(mP).val()...>{};
	}
	else {
	    return Arr<SizeT,is.size()> { std::get<Is>(mP).val()... };
	}
    }

    template <class BPosT, class... OPosTs>
    template <SizeT... Is>
    constexpr decltype(auto) EPos<BPosT,OPosTs...>::inext(std::index_sequence<Is...> is) const
    {
	typedef EPos<decltype(next()),decltype(std::get<Is>(mP).next())...> OEPosT;
	return OEPosT(next(), std::get<Is>(mP).next()...);
    }

    /*********************************
     *   Traits and Helper-Classes   *
     *********************************/

    template <SizeT N, class BPosT, class OPosT>
    decltype(auto) mkEPos(const BPosT& a, const OPosT& b)
    {
	return mkiEPos(a, b, std::make_index_sequence<N>{});
    }

    template <SizeT N, class BPosT, class OPosT, SizeT... Is>
    decltype(auto) mkiEPos(const BPosT& a, const OPosT& b, std::index_sequence<Is...> is)
    {
	return EPos<BPosT,decltype(b*SPos<Is>())...>(a, b*SPos<Is>()...);
    }
    
    template <class OPosT1, class OPosT2, class... OPosTs>
    constexpr bool pos_types_consecutive_2<OPosT1,OPosT2,OPosTs...>::eval()
    {
	if constexpr(is_static_pos_type<OPosT1>::value and is_static_pos_type<OPosT2>::value){
	    if constexpr(sizeof...(OPosTs) != 0){
		return OPosT1().val() < OPosT2().val() and pos_types_consecutive<OPosT2,OPosTs...>::value;
	    }
	    else {
		return OPosT1().val() < OPosT2().val();
	    }
	}
	return false;
    }

    template <class OPosT1, class... OPosTs>
    constexpr bool pos_type_same<OPosT1,OPosTs...>::eval()
    {
	if constexpr(is_static_pos_type<OPosT1>::value){
	    if constexpr(sizeof...(OPosTs) != 0){
		return OPosT1().val() == 0 and pos_type_same<OPosTs...>::value;
	    }
	    else {
		return OPosT1().val() == 0;
	    }
	}
	else {
	    return false;
	}
    }
}

#endif
