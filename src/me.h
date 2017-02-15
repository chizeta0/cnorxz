// -*- C++ -*-

#ifndef __me_h__
#define __me_h__

#include <cstdlib>
#include <vector>
#include <string>
#include <map>

#define DEFAULT_MEMBERS(__class_name__) __class_name__() = default; \
    __class_name__(const __class_name__& in) = default; \
    __class_name__& operator=(const __class_name__& in) = default; \
    __class_name__(__class_name__&& in) = default; \
    __class_name__& operator=(__class_name__&& in) = default

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
	NIL = 0,
	SPACE = 1,
	MOMENTUM = 2,
	LORENTZ = 3,
	SPIN = 4
    };

    class MultiRangeType
    {
    public:

	DEFAULT_MEMBERS(MultiRangeType);
	
	MultiRangeType& operator=(RangeType& type);
	MultiRangeType& operator=(const std::vector<MultiRangeType>& multiType);

	MultiRangeType& operator[](size_t num);
	const MultiRangeType& operator[](size_t num) const;
	
	bool multi() const;

	bool operator==(const MultiRangeType& in) const;
	bool operator!=(const MultiRangeType& in) const;
	
    private:
	void setType(RangeType type);
	void setMultiType(const std::vector<MultiRangeType>& multiType);
	
	RangeType mType;
	std::vector<MultiRangeType>* mMultiType;
    };
    
    template <class Index>
    class RangeBase
    {
    public:
	typedef Index IndexType;
	
	virtual size_t size() const = 0;
	virtual Index begin() = 0;
	virtual Index end() = 0;
	virtual RangeBase<Index>* base() = 0;
	virtual bool isSubRange() const;

    protected:
	DEFAULT_MEMBERS(RangeBase);
	
    };

    template <class Range>
    auto cross(const Range& r1, const Range& r2) -> /**/;
    
    template <class Range1, class Range2>
    auto cross(const Range1& r1, const Range2& r2) -> /**/;
    
    template <class Index>
    class SubRangeBase : public RangeBase<Index>
    {
    public:
	virtual bool isSubRange() const override;
    protected:
	DEFAULT_MEMBERS(SubRangeBase);
	RangeBase<Index>* mBase;
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
	DEFAULT_MEMBERS(SingleRange);
	
	const U& get(size_t pos) const;
	size_t get(const U& metaPos) const;
	
    protected:
	std::vector<U> mSpace;
    };
    
    template <class... Ranges>
    class MultiRange : public RangeBase<MultiIndex<typename Ranges::indexType...> >
    {
    public:

	DEFAULT_MEMBERS(MultiRange);
	static size_t dim = sizeof...(Ranges);

	template <size_t N>
	auto get() -> decltype( std::get<N>(mSpace) );
	
    protected:
	std::tuple<Ranges...> mSpace;
    };

    /******************
     *   Index        *
     ******************/

    class IndefinitIndexBase
    {
    public:
	virtual IndefinitIndexBase& operator=(const IndefinitIndexBase& in) = 0;
	
	virtual IndefinitIndexBase& operator=(size_t pos) = 0;
	virtual IndefinitIndexBase& operator++() = 0;
	virtual IndefinitIndexBase& operator--() = 0;
	virtual IndefinitIndexBase& operator+=(int n) = 0;
	virtual IndefinitIndexBase& operator-=(int n) = 0;
	
	virtual bool operator==(const IndefinitIndexBase& i) = 0;
	virtual bool operator!=(const IndefinitIndexBase& i) = 0;
	
	virtual size_t dim() const = 0;
	virtual size_t pos() const = 0; // = mPos; implement !!!

	std::string& name();
	const std::string& name() const;

	MultiRangeType rangeType() const = 0;

	virtual bool link(IndefinitIndexBase* toLink);  
	virtual void freeLinked();
	virtual bool linked() const;
	virtual void linkTo(IndefinitIndexBase* target) = 0; 
	
	virtual void setPos(size_t pos);

	virtual size_t max() const;
	virtual size_t outOfRange() const;
	
    protected:

	DEFAULT_MEMBERS(IndefinitIndexBase);
	
	std::string mName;
	size_t mPos;

	IndefinitIndexBase* mLinked;
    };
    
    template <class Index>
    class IndexBase : public IndefinitIndexBase
    {
    public:
	virtual Index& operator=(const Index& in) override;
	
	virtual Index& operator=(size_t pos) override;
	virtual Index& operator++() override;
	virtual Index& operator--() override;
	virtual Index& operator+=(int n) override;
	virtual Index& operator-=(int n) override;
	
	virtual bool operator==(const IndexBase& i) override;
	virtual bool operator!=(const IndexBase& i) override;
	
	virtual size_t pos() const override; // = mPos; implement !!!
	
    protected:

	DEFAULT_MEMBERS(IndexBase);
	
	// translate index into position
	virtual size_t evaluate(const Index& in) const = 0;
	RangeBase<Index>* mRange;
    };
    
    template <typename U, RangeType TYPE>
    class SingleIndex : public IndexBase<SingleIndex<U,TYPE> >
    {
    public:

	DEFAULT_MEMBERS(SingleIndex);

	virtual size_t size() const override;
	virtual SingleIndexBase& operator=(size_t pos) override;
	virtual SingleIndexBase& operator=(const U& upos);
	virtual const U& getMetaPos() const;

	virtual size_t dim() const override; // = 1

	virtual void linkTo(IndefinitIndexBase* target) override;
	
    protected:
	virtual size_t evaluate(const Index& in) const override;
    };

    template <class... Indices>
    class MultiIndex : public IndexBase<MultiIndex<Indices...> >
    {
    public:

	DEFAULT_MEMBERS(MultiIndex);
	
	typedef std::tuple<Indices...> IndexPack;
	static size_t sMult = sizeof...(Indices);
	
	virtual MultiIndex& operator++() override;
	virtual MultiIndex& operator--() override;
	virtual MultiIndex& operator+=(int n) override;
	virtual MultiIndex& operator-=(int n) override;
	
	template <size_t N>
	auto& getIndex() -> decltype(std::get<N>(mIPack));

	template <size_t N>
	const auto& getIndex() const -> decltype(std::get<N>(mIPack));

	IndefinitIndexBase& getIndex(size_t n);
	const IndefinitIndexBase& getIndex(size_t n) const;
	
	// dimension of MultiRange
	virtual size_t dim() const override; // implement !!!

	virtual bool link(IndefinitIndexBase* toLink) override;
	virtual void linkTo(IndefinitIndexBase* target) override;
	
    protected:
	
	virtual bool linkLower(IndefinitIndexBase* toLink);
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

	DEFAULT_MEMBERS(MultiArray);
	MultiArray(const Range& range); // !!!!
	
	template <class... NameTypes>
	MultiArrayOperation<T,Range>& operator()(const NameTypes&... str) const;
	
	T& operator()(const typename Range::indexType& i);
	const T& operator()(const typename Range::indexType& i) const;
	
    private:
	bool mInit = false;
	std::vector<T> mCont;
	std::shared_ptr<Range> mRange;
    };

    
    
    
    /***************************
     *   MultiArrayOperation   *
     ***************************/

    template <typename T, class Range>
    class MultiArrayOperationBase
    {
    public:
	
	MultiArrayOperation(MultiArray<T,Range>& ma, const IndefinitIndexBase& iib);
	
	// execute AnyOperation
	// exception if range types are inconsitent with names
	template <class Range2>
	MultiArrayOperationBase& operator=(const MultiArrayOperationBase<T, Range2>& in);
	
	
	template <class Operation, class... Ranges>
	MultiArrayOperation<Operation> operator()(Operation& op, MultiArrayOperationBase<T,Ranges>&... secs);
	
	template <class Range2>
	MultiArrayOperation<std::plus<T>,Range2> operator+(MultiArrayOperationBase<T,Range2>& sec);

	template <class Range2>
	MultiArrayOperation<std::minus<T>,Range2> operator-(MultiArrayOperationBase<T,Range2>& sec);

	template <class Range2>
	MultiArrayOperation<std::multiplies<T>,Range2> operator*(MultiArrayOperationBase<T,Range2>& sec);

	template <class Range2>
	MultiArrayOperation<std::divides<T>,Range2> operator/(MultiArrayOperationBase<T,Range2>& sec);

	virtual size_t argNum() const;
	
    protected:

	MultiArray<T,Range>& mArrayRef;
	IndefinitIndexBase* mIibPtr;
	
    };

    template <typename T, class Range, class Operation, class... Ranges>
    class MultiArrayOperation : public MultiArrayOperationBase<T,Range>
    {
    public:

	virtual size_t argNum() const override;
	
    protected:

	Operation mOp;
	std::tuple<MultiArrayOperationBase<T,Ranges>... > mSecs;
    };

    // =========
    // Code that should finally work:

    MultiArray<double,/*Tensor*/,/*Tensor*/,/*3DSpace*/,/*3DSpace*/> ma1;
    MultiArray<double,/*Tensor*/,/*Tensor*/,/*3DSpace*/,/*3DSpace*/> ma2;
    MultiArray<double,/*Tensor*/,/*Tensor*/,/*Tensor*/,/*3DSpace*/,/*3DSpace*/,/*3DSpace*/> ma3;

    ma3("mu","nu","lambda","x","y","z") = ma1("mu","nu","x","y") * ma2("nu","lambda","y","z");
    //                              operator=               operation()
    // ("mu","nu","lambda","x","y","z") <--- ("mu","nu","x","y","nu","lambda","y","z")
    

} // end namespace ME

#include "me.cc"

#endif
