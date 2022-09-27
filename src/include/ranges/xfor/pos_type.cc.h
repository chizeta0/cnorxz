
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
    
    inline SizeT UPos::size() const
    {
	return 1;
    }
    
    inline const SizeT& UPos::val() const
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

    template <class PosT1, class PosT2, class PosT3>
    inline UPos& UPos::set(const CPosInterface<PosT1>& a, const CPosInterface<PosT2>& b,
			   const CPosInterface<PosT3>& c)
    {
	(*this) = a + b*c;
	return *this;
    }

    /************
     *   FPos   *
     ************/

    inline FPos::FPos(SizeT ext, const SizeT* map) : mExt(ext), mMap(map) {}
    
    constexpr SizeT FPos::size() const
    {
	return 1;
    }
    
    inline const SizeT& FPos::val() const
    {
	return mExt;
    }
	
    template <class PosT1>
    inline UPos FPos::operator+(const CPosInterface<PosT1>& a) const
    {
	return UPos(mExt + a.val());
    }

    inline UPos FPos::operator*(const CPosInterface<PosT1>& a) const
    {
	return UPos(mExt * mMap[a.val()]);
    }

    template <class PosT1>
    inline auto FPos::extend(const CPosInterface<PosT1>& a) const
    {
	return MPos<FPos,PosT>(*this,a);
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
	static constexpr Array<sizeof...(Ms)> ms({ Ms... });
	return SPos<N*std::get<a.val()>(ms)>;
    }

    template <SizeT N, SizeT... Ms>
    constexpr auto SFPos<N,Ms...>::operator+(const UPos& a) const
    {
	return UPos(N + a.val());
    }
    
    template <SizeT N, SizeT... Ms>
    constexpr auto SFPos<N,Ms...>::operator*(const UPos& a) const
    {
	static constexpr Array<sizeof...(Ms)> ms({ Ms... });
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
	ObjHandle<VPosBase>(std::forward<Uptr<VPosBase>>(a)),
	mNextRef(mC->next())
    {}

    template <class PosT>
    inline DPos::DPos(const PosT& a) :
	ObjHandle<VPosBase>( std::make_unique<PosT>(a) )
    {}

    inline SizeT DPos::size() const
    {
	return mC->vsize();
    }
    
    inline const SizeT& DPos::val() const
    {
	return mC->vval();
    }
    
    inline const DPosRef& DPos::next() const
    {
	return mNextRef;
    } 

    template <class PosT2>
    inline auto DPos::operator+(const MPos<PosT2>& a) const
    {
	return MPos<PosT2>(val()+a.val(), next()+a.next());
    }

    inline auto DPos::operator+(const UPos& a) const
    {
	return UPos(mC->val() + a.val());
    }

    inline auto DPos::operator+(const DPos& a) const
    {
	return DPos((*mC) + (*a));
    }

    inline auto DPos::operator+(const DPosRef& a) const
    {
	return DPos((*mC) + (*a));
    }

    inline auto DPos::operator*(SizeT a) const
    {
	return DPos((*mC)*a);
    }

    template <class PosT1>
    inline DPos DPos::extend(const PosT1& a) const
    {
	Uptr<VPosBase> out = mC->vextend()
	
    }

    template <class PosT1, class PosT2>
    inline PosInterface& DPos::set(const CPosInterface<PosT1>& a, const CPosInterface<PosT2>& b, sizeT c)
    {
	mC->setVal( a.val() + b.val()*c ) ;
	mC->vnext()->set(a.next(),b.next(),c);
	return *this;
    }

    /***************
     *   DPosRef   *
     ***************/
    
    inline DPosRef::DPosRef(const VPosBase* c) :
	mC(c), mNextRef(mC->vnext())
    {}
    
    inline const T& DPosRef::operator*() const
    {
	return *mC;
    }
    
    inline const T* DPosRef::operator->() const
    {
	return mC;
    }
	
    inline sizeT DPosRef::size() const
    {
	return mC->vsize();
    }
    
    inline const SizeT& DPosRef::val() const
    {
	return mC->vsize();
    }
    
    inline const DPosRef& DPosRef::next() const
    {
	return mNextRef;
    } 

    template <class PosT2>
    inline auto DPosRef::operator+(const MPos<PosT2>& a) const
    {
	return MPos<PosT2>(val()+a.val(), next()+a.next());
    }

    inline auto DPosRef::operator+(const UPos& a) const
    {
	return UPos(val()+a.val());
    }

    inline auto DPosRef::operator+(const DPos& a) const
    {
	return DPos((*mC)+(*a));
    }

    inline auto DPosRef::operator+(const DPosRef& a) const
    {
	return DPos((*mC)+(*a));
    }

    inline auto DPosRef::operator*(SizeT a) const
    {
	return DPos((*mC)*a);
    }

    /************
     *   MPos   *
     ************/

    template <class PosT>
    inline MPos<PosT>::MPos(SizeT ext, PosT next) : mExt(ext), mNext(next) {}

    template <class PosT>
    template <class PosT1>
    inline MPos<PosT>::MPos(SizeT y, const PosT1& z) :
	mExt(z.val()), mNext(z.val(), z.next()) {}
    
    template <class PosT>
    template <class PosT1, class PosT2>
    inline MPos<PosT>::MPos(const PosT1& y, const PosT2& z) :
	mExt(y.val()), mNext(y.next(), z) {}

    template <class PosT>
    template <class PosT1>
    inline MPos<PosT>::MPos(const MPos<PosT1>& y) :
        mExt(y.val()), mNext(y.next()) {}

    template <class PosT>
    inline const SizeT& MPos<PosT>::val() const
    {
	return mExt;
    }

    template <class PosT>
    inline const PosT& MPos<PosT>::next() const
    {
	return mNext;
    }

    template <class PosT>
    inline MPos<PosT> MPos<PosT>::operator+(const MPos<PosT>& in) const
    {
	return MPos<PosT>(mExt + in.val(), mNext + in.next());
    }

    template <class PosT>
    inline MPos<PosT> MPos<PosT>::operator+(const UPos& in) const
    {
	return MPos<PosT>(mExt + in.val(), mNext);
    }

    template <class PosT>
    inline MPos<PosT> MPos<PosT>::operator*(SizeT in) const
    {
	return MPos<PosT>(mExt * in, mNext * in);
    }

    template <class PosT>
    template <class PosT1>
    inline auto MPos<PosT>::extend(const PosInterface<PosT1>& y) const
    { return MPos<decltype(mNext.extend(y))>(mExt, mNext.extend(y)); }

    template <class PosT>
    template <class PosT1, class PosT2>
    inline PosInterface& MPos<PosT>::set(const CPosInterface<PosT1>& a,
					 const CPosInterface<PosT2>& b, sizeT c)
    { return *this = a + b*c; }
    //{ mExt = a.val() + b.val()*c ; mNext.set(a.next(),b.next(),c); return *this; }


}

#endif
