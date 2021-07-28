
#ifndef __exttype_h__
#define __exttype_h__

#include <array>

namespace CNORXZInternal
{
    
    struct None;
    
    class ExtBase
    {
    public:
	ExtBase() = default;
	ExtBase(const ExtBase& in) = default;
	ExtBase(ExtBase&& in) = default;
	ExtBase& operator=(const ExtBase& in) = default;
	ExtBase& operator=(ExtBase&& in) = default;
        
	virtual size_t size() const = 0;
	virtual const size_t& val() const = 0;
	//virtual size_t rootSteps() const = 0;
	virtual bool operator==(const ExtBase& in) const = 0;
	virtual bool operator==(size_t in) const = 0;
	    
	virtual std::shared_ptr<ExtBase> operator+(const ExtBase& in) const = 0;
	virtual std::shared_ptr<ExtBase> operator*(size_t in) const = 0;
	virtual void zero() = 0;

	virtual std::shared_ptr<ExtBase> deepCopy() const = 0;
	
	template <class ExtType>
	const ExtType& expl() const;

        virtual std::string stype() const = 0;
    };

    typedef std::shared_ptr<ExtBase> DExt;

    template <class ExtType>
    class ExtT : public ExtBase
    {
    private:
	ExtType mExt;
    public:
	static constexpr size_t SIZE = ExtType::SIZE;
	static constexpr size_t NUM = ExtType::NUM;
	
	ExtT() = default;
	ExtT(const ExtT& in) = default;
	ExtT(ExtT&& in) = default;
	ExtT& operator=(const ExtT& in) = default;
	ExtT& operator=(ExtT&& in) = default;

	ExtT(const ExtType& in) : mExt(in) {}

	virtual std::shared_ptr<ExtBase> deepCopy() const override final { return std::make_shared<ExtT<ExtType>>(mExt); }

	virtual size_t size() const override final { return sizeof(ExtType)/sizeof(size_t); }
	//virtual size_t size() const override final { return ExtType::MExtSize(); }
	//virtual size_t rootSteps() const override final;
	const ExtType& ext() const { return mExt; }
	virtual const size_t& val() const override final { return mExt.val(); }
	virtual void zero() override final { mExt.zero(); }

	virtual bool operator==(const ExtBase& in) const override final
	{ return mExt == dynamic_cast<const ExtT<ExtType>&>(in).mExt; }
	
	virtual bool operator==(size_t in) const override final
	{ return mExt == in; }

	virtual DExt operator+(const ExtBase& in) const override final
	{ return std::make_shared<ExtT<ExtType>>( mExt + dynamic_cast<const ExtT<ExtType>&>(in).mExt ); }
	virtual DExt operator*(size_t in) const override final
	{ return std::make_shared<ExtT<ExtType>>( mExt * in ); }

        virtual std::string stype() const override final { return std::string("T[") + mExt.stype() + "]"; }
    };
    //class DExtT;

    template <class ExtType>
    DExt mkDExt(const ExtT<ExtType>& in)
    {
        return std::make_shared<ExtT<ExtType>>(in);
    }

    template <class ExtType>
    ExtT<ExtType> mkExtT(const ExtType& in)
    {
        return ExtT<ExtType>(in);
    }
    
    template <class X>
    class MExt
    {
    private:

	size_t mExt = 0u;
	X mNext;
	
    public:
	
	static constexpr size_t NUM = X::SIZE;
        static constexpr size_t SIZE = NUM + 1;

	MExt() = default;
	MExt(const MExt& in) = default;
	MExt& operator=(const MExt& in) = default;
	MExt(MExt&& in) = default;
	MExt& operator=(MExt&& in) = default;

	inline MExt(size_t ext, X next);

	template <class Z>
	inline MExt(size_t y, const Z& z);
	
	template <class Y, class Z>
	inline MExt(const Y& y, const Z& z);

	template <size_t N>
	inline MExt(const std::array<size_t,N>& arr);

        template <class Y>
        inline MExt(const MExt<Y>& y);

	inline void zero();
	
	inline const size_t& val() const;
	inline const X& next() const;

        template <size_t N>
        inline auto nn() const;

	inline bool operator==(const MExt& in) const
	{
	    return mExt == in.mExt and mNext == in.mNext;
	}

	inline bool operator==(size_t in) const
	{
	    return mExt == in and mNext == in;
	}

	inline MExt operator+(const MExt& in) const;
	inline MExt operator+(const None& in) const;
	inline MExt operator*(size_t in) const;

	template <class Y>
	auto extend(const Y& y) const -> MExt<decltype(mNext.extend(y))>
	{ return MExt<decltype(mNext.extend(y))>(mExt, mNext.extend(y)); }

        std::string stype() const { return std::string("E[") + mNext.stype() + "]"; }
    };    

