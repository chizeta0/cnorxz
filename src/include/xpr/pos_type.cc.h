// -*- C++ -*-
/**
   
   @file include/xpr/pos_type.cc.h
   @brief Position types template implementations.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_pos_type_cc_h__
#define __cxz_pos_type_cc_h__

#include "pos_type.h"
#include "vpos_type.h"

namespace CNORXZ
{
    
    /*==========+
     |   SPos   |
     +==========*/

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
    constexpr decltype(auto) SPos<N>::operator-(const SPos<N1>& a) const
    {
	return SPos<N-N1>();
    }

    template <SizeT N>
    template <class PosT>
    constexpr decltype(auto) SPos<N>::operator+(const PosT& a) const
    {
	if constexpr(is_epos_type<PosT>::value){
	    return mkEPos<epos_size<PosT>::value>(*this,SPos<0>{}) + a;
	}
	else if constexpr(is_static_pos_type<PosT>::value){
	    return SPos<N+a.val()>{};
	}
	else {
	    return UPos(N+a.val());
	}
    }

    template <SizeT N>
    template <class PosT>
    constexpr decltype(auto) SPos<N>::operator*(const PosT& a) const
    {
	if constexpr(is_epos_type<PosT>::value){
	    return mkEPos<epos_size<PosT>::value>(*this,SPos<0>{}) * a;
	}
	else {
	    if constexpr(N == 0) {
		return SPos<0>{};
	    }
	    else if constexpr(is_static_pos_type<PosT>::value){
		return SPos<N*a.val()>{};
	    }
	    else {
		return UPos(N*a.val());
	    }
	}
    }

    template <SizeT N>
    template <class PosT>
    constexpr decltype(auto) SPos<N>::operator()(const PosT& a) const
    {
	if constexpr(is_epos_type<PosT>::value){
	    return mkEPos<epos_size<PosT>::value>(*this,SPos<0>{}) * a;
	}
	else {
	    if constexpr(N == 0) {
		return SPos<0>{};
	    }
	    else if constexpr(is_static_pos_type<PosT>::value){
		return SPos<N*a.val()>{};
	    }
	    else {
		return UPos(N*a.val());
	    }
	}
    }
    
    template <SizeT N>
    template <class PosT>
    constexpr decltype(auto) SPos<N>::extend(const PosT& a) const
    {
	return MPos<SPos<N>,PosT>(*this,a);
    }

    template <SizeT N>
    template <class PosT>
    constexpr decltype(auto) SPos<N>::operator<<(const PosT& a) const
    {
	return extend(a);
    }

    template <SizeT N>
    constexpr SPos<N>::operator UPos() const
    {
	return UPos(N);
    }

    template <SizeT N>
    constexpr SPos<N>::operator SizeT() const
    {
	return val();
    }

    /*==========+
     |   UPos   |
     +==========*/

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
    constexpr decltype(auto) UPos::operator+(const PosT& in) const
    {
	if constexpr(is_epos_type<PosT>::value){
	    //return in;
	    return mkEPos<epos_size<PosT>::value>(*this,SPos<0>{}) + in;
	}
	else {
	    return UPos(mExt + in.val());
	}
    }

    template <class PosT>
    constexpr decltype(auto) UPos::operator-(const PosT& in) const
    {
	return UPos(mExt - in.val());
    }

    template <class PosT>
    constexpr decltype(auto) UPos::operator*(const PosT& in) const
    {
	if constexpr(is_epos_type<PosT>::value){
	    return mkEPos<epos_size<PosT>::value>(*this,SPos<0>{}) * in;
	}
	else {
	    if constexpr(std::is_same<PosT,SPos<0>>::value){
		return SPos<0>{};
	    }
	    else {
		return UPos(mExt * in.val());
	    }
	}
    }

    template <class PosT>
    constexpr decltype(auto) UPos::operator()(const PosT& in) const
    {
	if constexpr(is_epos_type<PosT>::value){
	    return mkEPos<epos_size<PosT>::value>(*this,SPos<0>{}) * in;
	    //return in*(*this);
	}
	else {
	    if constexpr(std::is_same<PosT,SPos<0>>::value){
		return SPos<0>{};
	    }
	    else {
		return UPos(mExt * in.val());
	    }
	}
    }
	
    template <class PosT>
    constexpr decltype(auto) UPos::extend(const PosT& p1) const
    {
	return MPos<UPos,PosT>(*this, p1);
    }

    template <class PosT>
    constexpr decltype(auto) UPos::operator<<(const PosT& a) const
    {
	return extend(a);
    }

    constexpr UPos::operator SizeT() const
    {
	return val();
    }

    /*==========+
     |   FPos   |
     +==========*/

    inline FPos::FPos(SizeT ext, const SizeT* map, SizeT max) :
	mExt(ext), mMap(map), mMax(max) {}
    
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
    constexpr decltype(auto) FPos::operator*(const PosT& a) const
    {
	if constexpr(is_epos_type<PosT>::value){
	    return FPos(mExt * a.scal().val(), mMap);
	}
	else {
	    return FPos(mExt * a.val(), mMap);
	}
    }
    
    template <class PosT1>
    constexpr decltype(auto) FPos::operator()(const PosT1& a) const
    {
	if constexpr(is_epos_type<PosT1>::value){
	    return UPos(mExt * mMap[a.scal().val()]);
	}
	else {
	    return UPos(mExt * mMap[a.val()]);
	}
    }

    template <class PosT>
    constexpr decltype(auto) FPos::extend(const PosT& a) const
    {
	return MPos<FPos,PosT>(*this,a);
    }

    template <class PosT>
    constexpr decltype(auto) FPos::operator<<(const PosT& a) const
    {
	return extend(a);
    }

    constexpr FPos::operator SizeT() const
    {
	return val();
    }

    constexpr SizeT FPos::max() const
    {
	return mMax;
    }

    template <class PosT>
    struct MkFPos
    {
	static constexpr decltype(auto) mk(const PosT& pos, const SizeT* map)
	{
	    return FPos(pos.val(), map);
	}
    };

    template <class BPosT, class NPosT>
    struct MkFPos<MPos<BPosT,NPosT>>
    {
	static constexpr decltype(auto) mk(const MPos<BPosT,NPosT>& pos, const SizeT* map)
	{
	    return mkMPos( MkFPos<BPosT>::mk( pos, map ), MkFPos<NPosT>::mk( pos.next(), map ) );
	}
    };

    template <class PosT>
    constexpr decltype(auto) mkFPos(const PosT& pos, const SizeT* map)
    {
	return MkFPos<PosT>::mk(pos, map);
    }

    
    /*===========+
     |   SFPos   |
     +===========*/

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
    template <class PosT>
    constexpr decltype(auto) SFPos<N,Ms...>::operator+(const PosT& a) const
    {
	if constexpr(is_static_pos_type<PosT>::value){
	    return SPos<N+a.val()>();
	}
	else {
	    return UPos(N + a.val());
	}
    }
    
    template <SizeT N, SizeT... Ms> 
    template <class PosT>
    constexpr decltype(auto) SFPos<N,Ms...>::operator*(const PosT& a) const
    {
	if constexpr(is_static_pos_type<PosT>::value){
	    if constexpr(is_epos_type<PosT>::value){
		return SFPos<N*a.scal().val(),Ms...>();
	    }
	    else {
		return SFPos<N*a.val(),Ms...>();
	    }
	}
	else {
	    if constexpr(is_epos_type<PosT>::value){
		return FPos(N * a.scal().val(), &sMs[0]);
	    }
	    else {
		return FPos(N * a.val(), &sMs[0]);
	    }
	}
    }

    template <SizeT N, SizeT... Ms>
    template <class PosT>
    constexpr decltype(auto) SFPos<N,Ms...>::operator()(const PosT& a) const
    {
	if constexpr(is_static_pos_type<PosT>::value){
	    constexpr Arr<SizeT,sizeof...(Ms)> ms({ Ms... });
	    if constexpr(is_epos_type<PosT>::value){
		return SPos<N*std::get<a.scal().val()>(ms)>();
	    }
	    else {
		return SPos<N*std::get<a.val()>(ms)>();
	    }
	}
	else {
	    constexpr Arr<SizeT,sizeof...(Ms)> ms({ Ms... });
	    if constexpr(is_epos_type<PosT>::value){
		return UPos(N * ms[a.scal().val()]);
	    }
	    else {
		return UPos(N * ms[a.val()]);
	    }
	}
    }

    template <SizeT N, SizeT... Ms>
    template <class PosT>
    constexpr decltype(auto) SFPos<N,Ms...>::extend(const PosT& a) const
    {
	return MPos<SFPos<N,Ms...>,PosT>(*this,a);
    }

    template <SizeT N, SizeT... Ms>
    template <class PosT>
    constexpr decltype(auto) SFPos<N,Ms...>::operator<<(const PosT& a) const
    {
	return extend(a);
    }

    template <SizeT N, SizeT... Ms>
    constexpr SFPos<N,Ms...>::operator FPos() const
    {
	return FPos(N, &sMs[0]);
    }

    template <SizeT N, SizeT... Ms>
    constexpr SFPos<N,Ms...>::operator SizeT() const
    {
	return val();
    }

    template <SizeT N, SizeT... Ms>
    constexpr SizeT SFPos<N,Ms...>::max() const
    {
	return sizeof...(Ms); // CHECK!!!
    }
    
    /*==========+
     |   MPos   |
     +==========*/

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
    constexpr decltype(auto) MPos<BPosT,NPosT>::operator+(const PosT& a) const
    {
	typedef decltype(BPosT::operator+(a)) OBPosT;
	typedef decltype(mNext + a.next()) ONPosT;
	return MPos<OBPosT,ONPosT>( BPosT::operator+(a), mNext + a.next() );
    }

    template <class BPosT, class NPosT>
    template <class PosT>
    constexpr decltype(auto) MPos<BPosT,NPosT>::operator*(const PosT& a) const
    {
	typedef decltype(BPosT::operator*(a)) OBPosT;
	typedef decltype(mNext * a) ONPosT;
	return MPos<OBPosT,ONPosT>( BPosT::operator*(a), mNext * a );
    }

    template <class BPosT, class NPosT>
    template <class PosT>
    constexpr decltype(auto) MPos<BPosT,NPosT>::operator()(const PosT& a) const
    {
	typedef decltype(BPosT::operator()(a)) OBPosT;
	typedef decltype(mNext(a)) ONPosT;
	return MPos<OBPosT,ONPosT>( BPosT::operator()(a), mNext(a) );
    }
	
    template <class BPosT, class NPosT>
    template <class PosT>
    constexpr decltype(auto) MPos<BPosT,NPosT>::extend(const PosT& a) const
    {
	typedef decltype(mNext.extend(a)) ONPosT;
	return MPos<BPosT,ONPosT>(*this, mNext.extend(a));
    }

    template <class BPosT, class NPosT>
    template <class PosT>
    constexpr decltype(auto) MPos<BPosT,NPosT>::operator<<(const PosT& a) const
    {
	return extend(a);
    }
    
    template <class BPosT, class NPosT>
    constexpr decltype(auto) mkMPos(const BPosT& bpos, const NPosT& npos)
    {
	return MPos<BPosT,NPosT>(bpos, npos);
    }

    /*==========+
     |   DPos   |
     +==========*/

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

    inline DPos::operator SizeT() const
    {
	return val();
    }

    /*=============+
     |   DPosRef   |
     +=============*/

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

    inline DPosRef::operator SizeT() const
    {
	return val();
    }
    
    /*==========+
     |   EPos   |
     +==========*/

    template <class BPosT, class... OPosTs>
    constexpr EPos<BPosT,OPosTs...>::EPos(const BPosT& b, const OPosTs&... os) :
	BPosT(b),
	mP(os...)
    {
	static_assert(static_pos_size<BPosT>::value == 1, "only single pos types allowed");
	static_assert(((static_pos_size<OPosTs>::value == 1) and ...), "only single pos types allowed");
    }

    template <class BPosT, class... OPosTs>
    constexpr EPos<BPosT,OPosTs...>::EPos(BPosT&& b, OPosTs&&... os) :
	BPosT(b),
	mP(os...)
    {
	static_assert(static_pos_size<BPosT>::value == 1, "only single pos types allowed");
	static_assert(((static_pos_size<OPosTs>::value == 1) and ...), "only single pos types allowed");
    }

    // TODO: transform to trivial epos if not epos!!!
    template <class BPosT, class... OPosTs>
    template <class PosT>
    constexpr decltype(auto) EPos<BPosT,OPosTs...>::operator+(const PosT& a) const
    {
	static_assert(static_pos_size<BPosT>::value == 1, "only single pos types allowed");
	static_assert(((static_pos_size<OPosTs>::value == 1) and ...), "only single pos types allowed");
	if constexpr(is_epos_type<PosT>::value){
	    return iter<0,sizeof...(OPosTs)>
		( [&](auto i) { return std::get<i>(mP)+a.template oget<i>(); },
		  [&](const auto&... e) { return epos(BPosT::operator+(a.scal()),e...); } );
	}
	else {
	    auto ax = mkEPos<sizeof...(OPosTs)>(a,a*SPos<0>{});
	    return iter<0,sizeof...(OPosTs)>
		( [&](auto i) { return std::get<i>(mP)+ax.template oget<i>(); },
		  [&](const auto&... e) { return epos(BPosT::operator+(a),e...); } );
	}
    }
    
    template <class BPosT, class... OPosTs>
    template <class PosT>
    constexpr decltype(auto) EPos<BPosT,OPosTs...>::operator*(const PosT& a) const
    {
	static_assert(static_pos_size<BPosT>::value == 1, "only single pos types allowed");
	static_assert(((static_pos_size<OPosTs>::value == 1) and ...), "only single pos types allowed");
	if constexpr(is_epos_type<PosT>::value){
	    return iter<0,sizeof...(OPosTs)>
		( [&](auto i) { auto x = scal(); auto y = a.scal();
		    auto q = std::get<i>(mP); auto p = a.template oget<i>();
		    return x*p+q*(y+p); },
		    [&](const auto&... e) { return epos(BPosT::operator*(a.scal()),e...); } );
	}
	else {
	    auto ax = mkEPos<sizeof...(OPosTs)>(a,a*SPos<0>{});
	    return iter<0,sizeof...(OPosTs)>
		( [&](auto i) { auto x = scal(); auto y = ax.scal();
		    auto q = std::get<i>(mP); auto p = ax.template oget<i>();
		    return x*p+q*(y+p); },
		  [&](const auto&... e) { return epos(BPosT::operator*(a),e...); } );
	}
	/*
	return iter<0,sizeof...(OPosTs)>
	    ( [&](auto i) { return std::get<i>(mP); },
	      [&](const auto&... e) { return EPos<decltype(BPosT::operator*(a)),
						  decltype(e*a)...>
		    (BPosT::operator*(a),e*a...); } );
	*/
    }

    template <class BPosT, class... OPosTs>
    template <class PosT>
    constexpr decltype(auto) EPos<BPosT,OPosTs...>::operator()(const PosT& a) const
    {
	return iter<0,sizeof...(OPosTs)>
	    ( [&](auto i) { return std::get<i>(mP); },
	      [&](const auto&... e) { return EPos<decltype(BPosT::operator()(a)),
						  decltype(e*a)...>
		    (BPosT::operator()(a),e*a...); } );
    }


    template <class BPosT, class... OPosTs>
    constexpr decltype(auto) EPos<BPosT,OPosTs...>::val() const
    {
	return ival(std::index_sequence_for<OPosTs...>{});
    }

    template <class BPosT, class... OPosTs>
    constexpr decltype(auto) EPos<BPosT,OPosTs...>::scal() const
    {
	return static_cast<const BPosT&>(*this);
    }

    template <class BPosT, class... OPosTs>
    template <SizeT I>
    constexpr decltype(auto) EPos<BPosT,OPosTs...>::get() const
    {
	return scal()+std::get<I>(mP);
    }

    template <class BPosT, class... OPosTs>
    template <SizeT I>
    constexpr decltype(auto) EPos<BPosT,OPosTs...>::oget() const
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
	    return Arr<SizeT,is.size()> { (BPosT::val()+std::get<Is>(mP).val())... };
	}
    }

    template <class BPosT, class... OPosTs>
    constexpr decltype(auto) epos(const BPosT& b, const OPosTs&... os)
    {
	return EPos<BPosT,OPosTs...>(b,os...);
    }

    
    /*===============================+
     |   Traits and Helper-Classes   |
     +===============================*/

    template <class BPosT, class OPosT, SizeT N>
    decltype(auto) MkEPos<BPosT,OPosT,N>::mk(const BPosT& a, const OPosT& b)
    {
	static_assert(static_pos_size<BPosT>::value == 1, "only single pos types allowed");
	static_assert(static_pos_size<OPosT>::value == 1, "only single pos types allowed");
	return mkiEPos(a, b, std::make_index_sequence<N>{});
    }

    template <class BPosT, class NPosT, class OPosT, class ONPosT, SizeT N>
    decltype(auto) MkEPos<MPos<BPosT,NPosT>,MPos<OPosT,ONPosT>,N>::mk(const MPos<BPosT,NPosT>& a, const MPos<OPosT,ONPosT>& b)
    {
	const BPosT& ax = static_cast<const BPosT&>(a);
	const OPosT& bx = static_cast<const OPosT&>(b);
	typedef decltype(mkEPos<N>(ax,bx)) T1;
	typedef decltype(mkEPos<N>(a.next(),b.next())) T2;
	return MPos<T1,T2>(mkEPos<N>(ax,bx), mkEPos<N>(a.next(),b.next()));
    }

    template <SizeT N, class BPosT, class OPosT>
    decltype(auto) mkEPos(const BPosT& a, const OPosT& b)
    {
	return MkEPos<BPosT,OPosT,N>::mk(a,b);
    }

    template <class BPosT, class OPosT, SizeT... Is>
    decltype(auto) mkiEPos(const BPosT& a, const OPosT& b, std::index_sequence<Is...> is)
    {
	return EPos<BPosT,decltype(b*SPos<Is>())...>(a, b*SPos<Is>()...);
    }
    
    template <class OPosT1, class OPosT2, class... OPosTs>
    constexpr bool pos_types_consecutive_2<OPosT1,OPosT2,OPosTs...>::eval()
    {
	if constexpr(is_static_pos_type<OPosT1>::value and is_static_pos_type<OPosT2>::value){
	    if constexpr(sizeof...(OPosTs) != 0){
		return OPosT1().val()+1 == OPosT2().val() and pos_types_consecutive<OPosT2,OPosTs...>::value;
	    }
	    else {
		return OPosT1().val()+1 == OPosT2().val();
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
