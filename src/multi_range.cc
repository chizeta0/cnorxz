
#include "multi_range.h"

namespace MultiArrayTools
{
    /*******************
     *  MultiIndexBase *	     
     *******************/

    namespace
    {
	template <size_t N>
	struct IndexGetter
	{	
	    template <class MultiIndex>
	    static IndefinitIndexBase& getIndex(MultiIndex& in, size_t n)
	    {
		if(n == N){
		    return in.getIndex<N>();
		}
		else {
		    return IndexGetter<N-1>::getIndex(in, n);
		}
	    }

	    template <class MultiIndex>
	    static const IndefinitIndexBase& getIndex(const MultiIndex& in, size_t n)
	    {
		if(n == N){
		    return in.getIndex<N>();
		}
		else {
		    return IndexGetter<N-1>::getIndex(in, n);
		}
	    }
	};

	template <>
	struct IndexGetter<0>
	{
	    template <class MultiIndex>
	    static IndefinitIndexBase& getIndex(MultiIndex& in, size_t n)
	    {
		return in.getIndex<0>();
	    }

	    template <class MultiIndex>
	    static const IndefinitIndexBase& getIndex(const MultiIndex& in, size_t n)
	    {
		return in.getIndex<0>();
	    }
	};

	template <size_t N>
	struct Evaluation
	{
	    template <class MultiIndex>
	    static size_t evaluate(const MultiIndex& index)
	    {
		const auto& subIndex = index.template getIndex<N>();
		return Evaluation<N-1>::evaluate(index) * subIndex.max() + subIndex.pos();
	    }
	};

	template <>
	struct Evaluation<0>
	{
	    template <class MultiIndex>
	    static size_t evaluate(const MultiIndex& index)
	    {
		const auto& subIndex = index.template getIndex<0>();
		return subIndex.pos();
	    }
	};

	template <class MultiIndex>
	inline void plus(MultiIndex& index, size_t digit, int num)
	{
	    IndefinitIndexBase& si = index.get(digit);
	    si.setPos( si.pos() + num );
	    size_t oor = si.outOfRange();
	    if(oor and digit != 0){
		plus(index, digit - 1, 1);
		plus(index, digit, -si.max());
	    }
	}

	template <size_t N>
	struct TupleNamer
	{
	    template <class IndexPack, class Name>
	    static void nameTuple(IndexPack& iPack, Name& name)
	    {
		std::get<N>(iPack).name(name.get(N));
		TupleNamer<N-1>::nameTuple(iPack, name);
	    }
	};
	
	template <>
	struct TupleNamer<0>
	{
	    template <class IndexPack, class Name>
	    static void nameTuple(IndexPack& iPack, Name& name)
	    {
		std::get<0>(iPack).name(name.get(0));
	    }
	};

	template <size_t N>
	struct IndexSubOrder
	{
	    template <class IndexPack>
	    static void subOrd(IndexPack& iPack, IndefinitIndexBase* major)
	    {
		std::get<N>(iPack).subOrd(major);
		IndexSubOrder<N-1>::subOrd(iPack, major);
	    }
	};
	
	template <>
	struct IndexSubOrder<0>
	{
	    template <class IndexPack>
	    static void subOrd(IndexPack& iPack, IndefinitIndexBase* major)
	    {
		std::get<0>(iPack).subOrd(major);
	    }
	};
    }

    template <class... Indices>
    MultiIndex<Indices...>::MultiIndex(RangeBase<MultiIndex<Indices...> > const* range,
				       Indices&&... inds) : IndexBase<MultiIndex<Indices...> >(range),
							    mIPack(std::make_tuple(inds...))
    {
	IndexSubOrder<sizeof...(Indices)-1>::subOrd(mIPack, this);
	IIB::mPos = evaluate(*this);
    }

    template <class... Indices>
    MultiIndex<Indices...>::MultiIndex(RangeBase<MultiIndex<Indices...> > const* range,
				       const IndexPack& ipack) : IndexBase<MultiIndex<Indices...> >(range),
								 mIPack(ipack)
    {
	IndexSubOrder<sizeof...(Indices)-1>::subOrd(mIPack, this);
	IIB::mPos = evaluate(*this);
    }