    struct None
    {
        size_t VAL = 0;
        None() = default;
        None(const None& in) = default;
        None(None&& in) = default;
        None& operator=(const None& in) = default;
        None& operator=(None&& in) = default;
        
        template <class Y>
        None(const Y& y) {}
        
        static constexpr size_t SIZE = 0;

	inline bool operator==(const None& in) const
	{
	    return true;
	}

	inline bool operator==(size_t in) const
	{
	    return true; // CHECK!!!
	}

        inline None operator+(const None& in) const { return None(); }
	inline None operator*(size_t in) const { return None(); }

	inline const size_t& val() const { assert(0); return VAL; }
	inline void zero() {}
	
        template <class Y>
        Y extend(const Y& y) const
        { return y; }

        std::string stype() const { return std::string("N"); }
    };
    
    template <>
    class MExt<None>
    {
    private:

	size_t mExt = 0u;
	
    public:

	static constexpr size_t NUM = 0;
        static constexpr size_t SIZE = NUM + 1;
        
	MExt() = default;
	MExt(const MExt& in) = default;
	MExt& operator=(const MExt& in) = default;
	MExt(MExt&& in) = default;
	MExt& operator=(MExt&& in) = default;
	
	inline MExt(size_t ext);

        inline MExt(size_t y, const None& z) : mExt(y) {}

	template <class Z>
	inline MExt(size_t y, const Z& z);

	template <class Y, class Z>
	inline MExt(const Y& y, const Z& z);

	template <size_t N>
	inline MExt(const std::array<size_t,N>& arr);

        template <class Y>
        inline MExt(const MExt<Y>& y);

	inline bool operator==(const MExt& in) const
	{
	    return mExt == in.mExt;
	}

	inline bool operator==(size_t in) const
	{
	    return mExt == in;
	}

	inline const size_t& val() const;
	inline None next() const { return None(); }

	inline void zero();
	
        template <size_t N>
        inline auto nn() const;
        
	inline MExt operator+(const MExt& in) const;
	inline MExt operator+(const None& in) const;
	inline MExt operator*(size_t in) const;

	template <class Y>
	auto extend(const Y& y) const -> MExt<Y>
	{ return MExt<Y>(mExt,y); }

        std::string stype() const { return std::string("E[N]"); }

    };    

    template <class X>
    class DExtTX
    {
    private:
	mutable DExt mDExt;
	X mNext;

        template <class Y>
        friend class DExtTX;
    public:
	static constexpr size_t NUM = X::SIZE;
	static constexpr size_t SIZE = NUM + 1;
	
        DExtTX() { mDExt = std::make_shared<ExtT<None>>(); }
        DExtTX(const DExtTX& in) : mNext(in.mNext)
	{ mDExt = in.mDExt->deepCopy(); }
        DExtTX(DExtTX&& in) : mNext(in.mNext)
	{ mDExt = in.mDExt->deepCopy(); }
        DExtTX& operator=(const DExtTX& in) { mNext = in.mNext; mDExt = in.mDExt->deepCopy(); return *this; }
        DExtTX& operator=(DExtTX&& in) { mNext = in.mNext; mDExt = in.mDExt->deepCopy(); return *this; }
        explicit DExtTX(const DExt& in) : mDExt(in) {}
        /*
	template <class Y>
	DExtTX& operator=(const Y& y) { mDExt = std::make_shared<ExtT<Y>>(y); return *this; }

	template <class Y>
	DExtTX(const Y& y) : mDExt(std::make_shared<ExtT<Y>>(y)) {}
        */
	bool operator==(const DExtTX& in) const
	{ return *mDExt == *in.mDExt and mNext == in.mNext; }
	
	bool operator==(size_t in) const
	{ return *mDExt == in and mNext == in; }

        template <class Y>
        DExtTX(const DExtTX<Y>& in) : mDExt(in.mDExt), mNext(in.mNext) {}
        
	DExtTX(const DExt& y, const X& x) : mDExt(y->deepCopy()),
                                            mNext(x) {}

        virtual size_t size() const { return mDExt->size(); }
        inline const DExt& get() const { return mDExt; }

        inline DExtTX<None> reduce() const { return DExtTX<None>(mDExt,None(0)); }
        
        inline DExtTX operator+(const DExtTX& in) const
        { if (not mDExt) return in; else return DExtTX( (*mDExt) + (*in.mDExt), mNext + in.mNext ); }
        inline DExtTX operator*(size_t in) const
        { if (not mDExt) return *this; else return DExtTX((*mDExt) * in, mNext * in); }

	template <class ExtType>
	inline const ExtType& expl() const
	{ if(mDExt == nullptr) mDExt = std::make_shared<ExtT<ExtType>>(); assert(mDExt != nullptr); return mDExt->expl<ExtType>(); }

