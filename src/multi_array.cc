// -*- C++ -*-

#include "multi_array.h"

namespace MultiArrayTools
{
    /**************************************
     *  MultiArrayBase::const_iterator    *	     
     **************************************/

    template <typename T, class Range>
    MultiArrayBase<T,Range>::const_iterator::const_iterator(const MultiArrayBase<T,Range>& ma):
	mMAPtr(&ma), mIndex(mMAPtr->beginIndex())
    { }

    template <typename T, class Range>
    MultiArrayBase<T,Range>::const_iterator::const_iterator(const MultiArrayBase<T,Range>& ma,
							    const typename Range::IndexType& index):
	mMAPtr(&ma), mIndex(index)
    { }
    
    template <typename T, class Range>
    bool MultiArrayBase<T,Range>::const_iterator::operator==(const const_iterator& it) const
    {
	return mMAPtr == it.mMAPtr and mIndex.pos() == it.mIndex.pos();
    }

    template <typename T, class Range>
    bool MultiArrayBase<T,Range>::const_iterator::operator!=(const const_iterator& it) const
    {
	return mMAPtr != it.mMAPtr or mIndex.pos() != it.mIndex.pos();
    }
    
    template <typename T, class Range>
    const T& MultiArrayBase<T,Range>::const_iterator::operator*() const
    {
	return (*mMAPtr)[mIndex];
    }

    template <typename T, class Range>
    T const* MultiArrayBase<T,Range>::const_iterator::operator->() const
    {
	return &(*mMAPtr)[mIndex];
    }
    
    template <typename T, class Range>
    typename MultiArrayBase<T,Range>::const_iterator& MultiArrayBase<T,Range>::const_iterator::operator++()
    {
	++mIndex;
	return *this;
    }

    template <typename T, class Range>
    typename MultiArrayBase<T,Range>::const_iterator MultiArrayBase<T,Range>::const_iterator::operator++(int)
    {
	const_iterator tmp(*this);
	++mIndex;
	return tmp;
    }

    template <typename T, class Range>
    typename MultiArrayBase<T,Range>::const_iterator& MultiArrayBase<T,Range>::const_iterator::operator--()
    {
	--mIndex;
	return *this;
    }

    template <typename T, class Range>
    typename MultiArrayBase<T,Range>::const_iterator MultiArrayBase<T,Range>::const_iterator::operator--(int)
    {
	const_iterator tmp(*this);
	--mIndex;
	return tmp;
    }
    
    template <typename T, class Range>
    typename MultiArrayBase<T,Range>::const_iterator& MultiArrayBase<T,Range>::const_iterator::operator+=(int diff)
    {
	mIndex += diff;
	return *this;
    }

    template <typename T, class Range>
    typename MultiArrayBase<T,Range>::const_iterator& MultiArrayBase<T,Range>::const_iterator::operator-=(int diff)
    {
	mIndex -= diff;
	return *this;
    }

    template <typename T, class Range>
    typename MultiArrayBase<T,Range>::const_iterator MultiArrayBase<T,Range>::const_iterator::operator+(int num) const
    {
	const_iterator tmp(*this);
	tmp += num;
	return tmp;
    }

    template <typename T, class Range>
    typename MultiArrayBase<T,Range>::const_iterator MultiArrayBase<T,Range>::const_iterator::operator-(int num) const
    {
	const_iterator tmp(*this);
	tmp -= num;
    }
    
    template <typename T, class Range>
    int MultiArrayBase<T,Range>::const_iterator::operator-(const const_iterator& it) const
    {
	return mIndex.pos() - it.mIndex.pos();
    }
    
    template <typename T, class Range>
    const T& MultiArrayBase<T,Range>::const_iterator::operator[](int num) const
    {
	return *(operator+(num));
    }
    
    template <typename T, class Range>
    bool MultiArrayBase<T,Range>::const_iterator::operator<(const const_iterator& it) const
    {
	return mIndex.pos() < it.mIndex.pos();
    }

    template <typename T, class Range>
    bool MultiArrayBase<T,Range>::const_iterator::operator>(const const_iterator& it) const
    {
	return mIndex.pos() > it.mIndex.pos();
    }

    template <typename T, class Range>
    bool MultiArrayBase<T,Range>::const_iterator::operator<=(const const_iterator& it) const
    {
	return mIndex.pos() <= it.mIndex.pos();
    }

    template <typename T, class Range>
    bool MultiArrayBase<T,Range>::const_iterator::operator>=(const const_iterator& it) const
    {
	return mIndex.pos() >= it.mIndex.pos();
    }

    template <typename T, class Range>
    const typename Range::IndexType& MultiArrayBase<T,Range>::const_iterator::index() const
    {
	return mIndex;
    }

