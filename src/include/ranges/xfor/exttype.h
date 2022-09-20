
#ifndef __cxz_exttype_h__
#define __cxz_exttype_h__

#include "base/base.h"

namespace CNORXZ
{

    // Dynamic Ext: ObjHandl<ExtBase>
    // In For Expr: try to resolve until some upper limit
    
    class VPosBase
    {
    public:
	DEFAULT_MEMBERS(VPosBase);

	virtual Uptr<VPosBase> copy() const = 0;
	virtual SizeT vsize() const = 0;
	virtual const SizeT& vval() const = 0;
	virtual const VPosBase* vnext() const = 0;
	virtual Uptr<VPosBase> vplus(const VPosBase* a) const = 0;
	virtual Uptr<VPosBase> vtimes(SizeT a) const = 0;
	virtual Uptr<VPosBase> vextend(const DPos& a) const = 0;
	virtual Uptr<VPosBase> vextend(const UPos& a) const = 0;
	virtual Uptr<VPosBase> vextend(const FPos& a) const = 0;
    };

    template <class PosT>
    class CPosInterface
    {
    public:
	DEFAULT_MEMBERS(CPosInterface);

	PosT& THIS() { return static_cast<PosT&>(*this); }
	const PosT& THIS() const { return static_cast<const PosT&>(*this); }
	
	inline SizeT size() const { return THIS().size(); }
	inline const SizeT& val() const { return THIS().val(); }
	inline const auto& next() const { return THIS().next(); }
	inline bool operator==(const CPosInterface<PosT>& a) const { return val() == a.val() and next() == a.next(); }
	inline bool operator!=(const CPosInterface<PosT>& a) const { return val() != a.val() or next() != a.next(); }
	inline bool operator==(SizeT a) const { return val() == a and next() == a; }
	inline bool operator!=(SizeT a) const { return val() != a or next() != a; }
	inline CPosInterface<PosT> operator+(const CPosInterface<PosT>& a) const
	{ return THIS() + a.THIS(); }
	inline CPosInterface<PosT> operator*(SizeT a) const { return THIS() * a; }

	template <class P>
	inline auto extend(const CPosInterface<P>& a) const { return THIS().extend(a); }
    };

    template <class PosT>
    class PosInterface : public CPosInterface<PosT>
    {
    public:
	typedef CPosInterface<PosT> PI;
	
	DEFAULT_MEMBERS(PosInterface);

	template <class PosT1, class PosT2>
	inline PosInterface& set(const CPosInterface<PosT1>& a, const CPosInterface<PosT2>& b, sizeT c)
	{ return PI::THIS().set(a,b,c); }

	template <class PosT1, class PosT2>
	inline PosInterface& operator()(const CPosInterface<PosT1>& a, const CPosInterface<PosT2>& b, sizeT c)
	{ return set(a,b,c); }
    };
    
    template <class PosT>
    class VPos : public VPosBase, public PosT
    {
    private:
	VPosRef<decltype(THIS::next())> mNextRef;
    public:
	
	DEFAULT_MEMBERS(VPos);
	
	VPos(const PosInterface<PosT>& a) : PosT(a.THIS()), mNextRef(&PosT::THIS().next()) {}
	
	virtual Uptr<VPosBase> copy() const override final { return std::make_unique<VPos>(*this); }
	virtual SizeT vsize() const override final { return PosT::THIS().size(); }
	virtual const SizeT& vval() const override final { return PosT::THIS().val(); }
	virtual const VPosRef& vnext() const override final
	{ return mNextRef; }
	virtual Uptr<VPosBase> vextend(const DPos& a) const override final
	{ return std::make_unique<VPos<decltype(PosT::THIS().extend(a))>>
		( PosT::THIS().extend(a) ); }
	virtual Uptr<VPosBase> vextend(const UPos& a) const override final
	{ return std::make_unique<VPos<decltype(PosT::THIS().extend(a))>>
		( PosT::THIS().extend(a) ); } // ??? if that works it would be a miracle ???
	virtual Uptr<VPosBase> vextend(const FPos& a) const override final
	{ return std::make_unique<VPos<decltype(PosT::THIS().extend(a))>>
		( PosT::THIS().extend(a) ); } // ??? if that works it would be a miracle ???
	// .... probably I need to define a static instanciation limit...

    };

    template <class PosT>
    class VPosRef : public VPosBase
    {
    private:
	const PosT* mC;
    public:
	DEFAULT_MEMBERS(VPosRef);

