
#include "mutli_range.h"

namespace MultiArrayTools
{
    /*******************
     *  MultiIndexBase *	     
     *******************/

    namespace
    {
	template <class MultiIndex, size_t N>
	IndefinitIndexBase& getIndex(MultiIndex& in, size_t n)
	{
	    if(n == N){
		return in.getIndex<N>();
	    }
	    else {
		return getIndex<N-1>(in, n);
	    }
	}
	
	template <class MultiIndex>
	IndefinitIndexBase& getIndex<MultiIndex,0>(MultiIndex& in, size_t n)
	{
	    return in.getIndex<0>();
	}
		
	template <size_t N, class MultiIndex>
	size_t evaluate_x(const MultiIndex& index)
	{
	    const auto& subIndex = index.getIndex<N>();
	    return evaluate_x<N-1>(index) * subIndex.size() + subIndex.pos();
	}

	template <class MultiIndex>
	size_t evaluate_x<0>(const MultiIndex& index)
	{
	    const auto& subIndex = index.getIndex<0>();
	    return subIndex.pos();
	}

	template <class MultiIndex>
	inline void plus(MultiIndex& index, size_t digit, int num)
	{
	    IndefinitIndexBase& si = index.getIndex(digit);
	    si.setPos( si.pos() + num );
	    size_t oor = si.outOfRange();
	    if(oor and digit != MultiIndex::mult - 1){
		plus(index, digit + 1, 1);
		plus(index, digit, oor - max());
	    }
	}
    }

    
    template <class... Indices>
    MultiIndex& MultiIndex<Indices...>::operator++()
    {
	setPos( pos() + 1 );
	plus(*this, 0, 1);
	return *this;
    }

    template <class... Indices>
    MultiIndex& MultiIndex<Indices...>::operator--()
    {
	setPos( pos() - 1 );
	plus(*this, 0, -1);
	return *this;
    }

    template <class... Indices>
    MultiIndex& MultiIndex<Indices...>::operator+=(int n)
    {
	setPos( pos() + n );
	plus(*this, 0, n);
	return *this;
    }

    template <class... Indices>
    MultiIndex& MultiIndex<Indices...>::operator-=(int n)
    {
	setPos( pos() - n );
	plus(*this, 0, 0-n);
	return *this;
    }
    
    template <class... Indices>
    size_t MultiIndex<Indices...>::evaluate(const MultiIndex<Indices...>& in) const
    {
	return evaluate_x<sizeof...(Indices)-1>(in);
    }

    template <class... Indices>
    size_t MultiIndex<Indices...>::dim() const
    {
	size_t res = 1;
	for(size_t i = 0; i != sMult; ++i){
	    res *= getIndex(i).dim();
	}
	return res;
    }
    
    template <class... Indices>
    bool MultiIndex<Indices...>::link(IndefinitIndexBase* toLink)
    {
	if(toLink->rangeType() != rangeType() and toLink->name() == name()){
	    // throw !!
	}
	
	if(toLink->rangeType() == rangeType() and toLink->name() == name()){
	    if(mLinked == toLink){
		return true; // dont link twice the same
	    }
	    else if(mLinked == nullptr){
		mLinked = toLink;
		return true;
	    }
	    else {
		return mLinked->link(toLink);
	    }
	}
	else {
	    return linkLower(toLink);
	}
    }

    template <size_t N>
    auto& MultiIndex<Indices...>::getIndex() -> decltype(std::get<N>(mIPack))
    {
	return std::get<N>(mIPack);
    }
    
    template <size_t N>
    const auto& MultiIndex<Indices...>::getIndex() const -> decltype(std::get<N>(mIPack));
    {
	return std::get<N>(mIPack);
    }
    
    template <class... Indices>
    IndefinitIndexBase& MultiIndex<Indices...>::getIndex(size_t n)
    {
	if(n >= sMult){
	    // throw !!
	}
	MultiIndex<Indices...>* t = this;
	return getIndex<sizeof...(Indices)>(*t, n);
    }

    template <class... Indices>
    const IndefinitIndexBase& MultiIndex<Indices...>::getIndex(size_t n) const
    {
	if(n >= sMult){
	    // throw !!
	}
	MultiIndex<Indices...>* t = this;
	return getIndex<sizeof...(Indices)>(*t, n);
    }

    template <class... Indices>
    bool MultiIndex<Indices...>::linkLower(IndefinitIndexBase* toLink)
    {
	bool res = false;
	for(size_t i = 0; i != sMult; ++i){
	    res |= getIndex(i).link(toLink);
	}
	return res;
    }

    template <class... Indices>
    void MultiIndex<Indices...>::linkTo(IndefinitIndexBase* target)
    {
	target->link(this);
	for(size_t i = 0; i != sMult; ++i){
	    getIndex(i).linkTo(target);
	}
    }

    /******************
     *   MultiRange   *
     ******************/
    
    template <size_t N>
    auto MultiRange<Ranges...>::get() -> decltype( std::get<N>(mSpace) )
    {
	return std::get<N>(mSpace);
    }
    
    
}