    template <typename T, class Range>
    typename Range::IndexType& MultiArrayBase<T,Range>::const_iterator::index()
    {
	return mIndex;
    }

    
    /**********************
     *  MultiArrayBase    *	     
     **********************/

    template <typename T, class Range>
    MultiArrayBase<T,Range>::MultiArrayBase(const Range& range) : mRange(new Range(range)) {}

    template <typename T, class Range>
    void MultiArrayBase<T,Range>::link(IndefinitIndexBase* iibPtr) const
    { }
    
    template <typename T, class Range>
    size_t MultiArrayBase<T,Range>::size() const
    {
	return mRange->size();
    }

    template <typename T, class Range>
    typename MultiArrayBase<T,Range>::const_iterator MultiArrayBase<T,Range>::begin() const
    {
	return const_iterator(*this, beginIndex());
    }
    
    template <typename T, class Range>
    typename MultiArrayBase<T,Range>::const_iterator MultiArrayBase<T,Range>::end() const
    {
	return const_iterator(*this, endIndex());
    }
    
    template <typename T, class Range>
    auto MultiArrayBase<T,Range>::beginIndex() const -> decltype(Range().begin())
    {
	//CHECK;
	return mRange->begin();
    }

    template <typename T, class Range>
    auto MultiArrayBase<T,Range>::endIndex() const -> decltype(Range().end())
    {
	return mRange->end();
    }

    template <typename T, class Range>
    const Range& MultiArrayBase<T,Range>::range() const
    {
	return *mRange;
    }

    template <typename T, class Range>
    bool MultiArrayBase<T,Range>::isConst() const
    {
	return true;
    }
    
    template <typename T, class Range>
    template <class... NameTypes>
    ConstMultiArrayOperationRoot<T,Range> MultiArrayBase<T,Range>::operator()(const NameTypes&... str) const
    {
	return ConstMultiArrayOperationRoot<T,Range>(*this, Name("master", str...));
    }

    template <typename T, class Range>
    template <class NameType>
    ConstMultiArrayOperationRoot<T,Range> MultiArrayBase<T,Range>::operator()(const NameType& name, bool master) const
    {
	//CHECK;
	if(master){
	    return ConstMultiArrayOperationRoot<T,Range>(*this, name);
	}
	else {
	    return operator()(name);
	}
    }

    template <typename T, class Range>
    bool MultiArrayBase<T,Range>::isInit() const
    {
	return mInit;
    }

    /****************************************
     *  MutableMultiArrayBase::iterator     *	     
     ****************************************/
    
    template <typename T, class Range>
    MutableMultiArrayBase<T,Range>::iterator::iterator(MutableMultiArrayBase<T,Range>& ma):
	mMAPtr(&ma), mIndex(mMAPtr->beginIndex())
    { }
    
    template <typename T, class Range>
    MutableMultiArrayBase<T,Range>::iterator::iterator(MutableMultiArrayBase<T,Range>& ma,
						       const typename Range::IndexType& index):
	mMAPtr(&ma), mIndex(index)
    { }
    
    template <typename T, class Range>
    bool MutableMultiArrayBase<T,Range>::iterator::operator==(const iterator& it) const
    {
	return mMAPtr == it.mMAPtr and mIndex.pos() == it.mIndex.pos();
    }

    template <typename T, class Range>
    bool MutableMultiArrayBase<T,Range>::iterator::operator!=(const iterator& it) const
    {
	return mMAPtr != it.mMAPtr or mIndex.pos() != it.mIndex.pos();
    }
    
    template <typename T, class Range>
    const T& MutableMultiArrayBase<T,Range>::iterator::operator*() const
    {
	return (*mMAPtr)[mIndex];
    }

    template <typename T, class Range>
    T const* MutableMultiArrayBase<T,Range>::iterator::operator->() const
    {
	return &(*mMAPtr)[mIndex];
    }

    template <typename T, class Range>
    T& MutableMultiArrayBase<T,Range>::iterator::operator*()
    {
	return (*mMAPtr)[mIndex];
    }

    template <typename T, class Range>
    T* MutableMultiArrayBase<T,Range>::iterator::operator->()
    {
	return &(*mMAPtr)[mIndex];
    }

    template <typename T, class Range>
    typename MutableMultiArrayBase<T,Range>::iterator& MutableMultiArrayBase<T,Range>::iterator::operator++()
    {
	++mIndex;
	return *this;
    }

    template <typename T, class Range>
    typename MutableMultiArrayBase<T,Range>::iterator MutableMultiArrayBase<T,Range>::iterator::operator++(int)
    {
	iterator tmp(*this);
	++mIndex;
	return tmp;
    }