	VPosRef(const PosT* c) : mC(c) {}
    };
    
    // defined as empty since they should never instanciated
    template <>
    class VPos<DPos>
    {};

    // defined as empty since they should never instanciated
    template <>
    class VPos<DPosRef>
    {};

    class DPosRef : public CPosInterface<DPosRef>
    {
    private:
	DPosRef mNextRef;
	const VPosBase* mC;
    public:
	DEFAULT_MEMBERS(DPosRef);
	DPosRef(const VPosBase* c) : mC(c), mNextRef(mC->vnext()) {}

	const T& operator*() const { return *mC; }
	const T* operator->() const { return mC; }
	
	inline sizeT size() const { return mC->vsize(); }
	inline const SizeT& val() const { return mC->vsize(); }
	inline const DPosRef& next() const { return mNextRef; } 

	template <class PosT2>
	inline auto operator+(const MPos<PosT2>& a) const
	{ return MPos<PosT2>(val()+a.val(), next()+a.next()); }

	inline auto operator+(const UPos& a) const
	{ return UPos(val()+a.val()); }

	inline auto operator+(const DPos& a) const
	{ return DPos((*mC)+(*a)); }

	inline auto operator+(const DPosRef& a) const
	{ return DPos((*mC)+(*a)); }

	inline auto operator*(SizeT a) const
	{ return DPos((*mC)*a); }
	
    };
    
    class DPos : public ObjHandle<VPosBase>,
		 public PosInterface<DPos>
    {
    private:
	DPosRef mNextRef;
    public:
	DEFAULT_MEMBERS(DPos);
	DPos(Uptr<VPosBase>&& a) :
	    ObjHandle<VPosBase>(std::forward<Uptr<VPosBase>>(a)),
	    mNextRef(mC->next())
	{}

	template <class PosT>
	DPos(const PosT& a) : ObjHandle<VPosBase>( std::make_unique<PosT>(a) ) {}

	inline SizeT size() const { return mC->vsize(); }
	inline const SizeT& val() const { return mC->vval(); }
	inline const DPosRef& next() const { return mNextRef; } 

	template <class PosT2>
	inline auto operator+(const MPos<PosT2>& a) const
	{ return MPos<PosT2>(val()+a.val(), next()+a.next()); }

	inline auto operator+(const UPos& a) const
	{ return UPos(mC->val() + a.val()); }

	inline auto operator+(const DPos& a) const
	{ return DPos((*mC) + (*a)); }

	inline auto operator+(const DPosRef& a) const
	{ return DPos((*mC) + (*a)); }

	inline auto operator*(SizeT a) const
	{ return DPos((*mC)*a); }

	template <class PosT1, class PosT2>
	inline PosInterface& set(const CPosInterface<PosT1>& a, const CPosInterface<PosT2>& b, sizeT c)
	{ mC->setVal( a.val() + b.val()*c ) ; mC->vnext()->set(a.next(),b.next(),c); return *this; }
    };
    
    template <class PosT>
    class MPos
    {
    private:

	SizeT mExt = 0u;
	PosT mNext;
	
    public:
	
	//static constexpr SizeT NUM = PosT::SIZE;
        //static constexpr SizeT SIZE = NUM + 1;

	DEFAULT_MEMBERS(MPos);
	
	inline MPos(SizeT ext, PosT next);

	template <class PosT1>
	inline MPos(SizeT y, const CPosInterface<PosT1>& z);
	
	template <class PosT1, class PosT2>
	inline MPos(const CPosInterface<PosT1>& y, const CPosInterface<PosT2>& z);

        template <class PosT1>
        inline MPos(const MPos<PosT1>& y);

	inline const SizeT& val() const;
	inline const PosT& next() const;

	inline auto operator+(const MPos& in) const;
	//inline MPos operator+(const UPos& in) const;
	inline auto operator*(SizeT in) const;

	template <class PosT1>
	inline auto extend(const PosInterface<PosT1>& y) const -> MPos<decltype(mNext.extend(y))>
	{ return MPos<decltype(mNext.extend(y))>(mExt, mNext.extend(y)); }

	template <class PosT1, class PosT2>
	inline PosInterface& set(const CPosInterface<PosT1>& a, const CPosInterface<PosT2>& b, sizeT c)
	{ return *this = a + b*c; }
	//{ mExt = a.val() + b.val()*c ; mNext.set(a.next(),b.next(),c); return *this; }
    };    

