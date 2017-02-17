
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
		    return getIndex<N-1>(in, n);
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
	};

	template <size_t N>
	struct Evaluation
	{
	    template <class MultiIndex>
	    static size_t evaluate(const MultiIndex& index)
	    {
		const auto& subIndex = index.getIndex<N>(0);
		return Evaluation<N-1>::evaluate(index) * subIndex.size() + subIndex.pos();
	    }
	};

	template <>
	struct Evaluation<0>
	{
	    template <class MultiIndex>
	    static size_t evaluate(const MultiIndex& index)
	    {
		const auto& subIndex = index.getIndex<0>(0);
		return subIndex.pos();
	    }
	};

	template <class MultiIndex>
	inline void plus(MultiIndex& index, size_t digit, int num)
	{
	    IndefinitIndexBase& si = index.getIndex(digit);
	    si.setPos( si.pos() + num );
	    size_t oor = si.outOfRange();
	    if(oor and digit != MultiIndex::mult - 1){
		plus(index, digit + 1, 1);
		plus(index, digit, oor - si.max());
	    }
	}

	template <size_t N>
	struct TupleNamer
	{
	    template <class IndexPack, class Name>
	    static void nameTuple(IndexPack& iPack, Name& name)
	    {
		std::get<N>(iPack).name(name.get(N));
		nameTuple<N-1>(iPack, name);
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
    }

    template <class... Indices>
    MultiIndex<Indices...>::MultiIndex(Indices&&... inds) : mIPack(std::make_tuple(inds...)) {}
    
    template <class... Indices>
    MultiIndex<Indices...>& MultiIndex<Indices...>::operator++()
    {
	setPos( IIB::pos() + 1 );
	plus(*this, 0, 1);
	return *this;
    }

    template <class... Indices>
    MultiIndex<Indices...>& MultiIndex<Indices...>::operator--()
    {
	setPos( IIB::pos() - 1 );
	plus(*this, 0, -1);
	return *this;
    }

    template <class... Indices>
    MultiIndex<Indices...>& MultiIndex<Indices...>::operator+=(int n)
    {
	setPos( IIB::pos() + n );
	plus(*this, 0, n);
	return *this;
    }

    template <class... Indices>
    MultiIndex<Indices...>& MultiIndex<Indices...>::operator-=(int n)
    {
	setPos( IIB::pos() - n );
	plus(*this, 0, 0-n);
	return *this;
    }
    
    template <class... Indices>
    size_t MultiIndex<Indices...>::evaluate(const MultiIndex<Indices...>& in) const
    {
	return Evaluation<sizeof...(Indices)-1>::evaluate(in);
    }

    template <class... Indices>
    void MultiIndex<Indices...>::name(const Name& nm)
    {
	name(nm.own());
	if(nm.size() >= sizeof...(Indices)){
	    TupleNamer<sizeof...(Indices)-1>::nameTuple(mIPack, nm);
	}
	else {
	    Name nm2 = nm;
	    nm2.autoName(sizeof...(Indices));
	    TupleNamer<sizeof...(Indices)-1>::nameTuple(mIPack, nm);
	}
    }
    
    template <class... Indices>
    size_t MultiIndex<Indices...>::dim() const
    {
	size_t res = 1;
	for(size_t i = 0; i != sizeof...(Indices); ++i){
	    res *= getIndex(i).dim();
	}
	return res;
    }
    
    template <class... Indices>
    bool MultiIndex<Indices...>::link(IndefinitIndexBase* toLink)
    {
	if(toLink->rangeType() != IIB::rangeType() and toLink->name() == name()){
	    // throw !!
	}
	
	if(toLink->rangeType() == IIB::rangeType() and toLink->name() == name()){
	    if(IIB::mLinked == toLink){
		return true; // dont link twice the same
	    }
	    else if(IIB::mLinked == nullptr){
		IIB::mLinked = toLink;
		return true;
	    }
	    else {
		return IIB::mLinked->link(toLink);
	    }
	}
	else {
	    return linkLower(toLink);
	}
    }

    template <class... Indices>
    template <size_t N>
    auto MultiIndex<Indices...>::getIndex(size_t x) -> decltype(std::get<N>(MultiIndex<Indices...>::IndexPack()))
    {
	return std::get<N>(mIPack);
    }

    template <class... Indices>
    template <size_t N>
    auto MultiIndex<Indices...>::getIndex(size_t x) const ->
	decltype(std::get<N>(MultiIndex<Indices...>::IndexPack()))
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
	return IndexGetter<sizeof...(Indices)>::getIndex(*t, n);
    }

    template <class... Indices>
    const IndefinitIndexBase& MultiIndex<Indices...>::get(size_t n) const
    {
	if(n >= sizeof...(Indices)){
	    // throw !!
	}
	MultiIndex<Indices...>* t = this;
	return IndexGetter<sizeof...(Indices)>::getIndex(*t, n);
    }

    template <class... Indices>
    bool MultiIndex<Indices...>::linkLower(IndefinitIndexBase* toLink)
    {
	bool res = false;
	for(size_t i = 0; i != sizeof...(Indices); ++i){
	    res |= getIndex(i).link(toLink);
	}
	return res;
    }

    template <class... Indices>
    void MultiIndex<Indices...>::linkTo(IndefinitIndexBase* target)
    {
	target->link(this);
	for(size_t i = 0; i != sizeof...(Indices); ++i){
	    getIndex(i).linkTo(target);
	}
    }

    /******************
     *   MultiRange   *
     ******************/

    template <class... Ranges>
    template <size_t N>
    auto MultiRange<Ranges...>::get() -> decltype( std::get<N>(MultiRange<Ranges...>::SpaceType()) )
    {
	return std::get<N>(mSpace);
    }

    template <class... Ranges>
    template <size_t N>
    auto MultiRange<Ranges...>::get() const -> decltype( std::get<N>(MultiRange<Ranges...>::SpaceType()) )
    {
	return std::get<N>(mSpace);
    }

    template <class... Ranges>
    MultiIndex<typename Ranges::IndexType...> MultiRange<Ranges...>::begin() const
    {
	return MultiIndex<typename Ranges::IndexType...>(/*!!!!!!*/);
    }

    template <class... Ranges>
    MultiIndex<typename Ranges::IndexType...> MultiRange<Ranges...>::end() const
    {
	return MultiIndex<typename Ranges::IndexType...>(/*!!!!!!*/);
    }
}
