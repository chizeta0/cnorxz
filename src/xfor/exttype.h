
#ifndef __exttype_h__
#define __exttype_h__

#include <array>

namespace XFOR
{

    template <class X>
    class MExt
    {
    private:

	size_t mExt = 0u;
	X mNext;
	
    public:
	
	static constexpr size_t NUM = X::NUM + 1;

	MExt() = default;
	MExt(const MExt& in) = default;
	MExt& operator=(const MExt& in) = default;
	MExt(MExt&& in) = default;
	MExt& operator=(MExt&& in) = default;
	
	inline MExt(size_t ext, X next);
	
	template <size_t N>
	inline MExt(const std::array<size_t,N>& arr);

	inline size_t val() const;
	inline const X& next() const;
	
	inline MExt operator+(const MExt& in) const;
	inline MExt operator*(size_t in) const;
	
    };    

    template <>
    class MExt<void>
    {
    private:

	size_t mExt = 0u;
	
    public:
	
	static constexpr size_t NUM = 0;

	MExt() = default;
	MExt(const MExt& in) = default;
	MExt& operator=(const MExt& in) = default;
	MExt(MExt&& in) = default;
	MExt& operator=(MExt&& in) = default;
	
	inline MExt(size_t ext);
	
	template <size_t N>
	inline MExt(const std::array<size_t,N>& arr);

	inline size_t val() const;
	
	inline MExt operator+(const MExt& in) const;
	inline MExt operator*(size_t in) const;

    };    


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
	    -> decltype(Getter<I-1>::get(et.next()))
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
	    -> decltype(et.next())
	{
	    return et.next();
	}	
    };

} // end namespace XFOR

/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */

namespace XFOR
{

    template <class X>
    inline MExt<X>::MExt(size_t ext, X next) : mExt(ext), mNext(next) {}

    template <class X>
    template <size_t N>
    inline MExt<X>::MExt(const std::array<size_t,N>& arr) :
	mExt(std::get<NUM>(arr)), mNext(arr) {}

    template <class X>
    inline size_t MExt<X>::val() const
    {
	return mExt;
    }

    template <class X>
    inline const X& MExt<X>::next() const
    {
	return mNext;
    }
    
    template <class X>
    inline MExt<X> MExt<X>::operator+(const MExt<X>& in) const
    {
	return MExt<X>(mExt + in.val(), mNext + in.next());
    }

    template <class X>
    inline MExt<X> MExt<X>::operator*(size_t in) const
    {
	return MExt<X>(mExt * in, mNext * in);
    }

    
    //template <>
    inline MExt<void>::MExt(size_t ext) : mExt(ext) {}
    

    //template <>
    template <size_t N>
    inline MExt<void>::MExt(const std::array<size_t,N>& arr) :
	mExt(std::get<NUM>(arr)) {}

    //template <>
    inline size_t MExt<void>::val() const
    {
	return mExt;
    }
    
    //template <>
    inline MExt<void> MExt<void>::operator+(const MExt<void>& in) const
    {
	return MExt<void>(mExt + in.val());
    }

    //template <>
    inline MExt<void> MExt<void>::operator*(size_t in) const
    {
	return MExt<void>(mExt * in);
    }


} // end namespace XFOR


#endif