    template <class PosT>
    class FMPos : public PosInterface<FMPos<PosT>>
    {
    private:
	SizeT mExt = 0;
	const Vector<SizeT>* mMap;
	PosT mNext;

    public:

	DEFAULT_MEMBERS(FMPos);

	template <class PosT2>
	inline auto operator+(const PosT2& a) const { return MPos(mExt,mNext) + a; }

	inline auto operator*(SizeT a) const { return MPos(mExt*(*mMap)[a], mNext*a); }
    };
    
    class UPos : public PosInterface<UPos>
    {
    private:
        SizeT mExt = 0;
    public:
	
	DEFAULT_MEMBERS(NPos);
	
        template <class Y>
        NPos(const Y& y) {}
        
        //static constexpr SizeT SIZE = 0;

	inline const SizeT& val() const { return mExt; }
	inline SizeT size() const { return 1; }
	inline const SizeT& next() { static SizeT dummy = 0; return dummy; }
        inline UPos operator+(const UPos& in) const { return UPos(mExt + in.val()); }
	inline UPos operator*(size_t in) const { return UPos(mExt * in); }
	
        template <class Y>
        inline auto extend(const Y& y) const
        { return MPos<Y>(mExt, y); }

    };
    
    class FPos : public PosInterface<FPos>
    {
    private:
	SizeT mExt = 0;
	const Vector<SizeT>* mMap;

    public:

	DEFAULT_MEMBERS(FMPos);

	template <class PosT2>
	inline auto operator+(const PosT2& a) const { return UPos(mExt) + a; }

	inline auto operator*(SizeT a) const { return UPos(mExt*(*mMap)[a]); }
    };

    inline MPos<NPos> mkExt(SizeT s) { return MPos<NPos>(s); }

    /*
    template <size_t I, class X>
    auto getX(const MPos<X>& et)
    {
	if constexpr(I == 0){
	    return et;
	}
	else {
	    return getX<I-1>(et.next());
	}
    }

    template <size_t I, class X>
    auto getX(const DVPosX<X>& et)
    {
	if constexpr(I == 0){
	    return et;
	}
	else {
	    return getX<I-1>(et.next());
	}
    }

    template <size_t I>
    auto getX(const NPos& et)
    {
	static_assert(I == 0);
	return et;
    }

    template <size_t I, class X>
    size_t get(const MPos<X>& et)
    {
	return getX<I>(et).get();
    }

    template <size_t I, class X>
    size_t get(const DVPosX<X>& et)
    {
	return getX<I>(et).get();
    }

    template <size_t I>
    size_t get(const NPos& et)
    {
	return getX<I>(et).get();
    }
    */

} // end namespace CNORXZInternal

/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */

namespace CNORXZ
{

    template <class X>
    inline MPos<X>::MPos(size_t ext, X next) : mExt(ext), mNext(next) {}
    /*
    template <class X>
    template <size_t N>
    inline MPos<X>::MPos(const std::array<size_t,N>& arr) :
	mExt(std::get<NUM>(arr)), mNext(arr) {}
    */
    template <class X>
    template <class Z>
    inline MPos<X>::MPos(size_t y, const Z& z) :
	mExt(z.val()), mNext(z.val(), z.next()) {}
    
    template <class X>
    template <class Y, class Z>
    inline MPos<X>::MPos(const Y& y, const Z& z) :
	mExt(y.val()), mNext(y.next(), z) {}

    template <class X>
    template <class Y>
    inline MPos<X>::MPos(const MPos<Y>& y) :
        mExt(y.val()), mNext(y.next()) {}

    template <class X>
    inline const size_t& MPos<X>::val() const
    {
	return mExt;
    }

    template <class X>
    inline const X& MPos<X>::next() const
    {
	return mNext;
    }

    template <class X>
    inline void MPos<X>::zero()
    {
	mExt = 0u;
	mNext.zero();
    }
    /*
    template <class X>
    template <size_t N>
    inline auto MPos<X>::nn() const
    {
	return getX<N>(*this);
    }
    */
    template <class X>
    inline MPos<X> MPos<X>::operator+(const MPos<X>& in) const
    {
	return MPos<X>(mExt + in.val(), mNext + in.next());
    }

    template <class X>
    inline MPos<X> MPos<X>::operator+(const UPos& in) const
    {
	return MPos<X>(mExt + in.val(), mNext);
    }

    template <class X>
    inline MPos<X> MPos<X>::operator*(SizeT in) const
    {
	return MPos<X>(mExt * in, mNext * in);
    }

    
} // end namespace CNORXZInternal


#endif
