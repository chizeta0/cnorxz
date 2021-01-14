
#ifndef __exttype_h__
#define __exttype_h__

#include <array>

namespace MultiArrayHelper
{

    template <size_t I>
    struct Getter
    {
	template <class ExtType>
	static inline size_t get(const ExtType& et)
	{
	    return Getter<I-1>::get(et.next());
	}

	template <class ExtType>
	static inline auto getX(const ExtType& et)
	    -> decltype(Getter<I-1>::getX(et.next()))
	{
	    return Getter<I-1>::getX(et.next());
	}	
    };

    template <>
    struct Getter<0>
    {
	template <class ExtType>
	static inline size_t get(const ExtType& et)
	{
	    return et.get();
	}

	template <class ExtType>
	static inline auto getX(const ExtType& et)
	    -> ExtType
	{
	    return et;
	}	
    };

    struct None;
    
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
        inline auto nn() const
            -> decltype(Getter<N>::getX(*this))
        { return Getter<N>::getX(*this); }

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
        inline auto nn() const
            -> decltype(Getter<N>::getX(*this))
        { return Getter<N>::getX(*this); }
        
	inline MExt operator+(const MExt& in) const;
	inline MExt operator+(const None& in) const;
	inline MExt operator*(size_t in) const;

	template <class Y>
	auto extend(const Y& y) const -> MExt<Y>
	{ return MExt<Y>(mExt,y); }

        std::string stype() const { return std::string("E[N]"); }

    };    



} // end namespace MultiArrayHelper

/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */

namespace MultiArrayHelper
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


} // end namespace MultiArrayHelper


#endif