    template <class... Indices>
    MultiIndex<Indices...>::~MultiIndex()
    {
	IndexSubOrder<sizeof...(Indices)-1>::subOrd(mIPack, nullptr);
    }
    
    template <class... Indices>
    MultiIndex<Indices...>& MultiIndex<Indices...>::operator++()
    {
	plus(*this, sizeof...(Indices)-1, 1);
	IIB::setPos( evaluate(*this) );
	return *this;
    }

    template <class... Indices>
    MultiIndex<Indices...>& MultiIndex<Indices...>::operator--()
    {
	IIB::setPos( IIB::pos() - 1 );
	plus(*this, 0, -1);
	return *this;
    }

    template <class... Indices>
    MultiIndex<Indices...>& MultiIndex<Indices...>::operator+=(int n)
    {
	plus(*this, sizeof...(Indices)-1, 1);
	IIB::setPos( evaluate(*this) );
	return *this;
    }

    template <class... Indices>
    MultiIndex<Indices...>& MultiIndex<Indices...>::operator-=(int n)
    {
	IIB::setPos( IIB::pos() - n );
	plus(*this, 0, 0-n);
	return *this;
    }

    template <class... Indices>
    bool MultiIndex<Indices...>::operator==(const MultiIndex<Indices...>& in)
    {
	return IB::mRange == in.mRange and IIB::pos() == in.pos();
    }

    template <class... Indices>
    bool MultiIndex<Indices...>::operator!=(const MultiIndex<Indices...>& in)
    {
	return IB::mRange != in.mRange or IIB::pos() != in.pos();
    }
    
    template <class... Indices>
    IndefinitIndexBase& MultiIndex<Indices...>::operator=(size_t pos)
    {
	IIB::setPos( pos );
	//setIndexPack(mIPack, pos); // -> implement !!!
	return *this;
    }

    template <class... Indices>
    MultiRangeType MultiIndex<Indices...>::rangeType() const
    {
	return IB::mRange->type();
    }
    
    template <class... Indices>
    size_t MultiIndex<Indices...>::evaluate(const MultiIndex<Indices...>& in) const
    {
	size_t res = Evaluation<sizeof...(Indices)-1>::evaluate(in);
	return res;
    }

    template <class... Indices>
    void MultiIndex<Indices...>::name(const Name& nm)
    {
	IIB::mName = nm.own();
	if(nm.size() >= sizeof...(Indices)){
	    TupleNamer<sizeof...(Indices)-1>::nameTuple(mIPack, nm);
	}
	else {
	    Name nm2 = nm;
	    nm2.autoName(sizeof...(Indices));
	    TupleNamer<sizeof...(Indices)-1>::nameTuple(mIPack, nm2);
	}
    }

    template <class... Indices>
    MultiIndex<Indices...>& MultiIndex<Indices...>::operator()(Indices&&... inds)
    {
	mIPack = std::make_tuple(inds...);
	IndexSubOrder<sizeof...(Indices)-1>::subOrd(mIPack, this);
	IIB::mPos = evaluate(*this);
	return *this;
    }

    template <class... Indices>
    MultiIndex<Indices...>& MultiIndex<Indices...>::operator()(const Indices&... inds)
    {
	mIPack = std::make_tuple(Indices(inds)...);
	IndexSubOrder<sizeof...(Indices)-1>::subOrd(mIPack, this);
	IIB::mPos = evaluate(*this);
	return *this;
    }
    
    template <class... Indices>
    size_t MultiIndex<Indices...>::dim() const
    {
	size_t res = 1;
	for(size_t i = 0; i != sizeof...(Indices); ++i){
	    res *= get(i).dim();
	}
	return res;
    }
    