    template <typename T, class Range>
    typename MutableMultiArrayBase<T,Range>::iterator& MutableMultiArrayBase<T,Range>::iterator::operator--()
    {
	--mIndex;
	return *this;
    }

    template <typename T, class Range>
    typename MutableMultiArrayBase<T,Range>::iterator MutableMultiArrayBase<T,Range>::iterator::operator--(int)
    {
	iterator tmp(*this);
	--mIndex;
	return tmp;
    }
    
    template <typename T, class Range>
    typename MutableMultiArrayBase<T,Range>::iterator& MutableMultiArrayBase<T,Range>::iterator::operator+=(int diff)
    {
	mIndex += diff;
	return *this;
    }

    template <typename T, class Range>
    typename MutableMultiArrayBase<T,Range>::iterator& MutableMultiArrayBase<T,Range>::iterator::operator-=(int diff)
    {
	mIndex -= diff;
	return *this;
    }

    template <typename T, class Range>
    typename MutableMultiArrayBase<T,Range>::iterator MutableMultiArrayBase<T,Range>::iterator::operator+(int num) const
    {
	iterator tmp(*this);
	tmp += num;
	return tmp;
    }

    template <typename T, class Range>
    typename MutableMultiArrayBase<T,Range>::iterator MutableMultiArrayBase<T,Range>::iterator::operator-(int num) const
    {
	iterator tmp(*this);
	tmp -= num;
    }
    
    template <typename T, class Range>
    int MutableMultiArrayBase<T,Range>::iterator::operator-(const iterator& it) const
    {
	return mIndex.pos() - it.mIndex.pos();
    }
    
    template <typename T, class Range>
    const T& MutableMultiArrayBase<T,Range>::iterator::operator[](int num) const
    {
	return *(operator+(num));
    }

    template <typename T, class Range>
    T& MutableMultiArrayBase<T,Range>::iterator::operator[](int num)
    {
	return *(operator+(num));
    }
    
    template <typename T, class Range>
    bool MutableMultiArrayBase<T,Range>::iterator::operator<(const iterator& it) const
    {
	return mIndex.pos() < it.mIndex.pos();
    }

    template <typename T, class Range>
    bool MutableMultiArrayBase<T,Range>::iterator::operator>(const iterator& it) const
    {
	return mIndex.pos() > it.mIndex.pos();
    }

    template <typename T, class Range>
    bool MutableMultiArrayBase<T,Range>::iterator::operator<=(const iterator& it) const
    {
	return mIndex.pos() <= it.mIndex.pos();
    }

    template <typename T, class Range>
    bool MutableMultiArrayBase<T,Range>::iterator::operator>=(const iterator& it) const
    {
	return mIndex.pos() >= it.mIndex.pos();
    }

    template <typename T, class Range>
    const typename Range::IndexType& MutableMultiArrayBase<T,Range>::iterator::index() const
    {
	return mIndex;
    }

    template <typename T, class Range>
    typename Range::IndexType& MutableMultiArrayBase<T,Range>::iterator::index()
    {
	return mIndex;
    }    
    
    /******************************
     *  MutableMultiArrayBase     *	     
     ******************************/

    template <typename T, class Range>
    MutableMultiArrayBase<T,Range>::MutableMultiArrayBase(const Range& range) : MultiArrayBase<T,Range>(range) {}

    template <typename T, class Range>
    typename MutableMultiArrayBase<T,Range>::iterator MutableMultiArrayBase<T,Range>::begin()
    {
	return iterator(*this, MAB::beginIndex());
    }

    template <typename T, class Range>
    typename MutableMultiArrayBase<T,Range>::iterator MutableMultiArrayBase<T,Range>::end()
    {
	return iterator(*this, MAB::endIndex());
    }
    
    template <typename T, class Range>
    bool MutableMultiArrayBase<T,Range>::isConst() const
    {
	return false;
    }
    
    template <typename T, class Range>
    template <class... NameTypes>
    MultiArrayOperationRoot<T,Range> MutableMultiArrayBase<T,Range>::operator()(const NameTypes&... str)
    {
	return MultiArrayOperationRoot<T,Range>(*this, Name("master", str...));
    }

    template <typename T, class Range>
    template <class NameType>
    MultiArrayOperationRoot<T,Range> MutableMultiArrayBase<T,Range>::operator()(const NameType& name, bool master)
    {
	//CHECK;
	if(master){
	    return MultiArrayOperationRoot<T,Range>(*this, name);
	}
	else {
	    return operator()(name);
	}
    }

    template <typename T, class Range>
    template <class... NameTypes>
    ConstMultiArrayOperationRoot<T,Range> MutableMultiArrayBase<T,Range>::operator()(const NameTypes&... str) const
    {
	return ConstMultiArrayOperationRoot<T,Range>(*this, Name("master", str...));
    }
    
