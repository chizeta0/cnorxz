
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
    template <class PosT>
    constexpr auto SPos<N>::extend(const CPosInterface<PosT>& a) const
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
    constexpr UPos UPos::operator+(const CPosInterface<PosT>& in) const
    {
	return UPos(mExt + in.val());
    }
    
    template <class PosT>
    constexpr UPos UPos::operator*(const CPosInterface<PosT>& in) const
    {
	return UPos(mExt * in.val());
    }
	
    template <class PosT>
    constexpr auto UPos::extend(const CPosInterface<PosT>& p1) const
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
    constexpr UPos FPos::operator+(const CPosInterface<PosT1>& a) const
    {
	return UPos(mExt + a.val());
    }

    template <class PosT1>
    constexpr UPos FPos::operator*(const CPosInterface<PosT1>& a) const
    {
	return UPos(mExt * mMap[a.val()]);
    }

    template <class PosT1>
    constexpr auto FPos::extend(const CPosInterface<PosT1>& a) const
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
	constexpr Arr<SizeT,sizeof...(Ms)> ms({ Ms... });
	return UPos(N * ms[a.val()]);
    }

    template <SizeT N, SizeT... Ms>
    template <class PosT>
    constexpr auto SFPos<N,Ms...>::extend(const CPosInterface<PosT>& a) const
    {
	return MPos<SFPos<N,Ms...>,PosT>(*this,a);
    }

    
    /************
     *   DPos   *
     ************/

    inline DPos::DPos(Uptr<VPosBase>&& a) :
	ObjHandle<VPosBase>(std::forward<Uptr<VPosBase>>(a))
    {}

    template <class PosT>
    inline DPos::DPos(const CPosInterface<PosT>& a) :
	ObjHandle<VPosBase>( std::make_unique<PosT>(a) )
    {}

    inline SizeT DPos::size() const
    {
	return mC->vsize();
    }
    
    inline SizeT DPos::val() const
    {
	return mC->vval();
    }
    
    inline DPosRef DPos::next() const
    {
	return DPosRef(mC->vnext());
    } 

    template <class PosT1>
    inline DPos DPos::operator+(const CPosInterface<PosT1>& a) const
    {
	return DPos(mC->vplus(VPosRef<PosT1>(&a))); // check memory safety!!!
    }
    
    template <class PosT1>
    inline DPos DPos::operator*(const CPosInterface<PosT1>& a) const
    {
	return DPos(mC->vtimes(VPosRef<PosT1>(&a))); // check memory safety!!!
    }    
    
    template <class PosT1>
    inline DPos DPos::extend(const CPosInterface<PosT1>& a) const
    {
	return DPos(mC->vextend(a));
    }
    
    /***************
     *   DPosRef   *
     ***************/
    
    inline DPosRef::DPosRef(const VPosBase* c) :
	mC(c)
    {}
    
    inline SizeT DPosRef::size() const
    {
	return mC->vsize();
    }
    
    inline SizeT DPosRef::val() const
    {
	return mC->vval();
    }
    
    inline DPosRef DPosRef::next() const
    {
	return DPosRef(mC->vnext());
    } 

    template <class PosT1>
    inline DPos DPosRef::operator+(const CPosInterface<PosT1>& a) const
    {
	return DPos(mC->vplus(VPosRef<PosT1>(&a))); // check memory safety!!!
    }
    
    template <class PosT1>
    inline DPos DPosRef::operator*(const CPosInterface<PosT1>& a) const
    {
	return DPos(mC->vtimes(VPosRef<PosT1>(&a))); // check memory safety!!!
    }    
    
    template <class PosT1>
    inline DPos DPosRef::extend(const CPosInterface<PosT1>& a) const
    {
	return DPos(mC->vextend(a));
    }


    /************
     *   MPos   *
     ************/

    template <class PosT1, class PosT2>
    constexpr MPos<PosT1,PosT2>::MPos(const CPosInterface<PosT1>& first,
				      const CPosInterface<PosT2>& next) :
	mFirst(first), mNext(next)
    {}

    template <class PosT1, class PosT2>
    constexpr SizeT MPos<PosT1,PosT2>::size() const
    {
	return mFirst.size() + mNext.size();
    }

    template <class PosT1, class PosT2>
    constexpr auto MPos<PosT1,PosT2>::val() const
    {
	return mFirst.val();
    }

    template <class PosT1, class PosT2>
    constexpr const PosT1& MPos<PosT1,PosT2>::first() const
    {
	return mFirst;
    }

    template <class PosT1, class PosT2>
    constexpr const PosT2& MPos<PosT1,PosT2>::next() const
    {
	return mNext;
    }

    template <class PosT1, class PosT2>
    template <class PosT3, class PosT4>
    constexpr auto MPos<PosT1,PosT2>::operator+(const MPos<PosT3,PosT4>& a) const
    {
	typedef decltype(first()+a.first()) PosT5;
	typedef decltype(next()+a.next()) PosT6;
	return MPos<PosT5,PosT6>(first()+a.first(), next()+a.next());
    }

    template <class PosT1, class PosT2>
    template <class PosT3>
    constexpr auto MPos<PosT1,PosT2>::operator*(const CPosInterface<PosT3>& a) const
    {
	typedef decltype(first()*a) PosT5;
	typedef decltype(next()*a) PosT6;
	return MPos<PosT5,PosT6>(first()*a, next()*a);
    }

    template <class PosT1, class PosT2>
    template <class PosT3>
    constexpr auto MPos<PosT1,PosT2>::extend(const CPosInterface<PosT3>& p) const
    {
	return MPos<MPos<PosT1,PosT2>,PosT3>(*this,p);
    }

}

#endif