    template <class... Indices>
    bool MultiIndex<Indices...>::link(IndefinitIndexBase* toLink)
    {
	if(toLink->rangeType() != rangeType() and
	   toLink->name() == IIB::name() and
	   not (IIB::name() == "master")){
	    // throw !!
	    assert(0);
	}
	if(toLink->rangeType() == rangeType() and toLink->name() == IIB::name()){
	    if(IIB::mLinked == toLink or IIB::mSoftLinked == toLink){
		return true; // dont link twice the same
	    }
	    else if(IIB::mLinked == nullptr and IIB::mSoftLinked == nullptr){
		IIB::mLinked = toLink;
		return true;
	    }
	    else {
		if(IIB::mLinked == nullptr){
		    return IIB::mSoftLinked->link(toLink);
		}
		else {
		    return IIB::mLinked->link(toLink);
		}
	    }
	}
	else {
	    if(linkLower(toLink)){
		IIB::mSoftLinked = IIB::mLinked;
		IIB::mLinked = nullptr;
		return true;
	    }
	    else {
		return false;
	    }
	}
    }

    template <class... Indices>
    template <size_t N>
    typename std::tuple_element<N, std::tuple<Indices...> >::type& MultiIndex<Indices...>::getIndex()
    {
	return std::get<N>(mIPack);
    }

    template <class... Indices>
    template <size_t N>
    typename std::tuple_element<N, std::tuple<Indices...> >::type const& MultiIndex<Indices...>::getIndex() const
    {
	return std::get<N>(mIPack);
    }
    
    template <class... Indices>
    IndefinitIndexBase& MultiIndex<Indices...>::get(size_t n)
    {
	if(n >= sizeof...(Indices)){
	    // throw !!
	}
	MultiIndex<Indices...>* t = this;
	return IndexGetter<sizeof...(Indices)-1>::getIndex(*t, n);
    }

    template <class... Indices>
    const IndefinitIndexBase& MultiIndex<Indices...>::get(size_t n) const
    {
	if(n >= sizeof...(Indices)){
	    // throw !!
	}
	MultiIndex<Indices...> const* t = this;
	return IndexGetter<sizeof...(Indices)-1>::getIndex(*t, n);
    }

    template <class... Indices>
    bool MultiIndex<Indices...>::linkLower(IndefinitIndexBase* toLink)
    {
	bool res = false;
	for(size_t i = 0; i != sizeof...(Indices); ++i){
	    res |= get(i).link(toLink);
	}
	return res;
    }

    template <class... Indices>
    void MultiIndex<Indices...>::linkTo(IndefinitIndexBase* target)
    {
	target->link(this);
	for(size_t i = 0; i != sizeof...(Indices); ++i){
	    get(i).linkTo(target);
	}
    }
    /*
    template <size_t N>
    struct RangeAssigner
    {
	template <class... Indices>
	static void assignRange(const RangeBase<MultiIndex<Indices...> >& multiRange,
				MultiIndex<Indices...>& multiIndex)
	{
	    multiIndex.template getIndex<N>().assignRange(nullptr &multiRange.template getRange<N>());
	    RangeAssigner<N-1>::assignRange(multiRange, multiIndex);
	}
    };

    template <>
    struct RangeAssigner<0>
    {
	template <class... Indices>
	static void assignRange(const RangeBase<MultiIndex<Indices...> >& multiRange,
				MultiIndex<Indices...>& multiIndex)
	{
	    multiIndex.template getIndex<0>().assignRange(nullptr &multiRange.template getRange<0>());
	}
    };    
    
    template <class... Indices>
    void MultiIndex<Indices...>::assignRange(RangeBase<MultiIndex<Indices...> > const* range)
    {
	if(IB::toNull()){
	    IB::mRange = range;
	}
	MultiIndex<Indices...>& thisRef = *this;
	RangeAssigner<sizeof...(Indices)-1>::assignRange(*range, thisRef);
    }*/

   
    /******************
     *   MultiRange   *
     ******************/

    template <class... Ranges>
    MultiRange<Ranges...>::MultiRange(const Ranges&... rs) : mSpace(std::make_tuple(rs...)) {}
    