    template <typename T, class Range>
    template <class NameType>
    ConstMultiArrayOperationRoot<T,Range> MutableMultiArrayBase<T,Range>::operator()(const NameType& name, bool master) const
    {
	//CHECK;
	if(master){
	    return ConstMultiArrayOperationRoot<T,Range>(*this, name);
	}
	else {
	    return operator()(name);
	}
    }
    
    /*******************
     *  MultiArray     *	     
     *******************/

    template <typename T, class Range>
    MultiArray<T,Range>::MultiArray(const Range& range) :
	MutableMultiArrayBase<T,Range>(range),
	mCont(MAB::mRange->size())
    {
	MAB::mInit = true;
    }

    template <typename T, class Range>
    MultiArray<T,Range>::MultiArray(const Range& range, const std::vector<T>& vec) :
	MutableMultiArrayBase<T,Range>(range),
	mCont(vec)
    {
	MAB::mInit = true;
	if(mCont.size() > MAB::mRange->size()){
	    mCont.erase(mCont.begin() + MAB::mRange->size(), mCont.end());
	}
    }
    
    template <typename T, class Range>
    MultiArray<T,Range>::MultiArray(const Range& range, std::vector<T>&& vec) :
	MutableMultiArrayBase<T,Range>(range),
	mCont(vec)
    {
	MAB::mInit = true;
	if(mCont.size() > MAB::mRange->size()){
	    mCont.erase(mCont.begin() + MAB::mRange->size(), mCont.end());
	}
    }

    template <typename T, class Range>
    template <class Range2, class Range3>
    MultiArray<T,Range>::MultiArray(const MultiArray<MultiArray<T,Range2>,Range3> in) :
	MutableMultiArrayBase<T,Range>(merge(in.range(), in[ in.beginIndex() ].range()))
	// assert that Range2 has always same extension
    {
	MAB::mInit = true;
	mCont.clear();
	for(auto i = in.beginIndex(); i != in.endIndex(); ++i){
	    mCont.insert(mCont.end(), in[i].mCont.begin(), in[i].mCont.end());
	}
	assert(mCont.size() == MAB::mRange->size());
    }

    template <typename T, class Range>
    template <class Range2, class Range3>
    MultiArray<T,Range>& MultiArray<T,Range>::operator=(const MultiArray<MultiArray<T,Range2>,Range3> in)
    {
	MAB::mRange.reset(new Range(merge(in.range(), in[ in.beginIndex() ].range())));
	// assert that Range2 has always same extension
	mCont.clear();
	for(auto i = in.beginIndex(); i != in.endIndex(); ++i){
	    mCont.insert(mCont.end(), in[i].mCont.begin(), in[i].mCont.end());
	}
	assert(mCont.size() == MAB::mRange->size());
	return *this;
    }    

    template <typename T, class Range>
    T& MultiArray<T,Range>::operator[](const typename Range::IndexType& i)
    {
	return mCont[ i.pos() ];
    }

    template <typename T, class Range>
    const T& MultiArray<T,Range>::operator[](const typename Range::IndexType& i) const
    {
	return mCont[ i.pos() ];
    }

    template <typename T, class Range>
    bool MultiArray<T,Range>::isConst() const
    {
	return false;
    }
    
    template <typename T, class Range>
    bool MultiArray<T,Range>::isSlice() const
    {
	return false;
    }
    
    /*
    template <typename T, class Range>
    void MultiArray<T,Range>::manipulate(ManipulatorBase<T>& mb,
					 const typename Range::IndexType& manBegin,
					 const typename Range::IndexType& manEnd)
    {
	mb.setup(mCont, manBegin.pos(), manEnd.pos());
	mb.execute();
    }
    */


    /****************************
     *   FunctionalMultiArray   *	     
     ****************************/

    /*
    template <typename T, class Range, class Function>
    FunctionalMultiArray<T,Range,Function>::FunctionalMultiArray(const Range& range) :
	MultiArrayBase<T,Range>(range), mFunc() {}
    */
    template <typename T, class Range, class Function>
    FunctionalMultiArray<T,Range,Function>::FunctionalMultiArray(const Range& range,
								 const Function& func) :
	MultiArrayBase<T,Range>(range), mFunc(func) { /*CHECK;*/ }

    template <typename T, class Range, class Function>
    const T& FunctionalMultiArray<T,Range,Function>::operator[](const typename Range::IndexType& i) const
    {
	mVal = mFunc(i);
	return mVal;
    }

    template <typename T, class Range, class Function>
    bool FunctionalMultiArray<T,Range,Function>::isConst() const
    {
	return true;
    }

    template <typename T, class Range, class Function>
    bool FunctionalMultiArray<T,Range,Function>::isSlice() const
    {
	return false;
    }
    
}
