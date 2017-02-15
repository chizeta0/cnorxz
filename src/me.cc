// -*- C++ -*-

#include "me.h"

namespace ME
{

    /*********************
     *  MultiRangeType   *
     *********************/

    MultiRangeType& MultiRangeType::operator=(RangeType& type)
    {
	setType(type);
	return *this;
    }
    
    MultiRangeType& MultiRangeType::operator=(const std::vector<MultiRangeType>& multiType)
    {
	setMultiType(multiType);
	return *this;
    }

    MultiRangeType& MultiRangeType::operator[](size_t num)
    {
	return mMultiType->at(num);
    }
    
    const MultiRangeType& MultiRangeType::operator[](size_t num) const
    {
	return mMultiType->at(num);
    }
    
    bool MultiRangeType::multi() const
    {
	return mType != nullptr;
    }

    bool MultiRangeType::operator==(const MultiRangeType& in) const
    {
	if(multi()){
	    return *mMultiType == *in.mMultiType;
	}
	else {
	    return mType == in.mType;
	}
    }
    
    bool MultiRangeType::operator!=(const MultiRangeType& in) const
    {
	if(multi()){
	    return *mMultiType != *in.mMultiType;
	}
	else {
	    return mType != in.mType;
	}
    }
    
    void MultiRangeType::setType(RangeType type)
    {
	mType = type;
	if(mMultiType != nullptr){
	    delete mMultiType;
	}
	mMultiType = nullptr;
    }
    
    void MultiRangeType::setMultiType(const std::vector<MultiRangeType>& multiType)
    {
	mMultiType = new std::vector<MultiRangeType>( multiType );
	mType = RangeType::NIL;
    }
    
    /******************
     *   RangeBase    *
     ******************/

    template <class Index>
    bool RangeBase<Index>::isSubRange() const
    {
	return false;
    }

    /*********************
     *   SubRangeBase    *
     *********************/

    template <class Index>
    bool SubRangeBase<Index>::isSubRange() const
    {
	return true;
    }

    /********************
     *   SingleRange    *
     ********************/

    template <typename U, RangeType TYPE>
    const U& SingleRange<U,TYPE>::get(size_t pos) const
    {
	return mSpace[pos];
    }

    template <typename U, RangeType TYPE>
    size_t SingleRange<U,TYPE>::get(const U& metaPos) const
    {
	size_t cnt = 0;
	for(auto& x: mSpace){
	    if(x == metaPos){
		return cnt;
	    }
	    ++cnt;
	}
	return cnt;
    }

    /************************
     *  IndefinitIndexBase  *
     ************************/

    bool IndefinitIndexBase::link(IndefinitIndexBase* toLink)
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
	    return false;
	}
    }

    void IndefinitIndexBase::freeLinked()
    {
	if(linked()){
	    mLinked->freeLinked();
	    mLinked = nullptr;
	}
    }

    bool IndefinitIndexBase::linked() const
    {
	return mLinked != nullptr;
    }

    void IndefinitIndexBase::setPos(size_t pos)
    {
	mPos = pos;
	if(linked()){
	    mLinked->setPos(pos);
	}
    }

    size_t IndefinitIndexBase::max() const
    {
	return static_cast<size_t>( -1 );
    }

    size_t IndefinitIndexBase::outOfRange() const
    {
	int res = pos() - max();
	return res > 0 ? static_cast<size_t>(res) : 0;
    }
    
    /**************
     *  IndexBase *	     
     **************/

    template <class Index>
    Index& IndexBase<Index>::operator=(const Index& in)
    {
	setPos( evaluate(in) );
    }

    template <class Index>
    Index& IndexBase<Index>::operator=(size_t pos)
    {
	setPos( pos );
	return *this;
    }

    template <class Index>
    Index& IndexBase<Index>::operator++()
    {
	setPos( ++mPos );
	return *this;
    }

    template <class Index>
    Index& IndexBase<Index>::operator--()
    {
	setPos( --mPos );
	return *this;
    }

    template <class Index>
    Index& IndexBase<Index>::operator+=(int n)
    {
	setPos( mPos += n );
	return *this;
    }

    template <class Index>
    Index& IndexBase<Index>::operator-=(int n)
    {
	setPos( mPos -= n );
	return *this;
    }

    template <class Index>
    bool IndexBase<Index>::operator==(const Index& i)
    {
	return mRange == i.mRange and mPos == i.mPos;
    }

    template <class Index>
    bool IndexBase<Index>::operator!=(const Index& i)
    {
	return mRange != i.mRange or mPos != i.mPos;
    }

    template <class Index>
    size_t IndexBase<Index>::max() const
    {
	return mRange->size();
    }

    
    /********************
     *  SingleIndexBase *	     
     ********************/
    
    template <typename U, IndexType TYPE>
    const U& SingleIndexBase<U,TYPE>::getMetaPos() const
    {
	return dynamic_cast<SingleRange*>( mRange )->get(mPos);
    }

    template <typename U, IndexType TYPE>
    size_t dim() const
    {
	return 1;
    }
    
    template <typename U, IndexType TYPE>
    size_t SingleIndexBase<TYPE>::evaluate(const Index& in)
    {
	return in.mPos;
    }

    template <typename U, IndexType TYPE>
    void linkTo(IndefinitIndexBase* target)
    {
	target->link(this);
    }    

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
	void plus(MultiIndex& index, size_t digit, int num)
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
    auto& getIndex() -> decltype(std::get<N>(mIPack))
    {
	return std::get<N>(mIPack);
    }
    
    template <size_t N>
    const auto& getIndex() const -> decltype(std::get<N>(mIPack));
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
    
    /*******************
     *  MultiArray     *	     
     *******************/

    template <typename... Ranges>
    void giveNames(const std::string& name, /**/);

    template <typename... Ranges>
    void giveNames(const std::vector<std::string>& names, /**/);

    /*!!!!  giveNames(...)  !!!!!*/
    
    template <typename T, class Range>
    T& MultiArray<T,Is...>::operator()(const typename Range::indexType& i)
    {
	return mCont[ i.pos() ];
    }

    template <typename T, class Range>
    const T& MultiArray<T,Is...>::operator()(const typename Range::indexType& i) const
    {
	return mCont[ i.pos() ];
    }

    template <typename T, class Range>
    template <class... NameTypes>
    MultiArrayOperation<T,Range>& operator()(const NameTypes&... str) const
    {
	auto index = mRange->begin();
	// give names... !!!
	return MultiArrayOperation<T,Range>(*this, index);
    }
    
    
    /*********************************
     *   MultiArrayOperationBase     *
     *********************************/

    template <typename T, class Range>
    template <class Range2>
    MultiArrayOperationBase<T,Range>&
    MultiArrayOperationBase<T,Range>::operator=(const MultiArrayOperationBase<T, Range2>& in)
    {
	// perform operation (iterate head index)
    }

    template <typename T, class Range>
    template <class Operation, class... Ranges>
    MultiArrayOperation<Operation>
    MultiArrayOperationBase<T,Range>::operator()(Operation& op, MultiArrayOperationBase<T,Ranges>&... secs)
    {
	// set operation
	// set mSecs
	// link Indices
    }
    
    template <typename T, class Range>
    size_t MultiArrayOperationBase<T,Range>::argNum() const
    {
	return 1;
    }

    /*****************************
     *   MultiArrayOperation     *
     *****************************/

    template <typename T, class Range, class Operation, class... Ranges>
    size_t MultiArrayOperation<T,Range,Operation,Ranges...>::argNum() const
    {
	return sizeof...(Ranges) + 1;
    }
    
} // end namespace ME