    template <class... Ranges>
    template <size_t N>
    auto MultiRange<Ranges...>::getRange() -> decltype( std::get<N>(MultiRange<Ranges...>::SpaceType()) )
    {
	return std::get<N>(mSpace);
    }

    template <class... Ranges>
    template <size_t N>
    auto MultiRange<Ranges...>::getRange() const -> decltype( std::get<N>(MultiRange<Ranges...>::SpaceType()) )
    {
	return std::get<N>(mSpace);
    }

    template <size_t N>
    struct TupleSize
    {
	template <class RangeTuple>
	static size_t getSize(const RangeTuple& rt)
	{
	    return std::get<N>(rt).size() * TupleSize<N-1>::getSize(rt);
	}
    };

    template <>
    struct TupleSize<0>
    {
	template <class RangeTuple>
	static size_t getSize(const RangeTuple& rt)
	{
	    return std::get<0>(rt).size();
	}
    };

    template <class... Ranges>
    size_t MultiRange<Ranges...>::size() const
    {
	return TupleSize<sizeof...(Ranges)-1>::getSize(mSpace);
    }

    template <size_t N>
    struct IndexSetter
    {
	template <class... Ranges>
	static void setBegin(std::tuple<typename Ranges::IndexType...>& i, const std::tuple<Ranges...>& r)
	{
	    std::get<N>(i) = std::get<N>(r).begin();
	    IndexSetter<N-1>::setBegin(i,r);
	}

	template <class... Ranges>
	static void setEnd(std::tuple<typename Ranges::IndexType...>& i, const std::tuple<Ranges...>& r)
	{
	    std::get<N>(i) = std::get<N>(r).end();
	    std::get<N>(i) -= 1;
	    IndexSetter<N-1>::setEnd(i,r);
	}
    };

    template <>
    struct IndexSetter<0>
    {
	template <class... Ranges>
	static void setBegin(std::tuple<typename Ranges::IndexType...>& i, const std::tuple<Ranges...>& r)
	{
	    std::get<0>(i) = std::get<0>(r).begin();
	}

	template <class... Ranges>
	static void setEnd(std::tuple<typename Ranges::IndexType...>& i, const std::tuple<Ranges...>& r)
	{
	    std::get<0>(i) = std::get<0>(r).end();
	    std::get<0>(i) -= 1;
	}
    };

    template <size_t N>
    struct RangeVecBuilder
    {
	template <class... Ranges>
	static void buildRangeVec(std::vector<MultiRangeType>& rvec, const std::tuple<Ranges...>& rs)
	{
	    rvec.push_back(std::get<sizeof...(Ranges)-N-1>(rs).type());
	    RangeVecBuilder<N-1>::buildRangeVec(rvec, rs);
	}
    };
    template <>
    struct RangeVecBuilder<0>
    {
	template <class... Ranges>
	static void buildRangeVec(std::vector<MultiRangeType>& rvec, const std::tuple<Ranges...>& rs)
	{
	    rvec.push_back(std::get<sizeof...(Ranges)-1>(rs).type());
	}
    };
    
    template <class... Ranges>
    MultiRangeType MultiRange<Ranges...>::type() const
    {
	std::vector<MultiRangeType> rvec;
	RangeVecBuilder<sizeof...(Ranges)-1>::buildRangeVec(rvec, mSpace);
	return MultiRangeType(rvec);
    }
    
    template <class... Ranges>
    MultiIndex<typename Ranges::IndexType...> MultiRange<Ranges...>::begin() const
    {
	std::tuple<typename Ranges::IndexType...> is;
	IndexSetter<sizeof...(Ranges)-1>::setBegin(is,mSpace);
	return MultiIndex<typename Ranges::IndexType...>(this, is);
    }

    template <class... Ranges>
    MultiIndex<typename Ranges::IndexType...> MultiRange<Ranges...>::end() const
    {
	std::tuple<typename Ranges::IndexType...> is;
	IndexSetter<sizeof...(Ranges)-1>::setEnd(is,mSpace);
	return ++MultiIndex<typename Ranges::IndexType...>(this, is);
    }
}
