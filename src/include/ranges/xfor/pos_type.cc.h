
#ifndef __cxz_pos_type_cc_h__
#define __cxz_pos_type_cc_h__

#include "pos_type.h"

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
    
    template <class PosT>
    constexpr UPos UPos::operator*(const PosT& in) const
    {
	return UPos(mExt * in.val());
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
    constexpr FPos UPos::operator*(const PosT& in) const
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
    template <SizeT N1>
    constexpr auto SFPos<N,Ms...>::operator*(const UPos& a) const
    {
	static const Arr<SizeT,sizeof...(Ms)> ms({ Ms... });
	return FPos(N * a.val(), &ms[0]);
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
    constexpr const NPos& MPos<BPosT,NPosT>::next() const
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
	typedef decltype(mNext * a.next()) ONPosT;
	return MPos<OBPosT,ONPosT>( BPosT::operator*(a), mNext * a.next() );
    }

    template <class BPosT, class NPosT>
    template <class PosT>
    constexpr auto MPos<BPosT,NPosT>::operator()(const PosT& a) const
    {
	typedef decltype(BPosT::operator()(a)) OBPosT;
	typedef decltype(mNext(a.next())) ONPosT;
	return MPos<OBPosT,ONPosT>( BPosT::operator()(a), mNext(a.next()) );
    }
	
    template <class BPosT, class NPosT>
    template <class PosT>
    constexpr auto MPos<BPosT,NPosT>::extend(const PosT& a) const
    {
	return mNext.extend(a);
    }

}

#endif
