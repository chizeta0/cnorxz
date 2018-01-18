
#ifndef __for_params_h__
#define __for_params_h__

namespace MultiArrayHelper
{

    template <typename P>
    class ForParam
    {
    private:
	size_t mVal;
	P mNext;

    public:
	ForParam(const ForParam& last, const ForParam& ext, size_t pos);
	inline const P& next() const;
	inline const size_t& val() const;
	
    };

    template <>
    class ForParam<void>
    {
    private:
	size_t mVal;

    public:
	ForParam(const ForParam& last, const ForParam& ext, size_t pos);
	inline const size_t& val() const;
    };

    template <typename P>
    ForParam<P>::ForParam(const ForParam& last, const ForParam& ext, size_t pos) :
	mVal(last.val() + pos * ext.val()), mNext(last.next(), ext.next(), pos) {}

    template <>
    ForParam<void>::ForParam(const ForParam& last, const ForParam& ext, size_t pos) :
	mVal(last.val() + pos * ext.val()) {}

    template <typename P>
    inline const P& ForParam<P>::next() const
    {
	return mNext;
    }

    template <typename P>
    inline const size_t& ForParam<P>::val() const
    {
	return mVal;
    }

    template <>
    inline const size_t& ForParam<void>::val() const
    {
	return mVal;
    }

    
} // end namespace MultiArrayHelper


#endif
