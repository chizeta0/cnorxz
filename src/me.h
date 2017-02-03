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
	typedef Index IndexType;
	
	virtual size_t size() const = 0;
	virtual Index begin() = 0;
	virtual Index end() = 0;
	virtual RangeBase* base() = 0;
	virtual bool isSubRange() const;
    protected:
 	mutable std::string mName;
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
    public:
	const U& get(size_t pos) const;
	size_t get(const U& metaPos) const;
	
    protected:
	std::vector<U> mSpace;
    };
    
    template <class... Ranges>
    class MultiRange : public RangeBase<MultiIndex<typename Ranges::indexType...> >
    {
    public:

	static size_t dim = sizeof...(Ranges);
		
    protected:
	std::tuple<Ranges...> mSpace;
    };

    /******************
     *   Index        *
     ******************/

    template <Index>
    class IndexBase
    {
    public:
	virtual Index& operator=(const Index& in);
	
	virtual Index& operator=(size_t pos);
	virtual Index& operator++();
	virtual Index& operator--();
	virtual Index& operator+=(int n);
	virtual Index& operator-=(int n);
	
	virtual bool operator==(const IndexBase& i);
	virtual bool operator!=(const IndexBase& i);
	
	virtual size_t dim() const = 0;
	virtual size_t pos() const; // = mPos; implement !!!
	
    protected:
	// translate index into position
	virtual size_t evaluate(const Index& in) const = 0;
	
	size_t mPos;
	RangeBase<Index>* mRange;
    };
    
    template <typename U, RangeType TYPE>
    class SingleIndex : public IndexBase<SingleIndex<U,TYPE> >
    {
    public:

	virtual size_t size() const override;
	virtual SingleIndexBase& operator=(size_t pos) override;
	virtual SingleIndexBase& operator=(const U& upos);
	virtual const U& getMetaPos() const;

	// = 1
	virtual size_t dim() const override; // implement !!!
    protected:
	virtual size_t evaluate(const Index& in) const override;
    };

    template <class... Indices>template <class... Indices>
    class MultiIndex : public IndexBase<MultiIndex<Indices...> >
    {
    public:

	typedef std::tuple<Indices...> IndexPack;
	
	template <size_t N>
	auto getIndex() -> decltype(std::get<N>(mIPack));

	// dimension of MultiRange
	virtual size_t dim() const override; // implement !!!
	
    protected:
	virtual size_t evaluate(const MultiIndex& in) const override;
	
	IndexPack mIPack;
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

	template <typename... Strings>
	MultiArrayOperation<T,Range>& operator()(const Strings&... str) const;
	
	T& operator()(const typename Range::indexType& i); // implement
	const T& operator()(const typename Range::indexType& i) const; // implement
	
    private:
	bool mInit = false;
	std::vector<T> mCont;
	std::shared_ptr<Range> mRange;
    };

    template <typename T, class Range>
    class MultiArrayOperation
    {
    public:

	template <class Range2>
	MultiArrayOperation<T,Range2> operation(/*some operation*/) const;
	
	template <class Range2, class Range3>
	MultiArrayOperation<T,Range3> operation(const MultiArrayOperation<T,Range2>& in, /*some operation*/) const;

	// execute operation
	template <typename... Strings>
	MultiArray<T,Range> operator()(const Strings&... str);
    };


    // =========
    // Code that should finally work:

    MultiArray<double,/*Tensor*/,/*Tensor*/,/*3DSpace*/,/*3DSpace*/> ma1;
    MultiArray<double,/*Tensor*/,/*Tensor*/,/*3DSpace*/,/*3DSpace*/> ma2;
    MultiArray<double,/*Tensor*/,/*Tensor*/,/*Tensor*/,/*3DSpace*/,/*3DSpace*/,/*3DSpace*/> ma3;
    
    ma3 = ( ma1("mu","nu","x","y") * ma2("nu","lambda","y","z") )("mu","nu","lambda","x","y","z");
    
    

} // end namespace ME

#include "me.cc"

#endif