	template <class Y>
	inline auto extend(const Y& y) const -> DExtTX<decltype(mNext.extend(y))>
	{ return DExtTX<decltype(mNext.extend(y))>(mDExt, mNext.extend(y)); }

	inline const size_t& val() const { return mDExt->val(); }
	inline const X& next() const { return mNext; }

	inline void zero() { mDExt->zero(); }
	
	template <size_t N>
	inline auto nn() const;

        std::string stype() const { return std::string("D[") + mDExt->stype() + "," + mNext.stype() + "]"; }
    };

    typedef DExtTX<None> DExtT;
    
    inline MExt<None> mkExt(size_t s) { return MExt<None>(s); }


    template <size_t I, class X>
    auto getX(const MExt<X>& et)
    {
	if constexpr(I == 0){
	    return et;
	}
	else {
	    return getX<I-1>(et.next());
	}
    }

    template <size_t I, class X>
    auto getX(const DExtTX<X>& et)
    {
	if constexpr(I == 0){
	    return et;
	}
	else {
	    return getX<I-1>(et.next());
	}
    }

    template <size_t I>
    auto getX(const None& et)
    {
	static_assert(I == 0);
	return et;
    }

    template <size_t I, class X>
    size_t get(const MExt<X>& et)
    {
	return getX<I>(et).get();
    }

    template <size_t I, class X>
    size_t get(const DExtTX<X>& et)
    {
	return getX<I>(et).get();
    }

    template <size_t I>
    size_t get(const None& et)
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
    inline MExt<X>::MExt(size_t ext, X next) : mExt(ext), mNext(next) {}

    template <class X>
    template <size_t N>
    inline MExt<X>::MExt(const std::array<size_t,N>& arr) :
	mExt(std::get<NUM>(arr)), mNext(arr) {}

    template <class X>
    template <class Z>
    inline MExt<X>::MExt(size_t y, const Z& z) :
	mExt(z.val()), mNext(z.val(), z.next()) {}
    
    template <class X>
    template <class Y, class Z>
    inline MExt<X>::MExt(const Y& y, const Z& z) :
	mExt(y.val()), mNext(y.next(), z) {}

    template <class X>
    template <class Y>
    inline MExt<X>::MExt(const MExt<Y>& y) :
        mExt(y.val()), mNext(y.next()) {}

    template <class X>
    inline const size_t& MExt<X>::val() const
    {
	return mExt;
    }

    template <class X>
    inline const X& MExt<X>::next() const
    {
	return mNext;
    }

    template <class X>
    inline void MExt<X>::zero()
    {
	mExt = 0u;
	mNext.zero();
    }

    template <class X>
    template <size_t N>
    inline auto MExt<X>::nn() const
    {
	return getX<N>(*this);
    }

    template <class X>
    inline MExt<X> MExt<X>::operator+(const MExt<X>& in) const
    {
	return MExt<X>(mExt + in.val(), mNext + in.next());
    }

    template <class X>
    inline MExt<X> MExt<X>::operator+(const None& in) const
    {
	return *this;
    }

    template <class X>
    inline MExt<X> MExt<X>::operator*(size_t in) const
    {
	return MExt<X>(mExt * in, mNext * in);
    }

    
    //template <>
    inline MExt<None>::MExt(size_t ext) : mExt(ext) {}


    //template <>
    template <class Z>
    inline MExt<None>::MExt(size_t y, const Z& z) :
	mExt(z.val()) {}
    
    //template <>
    template <class Y, class Z>
    inline MExt<None>::MExt(const Y& y, const Z& z) :
	mExt(y.val()) {}


    //template <>
    template <size_t N>
    inline MExt<None>::MExt(const std::array<size_t,N>& arr) :
	mExt(std::get<NUM>(arr)) {}

    template <class Y>
    inline MExt<None>::MExt(const MExt<Y>& y) :
        mExt(y.val()) {}

    //template <>
    inline void MExt<None>::zero()
    {
	mExt = 0u;
    }

    template <size_t N>
    inline auto MExt<None>::nn() const
    {
	return getX<N>(*this);
    }

    inline const size_t& MExt<None>::val() const
    {
	return mExt;
    }

    //template <>
    inline MExt<None> MExt<None>::operator+(const MExt<None>& in) const
    {
	return MExt<None>(mExt + in.val());
    }

    inline MExt<None> MExt<None>::operator+(const None& in) const
    {
	return *this;
    }

    //template <>
    inline MExt<None> MExt<None>::operator*(size_t in) const
    {
	return MExt<None>(mExt * in);
    }

    template <class X>
    template <size_t N>
    inline auto DExtTX<X>::nn() const
    {
	return getX<N>(*this);
    }

} // end namespace CNORXZInternal


#endif
