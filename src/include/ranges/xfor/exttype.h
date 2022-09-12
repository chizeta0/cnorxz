
#ifndef __cxz_exttype_h__
#define __cxz_exttype_h__

#include <array>

namespace CNORXZInternal
{

    // Dynamic Ext: ObjHandl<ExtBase>
    // In For Expr: try to resolve until some upper limit
    
    class VPosBase
    {
    public:
	DEFAULT_MEMBERS(VPosBase);
        
	virtual SizeT vsize() const = 0;
	virtual const SizeT& vval() const = 0;
	virtual VPosBase& vzero() = 0;
	virtual Uptr<VPosBase> vextend(const DPos& a) const = 0;
	virtual Uptr<VPosBase> vextend(const MPos<NPos>& a) const = 0;
    };

    template <class PosT>
    class PosInterface
    {
    public:
	DEFAULT_MEMBERS(PosInterface);

	PosT& THIS() { return static_cast<PosT&>(*this); }
	const PosT& THIS() const { return static_cast<const PosT&>(*this); }
	
	inline SizeT size() const { THIS().size(); }
	inline const SizeT& val() const { THIS().val(); }
	inline auto next() const { THIS().next(); }
	inline bool operator==(const PosInterface<PosT>& a) const { return val() == a.val() and next() == a.next(); }
	inline bool operator!=(const PosInterface<PosT>& a) const { return val() != a.val() or next() != a.next(); }
	inline bool operator==(SizeT a) const { return val() == a and next() == a; }
	inline bool operator!=(SizeT a) const { return val() != a or next() != a; }
	inline PosInterface<PosT> operator+(const PosInterface<PosT>& a) const { return PosT(val()+a.val(), next()+a.next()); }
	inline PosInterface<PosT> operator*(SizeT a) const { return PosT(val()*a, next()*a); }
	inline PosInterface<PosT>& zero() { return THIS().zero(); }

	template <class P>
	inline auto extend(const PosInterface<P>& a) const { return THIS().extend(a); }
    };
    
    class DPos : public ObjHandle<VPosBase>,
		 public PosInterface<DPos>
    {
    public:
	DEFAULT_MEMBERS(DPos);
	DPos(Uptr<VPosBase> a) : ObjHandle<VPosBase>(a) {}

	template <class PosT>
	DPos(const PosT& a) : ObjHandle<VPosBase>( std::make_unique<PosT>(a) ) {}
	
	inline SizeT size() const { return mC->vsize(); }
	inline const SizeT& val() const { return mC->vval(); }
	inline DPos& zero() { mC->vzero(); return *this; }

	template <class P>
	inline DPos extend(const PosInterface<P>& a) const
	{ /* append MPos<NPos> in static for loop over entries */ }
    };

    /*
    template <class PosT>
    class VPos : public VPosBase
    {
    private:
	PosT mExt;
    public:

	DEFAULT_MEMBERS(VPos);

	VPos(const PosT& in) : mExt(in) {}

	virtual SizeT size() const override final { return sizeof(PosT)/sizeof(size_t); }
	//const PosT& ext() const { return mExt; }
	virtual const SizeT& val() const override final { return mExt.val(); }
	virtual void zero() override final { mExt.zero(); }

	virtual bool operator==(const VPosBase& in) const override final
	{ return mExt == dynamic_cast<const VPos<PosT>&>(in).mExt; }
	
	virtual bool operator==(SizeT in) const override final
	{ return mExt == in; }

	virtual DExt operator+(const VPosBase& in) const override final
	{ return std::make_shared<VPos<PosT>>( mExt + dynamic_cast<const VPos<PosT>&>(in).mExt ); }
	virtual DExt operator*(size_t in) const override final
	{ return std::make_shared<VPos<PosT>>( mExt * in ); }

    };
    */
    template <class PosT>
    class VPos : public VPosBase, public PosT
    {
    public:
	
	DEFAULT_MEMBERS(VPos);
	
	VPos(const PosInterface<PosT>& a) : PosT(a.THIS()) {}
	
	virtual SizeT vsize() const override final { return THIS().size(); }
	virtual const SizeT& vval() const override final { return THIS().val(); }
	virtual VPos& vzero() override final { THIS().zero(); return *this; }
	virtual Uptr<VPosBase> vextend(const DPos& a) const override final
	{ return std::make_unique<VPosBase>( THIS().extend(a) ); }
	virtual Uptr<VPosBase> vextend(const MPos<NPos>& a) const override final
	{ return std::make_unique<VPosBase>( THIS().extend(a) ); } // ??? if that works it would be a miracle ???
	// .... probably I need to define a static instanciation limit...

    };

    template <class PosT>
    DPos mkDExt(const VPos<PosT>& in)
    {
        return std::make_shared<VPos<PosT>>(in);
    }

    template <class PosT>
    VPos<PosT> mkVPos(const PosT& in)
    {
        return VPos<PosT>(in);
    }
    
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

	template <class Z>
	inline MPos(SizeT y, const Z& z);
	
	template <class Y, class Z>
	inline MPos(const Y& y, const Z& z);

	template <size_t N>
	inline MPos(const Arr<SizeT,N>& arr);

        template <class Y>
        inline MPos(const MPos<Y>& y);

	inline void zero();
	
	inline const SizeT& val() const;
	inline const PosT& next() const;

