// -*- C++ -*-

#ifndef __me_h__
#define __me_h__

#include <cstdlib>
#include <vector>
#include <string>
#include <map>

namespace ME
{

    template <class Tuple, size_t N>
    auto make_left_x(const Tuple& tp) -> decltype(std::tuple_cat(make_left<Tuple,N-1>(tp),
							       std::make_tuple(get<N>(tp))))
    {
	return std::tuple_cat(make_left<Tuple,N-1>(tp), std::make_tuple(get<N>(tp)));
    }

    template <class Tuple>
    auto make_left_x<Tuple, 0>(const Tuple& tp) -> decltype(std::make_tuple(get<N>(tp)))
    {
	return std::make_tuple(get<N>(tp));
    }

    template <class Tuple, size_t N>
    auto make_right_x(const Tuple& tp) -> decltype(std::tuple_cat(std::make_tuple(get<std::tuple_size(tp)-N>(tp)),
								make_left<Tuple,std::tuple_size(tp)-N+1>(tp)))
    {
	const size_t M = std::tuple_size(tp) - N;
	return std::tuple_cat(std::make_tuple(get<M>(tp)), make_left<Tuple,M+1>(tp));
    }

    template <class Tuple>
    auto make_right_x<Tuple, 0>(const Tuple& tp) -> decltype(std::make_tuple(get<std::tuple_size(tp)>(tp)))
    {
	const size_t M = std::tuple_size(tp);
	return std::make_tuple(get<M>(tp));
    }

    template <class Tuple, size_t N>
    auto make_left(const Tuple& tp) -> decltype(make_left_x<Tuple, N>(tp))
    {
	return make_left_x<Tuple, N>(tp);
    }

    template <class Tuple, size_t N>
    auto make_right(const Tuple& tp) -> decltype(make_right_x<Tuple, std::tuple_size(Tuple)-N>(tp))
    {
	return make_right_x<Tuple, std::tuple_size(Tuple)-N>(tp);
    }
    
    /***********************************************************/

    /******************
     *   Range        *
     ******************/
    
    enum class RangeType
    {
	SPACE,
	MOMENTUM,
	LORENTZ,
	SPIN
    };


    template <class Index>
    class RangeBase
    {
    public:
	static Index indexType;
	
	virtual size_t size() const = 0;
	virtual Index begin() = 0;
	virtual Index end() = 0;
	virtual RangeBase* base() = 0;
	virtual bool isSubRange() const;
    };

    template <class Index>
    class SubRangeBase : public RangeBase<Index>
    {
    public:
	virtual bool isSubRange() const override;
    protected:
	RangeBase* mBase;
	std::vector<bool> mOccupation;
    };

    template <typename U, RangeType TYPE>;
    class SingleIndex;

    template <class Rs...>
    class MultiIndex;
    
    template <typename U, RangeType TYPE>
    class SingleRange : public RangeBase<SingleIndex<U,TYPE> >
    {
    protected:
	std::vector<U> mSpace;
    };
    
    template <class... Ranges>
    class MultiRange : public RangeBase<MultiIndex<decltype(typename Ranges::indexType)...> >
    {
    public:

	static size_t dim = sizeof...(Ranges);

	template <class... Ranges2>
	auto combine(const MultiRange<Ranges2...>& in) -> MultiRange<Ranges... ,Ranges2...>;

	template <size_t N, size_t M>
	auto merge() -> /**/;
	
    protected:
	std::tuple<Ranges...> mSpace;
    };

    /******************
     *   Index        *
     ******************/
    
    class IndexBase
    {
    public:
	virtual size_t pos() const;
	virtual IndexBase& operator=(size_t pos);
	virtual IndexBase& operator++();
	//virtual IndexBase operator++(int);
	virtual IndexBase& operator--();
	//virtual IndexBase operator--(int);
	virtual IndexBase& operator+=(int n);
	//virtual IndexBase operator+(int n);
	virtual IndexBase& operator-=(int n);
	//virtual IndexBase operator-(int n);
	virtual bool operator==(const IndexBase& i);
	virtual bool operator!=(const IndexBase& i);
    protected:
	size_t mPos;
	RangeBase* mRange;
    };
    
    template <typename U, RangeType TYPE>
    class SingleIndex : public IndexBase
    {
    public:

	virtual size_t size() const override;
	virtual SingleIndexBase& operator=(size_t pos) override;
	virtual SingleIndexBase& operator=(const U& upos);
	virtual const U& getActPos() const;
    protected:
    };

    template <class... Is>
    class MultiIndex : public IndexBase
    {
    public:

	typedef std::tuple<Is...> ContType;
	
	virtual size_t size() const override;

	virtual MultiIndex& operator=(const U& upos);

	virtual const ContType& getActPos() const;
	
	// treat index N and N+1 as the same index
	template <size_t N>
	auto together() -> decltype(std::tuple_cat(make_left<ContType,N>(mCont),
						   make_right<ContType,N>(mCont)));
	
	// Inserts an index in 'mCont' at position 'N'
	template <class I, size_t N>
	auto add(const I& in) -> decltype(std::tuple_cat(make_left<ContType,N>(mCont), in,
							 make_right<ContType,N+1>(mCont)));
	
    protected:
	ContType mCont;
    };

    /******************
     *   MultiArray   *
     ******************/
    
    template <typename T, class Range>
    class MultiArray
    {
    public:

	MultiArray() = default;
	MultiArray(const MultiArray& in) = default;
	MultiArray& operator=(const MultiArray& in) = default;
	MultiArray(MultiArray&& in) = default;
	MultiArray& operator=(MultiArray&& in) = default;
	
	MultiArray(const Range& range); // !!!!

	template <class Range2, class Range3>
	MultiArray<T,Range3> operation(std::array<std::string,typename Range::dim>& d1,
				       std::array<std::string,typename Range2::dim>& d2,
				       const MultiArray<T,Range2>& in) const;
	
	T& operator()(const typename Range::indexType& i);
	const T& operator()(const typename Range::indexType& i) const;
	
    private:
	bool mInit = false;
	std::vector<T> mCont;
    };

    
    enum class NameTag
    {
	// to enumerate the ranges/indices when merging two MultiArrays, etc...
    };
	
    template <class Range1, class Range2, >

} // end namespace ME

#include "me.cc"

#endif