        template <size_t N>
        inline auto nn() const;

	inline bool operator==(const MPos& in) const
	{
	    return mExt == in.mExt and mNext == in.mNext;
	}

	inline bool operator==(size_t in) const
	{
	    return mExt == in and mNext == in;
	}

	inline MPos operator+(const MPos& in) const;
	inline MPos operator+(const NPos& in) const;
	inline MPos operator*(size_t in) const;

	template <class Y>
	auto extend(const Y& y) const -> MPos<decltype(mNext.extend(y))>
	{ return MPos<decltype(mNext.extend(y))>(mExt, mNext.extend(y)); }

    };    

    class NPos : public PosInterface<NPos>
    {
    private:
        SizeT VAL = 0;
    public:
	
	DEFAULT_MEMBERS(NPos);
	
        template <class Y>
        NPos(const Y& y) {}
        
        //static constexpr SizeT SIZE = 0;

	inline const SizeT& val() const { assert(0); return VAL; }
	inline bool operator==(const NPos& in) const { return true; }
	inline bool operator==(SizeT in) const { return true; }
        inline NPos operator+(const NPos& in) const { return NPos(); }
	inline NPos operator*(size_t in) const { return NPos(); }
	inline void zero() {}
	
        template <class Y>
        Y extend(const Y& y) const
        { return y; }

    };
    
    template <>
    class MPos<NPos> : public PosInterface<MPos<NPos>>
    {
    private:
	SizeT mExt = 0u;
	
    public:
	//static constexpr size_t NUM = 0;
        //static constexpr size_t SIZE = NUM + 1;
        
	DEFAULT_MEMBERS(MPos);
	
	inline MPos(size_t ext);

        inline MPos(size_t y, const NPos& z) : mExt(y) {}

	template <class Z>
	inline MPos(size_t y, const Z& z);

	template <class Y, class Z>
	inline MPos(const Y& y, const Z& z);

	template <size_t N>
	inline MPos(const std::array<size_t,N>& arr);

        template <class Y>
        inline MPos(const MPos<Y>& y);

	inline bool operator==(const MPos& in) const
	{
	    return mExt == in.mExt;
	}

	inline bool operator==(size_t in) const
	{
	    return mExt == in;
	}

	inline const size_t& val() const;
	inline NPos next() const { return NPos(); }

	inline void zero();
	
        template <size_t N>
        inline auto nn() const;
        
	inline MPos operator+(const MPos& in) const;
	inline MPos operator+(const NPos& in) const;
	inline MPos operator*(size_t in) const;

	template <class Y>
	auto extend(const Y& y) const -> MPos<Y>
	{ return MPos<Y>(mExt,y); }

    };    
    
    inline MPos<NPos> mkExt(SizeT s) { return MPos<NPos>(s); }


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

} // end namespace CNORXZInternal

/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */

namespace CNORXZInternal
{

    template <class X>
    inline MPos<X>::MPos(size_t ext, X next) : mExt(ext), mNext(next) {}

    template <class X>
    template <size_t N>
    inline MPos<X>::MPos(const std::array<size_t,N>& arr) :
	mExt(std::get<NUM>(arr)), mNext(arr) {}

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

    template <class X>
    template <size_t N>
    inline auto MPos<X>::nn() const
    {
	return getX<N>(*this);
    }

    template <class X>
    inline MPos<X> MPos<X>::operator+(const MPos<X>& in) const
    {
	return MPos<X>(mExt + in.val(), mNext + in.next());
    }

    template <class X>
    inline MPos<X> MPos<X>::operator+(const NPos& in) const
    {
	return *this;
    }

    template <class X>
    inline MPos<X> MPos<X>::operator*(size_t in) const
    {
	return MPos<X>(mExt * in, mNext * in);
    }

    
    //template <>
    inline MPos<NPos>::MPos(size_t ext) : mExt(ext) {}


    //template <>
    template <class Z>
    inline MPos<NPos>::MPos(size_t y, const Z& z) :
	mExt(z.val()) {}
    
    //template <>
    template <class Y, class Z>
    inline MPos<NPos>::MPos(const Y& y, const Z& z) :
	mExt(y.val()) {}


    //template <>
    template <size_t N>
    inline MPos<NPos>::MPos(const std::array<size_t,N>& arr) :
	mExt(std::get<NUM>(arr)) {}

    template <class Y>
    inline MPos<NPos>::MPos(const MPos<Y>& y) :
        mExt(y.val()) {}

    //template <>
    inline void MPos<NPos>::zero()
    {
	mExt = 0u;
    }

    template <size_t N>
    inline auto MPos<NPos>::nn() const
    {
	return getX<N>(*this);
    }

    inline const size_t& MPos<NPos>::val() const
    {
	return mExt;
    }

    //template <>
    inline MPos<NPos> MPos<NPos>::operator+(const MPos<NPos>& in) const
    {
	return MPos<NPos>(mExt + in.val());
    }

    inline MPos<NPos> MPos<NPos>::operator+(const NPos& in) const
    {
	return *this;
    }

    //template <>
    inline MPos<NPos> MPos<NPos>::operator*(size_t in) const
    {
	return MPos<NPos>(mExt * in);
    }

    template <class X>
    template <size_t N>
    inline auto DVPosX<X>::nn() const
    {
	return getX<N>(*this);
    }

} // end namespace CNORXZInternal


#endif
