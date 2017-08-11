// -*- C++ -*-

#include "multi_array.h"

namespace MultiArrayTools
{
    /**************************************
     *  MultiArrayBase::const_iterator    *	     
     **************************************/

    template <typename T, class CRange>
    MultiArrayBase<T,CRange>::const_iterator::const_iterator(const MultiArrayBase<T,CRange>& ma):
	mMAPtr(&ma), mPos(0) { }

    template <typename T, class CRange>
    MultiArrayBase<T,CRange>::const_iterator::const_iterator(const MultiArrayBase<T,CRange>& ma,
							     const typename CRange::IndexType& index):
	mMAPtr(&ma), mPos(index.pos()) { }
    
    template <typename T, class CRange>
    bool MultiArrayBase<T,CRange>::const_iterator::operator==(const const_iterator& it) const
    {
	return mMAPtr == it.mMAPtr and mPos == it.mPos;
    }

    template <typename T, class CRange>
    bool MultiArrayBase<T,CRange>::const_iterator::operator!=(const const_iterator& it) const
    {
	return mMAPtr != it.mMAPtr or mPos != it.mPos;
    }
    
    template <typename T, class CRange>
    const T& MultiArrayBase<T,CRange>::const_iterator::operator*() const
    {
	return mMAPtr->data()[mPos];
    }

    template <typename T, class CRange>
    T const* MultiArrayBase<T,CRange>::const_iterator::operator->() const
    {
	return &mMAPtr->data()[mPos];
    }
    
    template <typename T, class CRange>
    typename MultiArrayBase<T,CRange>::const_iterator& MultiArrayBase<T,CRange>::const_iterator::operator++()
    {
	++mPos;
	return *this;
    }

    template <typename T, class CRange>
    typename MultiArrayBase<T,CRange>::const_iterator MultiArrayBase<T,CRange>::const_iterator::operator++(int)
    {
	const_iterator tmp(*this);
	++mPos;
	return tmp;
    }

    template <typename T, class CRange>
    typename MultiArrayBase<T,CRange>::const_iterator& MultiArrayBase<T,CRange>::const_iterator::operator--()
    {
	--mPos;
	return *this;
    }

    template <typename T, class CRange>
    typename MultiArrayBase<T,CRange>::const_iterator MultiArrayBase<T,CRange>::const_iterator::operator--(int)
    {
	const_iterator tmp(*this);
	--mPos;
	return tmp;
    }
    
    template <typename T, class CRange>
    typename MultiArrayBase<T,CRange>::const_iterator& MultiArrayBase<T,CRange>::const_iterator::operator+=(int diff)
    {
	mPos += diff;
	return *this;
    }

    template <typename T, class CRange>
    typename MultiArrayBase<T,CRange>::const_iterator& MultiArrayBase<T,CRange>::const_iterator::operator-=(int diff)
    {
	mPos -= diff;
	return *this;
    }

    template <typename T, class CRange>
    typename MultiArrayBase<T,CRange>::const_iterator MultiArrayBase<T,CRange>::const_iterator::operator+(int num) const
    {
	const_iterator tmp(*this);
	tmp += num;
	return tmp;
    }

    template <typename T, class CRange>
    typename MultiArrayBase<T,CRange>::const_iterator MultiArrayBase<T,CRange>::const_iterator::operator-(int num) const
    {
	const_iterator tmp(*this);
	tmp -= num;
    }
    
    template <typename T, class CRange>
    int MultiArrayBase<T,CRange>::const_iterator::operator-(const const_iterator& it) const
    {
	return mPos - it.mPos;
    }
    
    template <typename T, class CRange>
    const T& MultiArrayBase<T,CRange>::const_iterator::operator[](int num) const
    {
	return *(operator+(num));
    }
    
    template <typename T, class CRange>
    bool MultiArrayBase<T,CRange>::const_iterator::operator<(const const_iterator& it) const
    {
	return mPos < it.mPos;
    }

    template <typename T, class CRange>
    bool MultiArrayBase<T,CRange>::const_iterator::operator>(const const_iterator& it) const
    {
	return mPos > it.mPos;
    }

    template <typename T, class CRange>
    bool MultiArrayBase<T,CRange>::const_iterator::operator<=(const const_iterator& it) const
    {
	return mPos <= it.mPos;
    }

    template <typename T, class CRange>
    bool MultiArrayBase<T,CRange>::const_iterator::operator>=(const const_iterator& it) const
    {
	return mPos >= it.mPos;
    }

    template <typename T, class CRange>
    typename CRange::IndexType MultiArrayBase<T,CRange>::const_iterator::index() const
    {
	auto i = mMAPtr->beginIndex();
	i = mPos;
	return i;
    }
    
    /**********************
     *  MultiArrayBase    *	     
     **********************/

    template <typename T, class CRange>
    MultiArrayBase<T,CRange>::MultiArrayBase(const std::shared_ptr<CRange>& range) : mRange(range) {}
    
    template <typename T, class CRange>
    size_t MultiArrayBase<T,CRange>::size() const
    {
	return mRange->size();
    }

    template <typename T, class CRange>
    typename MultiArrayBase<T,CRange>::const_iterator MultiArrayBase<T,CRange>::begin() const
    {
	return const_iterator(*this, beginIndex());
    }
    
    template <typename T, class CRange>
    typename MultiArrayBase<T,CRange>::const_iterator MultiArrayBase<T,CRange>::end() const
    {
	return const_iterator(*this, endIndex());
    }
    
    template <typename T, class CRange>
    typename CRange::IndexType MultiArrayBase<T,CRange>::beginIndex() const
    {
	return mRange->begin();
    }

    template <typename T, class CRange>
    typename CRange::IndexType MultiArrayBase<T,CRange>::endIndex() const
    {
	return mRange->end();
    }

    template <typename T, class CRange>
    const std::shared_ptr<CRange>& MultiArrayBase<T,CRange>::range() const
    {
	return mRange;
    }

    template <typename T, class CRange>
    bool MultiArrayBase<T,CRange>::isConst() const
    {
	return true;
    }
    
    template <typename T, class CRange>
    template <class... SubRanges>
    ConstOperationRoot<T,SubRanges...>
    MultiArrayBase<T,CRange>::operator()(std::shared_ptr<typename SubRanges::IndexType>&... inds) const
    {
	return ConstOperationRoot<T,SubRanges...>(*this, inds...);
    }
    
    template <typename T, class CRange>
    bool MultiArrayBase<T,CRange>::isInit() const
    {
	return mInit;
    }

    /****************************************
     *  MutableMultiArrayBase::iterator     *	     
     ****************************************/
    
    template <typename T, class CRange>
    MutableMultiArrayBase<T,CRange>::iterator::iterator(MutableMultiArrayBase<T,CRange>& ma):
	mMAPtr(&ma), mPos(0)
    { }
    
    template <typename T, class CRange>
    MutableMultiArrayBase<T,CRange>::iterator::iterator(MutableMultiArrayBase<T,CRange>& ma,
							const typename CRange::IndexType& index):
	mMAPtr(&ma), mPos(index.pos())
    { }
    
    template <typename T, class CRange>
    bool MutableMultiArrayBase<T,CRange>::iterator::operator==(const iterator& it) const
    {
	return mMAPtr == it.mMAPtr and mPos == it.mPos;
    }

    template <typename T, class CRange>
    bool MutableMultiArrayBase<T,CRange>::iterator::operator!=(const iterator& it) const
    {
	return mMAPtr != it.mMAPtr or mPos != it.mPos;
    }
    
    template <typename T, class CRange>
    const T& MutableMultiArrayBase<T,CRange>::iterator::operator*() const
    {
	return mMAPtr->data()[mPos];
    }

    template <typename T, class CRange>
    T const* MutableMultiArrayBase<T,CRange>::iterator::operator->() const
    {
	return &mMAPtr->data()[mPos];
    }

    template <typename T, class CRange>
    T& MutableMultiArrayBase<T,CRange>::iterator::operator*()
    {
	return mMAPtr->data()[mPos];
    }

    template <typename T, class CRange>
    T* MutableMultiArrayBase<T,CRange>::iterator::operator->()
    {
	return &mMAPtr->data()[mPos];
    }

    template <typename T, class CRange>
    typename MutableMultiArrayBase<T,CRange>::iterator& MutableMultiArrayBase<T,CRange>::iterator::operator++()
    {
	++mPos;
	return *this;
    }

    template <typename T, class CRange>
    typename MutableMultiArrayBase<T,CRange>::iterator MutableMultiArrayBase<T,CRange>::iterator::operator++(int)
    {
	iterator tmp(*this);
	++mPos;
	return tmp;
    }

    template <typename T, class CRange>
    typename MutableMultiArrayBase<T,CRange>::iterator& MutableMultiArrayBase<T,CRange>::iterator::operator--()
    {
	--mPos;
	return *this;
    }

    template <typename T, class CRange>
    typename MutableMultiArrayBase<T,CRange>::iterator MutableMultiArrayBase<T,CRange>::iterator::operator--(int)
    {
	iterator tmp(*this);
	--mPos;
	return tmp;
    }
    
    template <typename T, class CRange>
    typename MutableMultiArrayBase<T,CRange>::iterator& MutableMultiArrayBase<T,CRange>::iterator::operator+=(int diff)
    {
	mPos += diff;
	return *this;
    }

    template <typename T, class CRange>
    typename MutableMultiArrayBase<T,CRange>::iterator& MutableMultiArrayBase<T,CRange>::iterator::operator-=(int diff)
    {
	mPos -= diff;
	return *this;
    }

    template <typename T, class CRange>
    typename MutableMultiArrayBase<T,CRange>::iterator MutableMultiArrayBase<T,CRange>::iterator::operator+(int num) const
    {
	iterator tmp(*this);
	tmp += num;
	return tmp;
    }

    template <typename T, class CRange>
    typename MutableMultiArrayBase<T,CRange>::iterator MutableMultiArrayBase<T,CRange>::iterator::operator-(int num) const
    {
	iterator tmp(*this);
	tmp -= num;
    }
    
    template <typename T, class CRange>
    int MutableMultiArrayBase<T,CRange>::iterator::operator-(const iterator& it) const
    {
	return mPos - it.mPos;
    }
    
    template <typename T, class CRange>
    const T& MutableMultiArrayBase<T,CRange>::iterator::operator[](int num) const
    {
	return *(operator+(num));
    }

    template <typename T, class CRange>
    T& MutableMultiArrayBase<T,CRange>::iterator::operator[](int num)
    {
	return *(operator+(num));
    }
    
    template <typename T, class CRange>
    bool MutableMultiArrayBase<T,CRange>::iterator::operator<(const iterator& it) const
    {
	return mPos < it.mPos;
    }

    template <typename T, class CRange>
    bool MutableMultiArrayBase<T,CRange>::iterator::operator>(const iterator& it) const
    {
	return mPos > it.mPos;
    }

    template <typename T, class CRange>
    bool MutableMultiArrayBase<T,CRange>::iterator::operator<=(const iterator& it) const
    {
	return mPos <= it.mPos;
    }

    template <typename T, class CRange>
    bool MutableMultiArrayBase<T,CRange>::iterator::operator>=(const iterator& it) const
    {
	return mPos >= it.mPos;
    }

    template <typename T, class CRange>
    typename CRange::IndexType MutableMultiArrayBase<T,CRange>::iterator::index() const
    {
	auto i = mMAPtr->beginIndex();
	i = mPos;
	return i;
    }    
    
    /******************************
     *  MutableMultiArrayBase     *	     
     ******************************/

    template <typename T, class CRange>
    MutableMultiArrayBase<T,CRange>::MutableMultiArrayBase(const std::shared_ptr<CRange>& range) :
	MultiArrayBase<T,CRange>(range) {}

    template <typename T, class CRange>
    typename MutableMultiArrayBase<T,CRange>::iterator MutableMultiArrayBase<T,CRange>::begin()
    {
	return iterator(*this, MAB::beginIndex());
    }

    template <typename T, class CRange>
    typename MutableMultiArrayBase<T,CRange>::iterator MutableMultiArrayBase<T,CRange>::end()
    {
	return iterator(*this, MAB::endIndex());
    }
    
    template <typename T, class CRange>
    bool MutableMultiArrayBase<T,CRange>::isConst() const
    {
	return false;
    }

    template <typename T, class CRange>
    template <class... SubRanges>
    OperationRoot<T,SubRanges...>
    MutableMultiArrayBase<T,CRange>::operator()(std::shared_ptr<typename SubRanges::IndexType>&... inds)
    {
	return OperationRoot<T,SubRanges...>(*this, inds...);
    }

    /*******************
     *  MultiArray     *	     
     *******************/

    template <typename T, class CRange>
    MultiArray<T,CRange>::MultiArray(const std::shared_ptr<CRange>& range) :
	MutableMultiArrayBase<T,CRange>(range),
	mCont(MAB::mRange->size())
    {
	MAB::mInit = true;
    }

    template <typename T, class CRange>
    MultiArray<T,CRange>::MultiArray(const std::shared_ptr<CRange>& range, const std::vector<T>& vec) :
	MutableMultiArrayBase<T,CRange>(range),
	mCont(vec)
    {
	MAB::mInit = true;
	if(mCont.size() > MAB::mRange->size()){
	    mCont.erase(mCont.begin() + MAB::mRange->size(), mCont.end());
	}
    }
    
    template <typename T, class CRange>
    MultiArray<T,CRange>::MultiArray(const std::shared_ptr<CRange>& range, std::vector<T>&& vec) :
	MutableMultiArrayBase<T,CRange>(range),
	mCont(vec)
    {
	MAB::mInit = true;
	if(mCont.size() > MAB::mRange->size()){
	    mCont.erase(mCont.begin() + MAB::mRange->size(), mCont.end());
	}
    }

    template <typename T, class CRange>
    template <class Range2, class Range3>
    MultiArray<T,CRange>::MultiArray(const MultiArray<MultiArray<T,Range2>,Range3> in) :
	MutableMultiArrayBase<T,CRange>(merge(in.range(), in[ in.beginIndex() ].range()))
	// assert that Range2 has always same extension
    {
	MAB::mInit = true;
	mCont.clear();
	for(auto i = in.beginIndex(); i != in.endIndex(); ++i){
	    mCont.insert(mCont.end(), in[i].mCont.begin(), in[i].mCont.end());
	}
	assert(mCont.size() == MAB::mRange->size());
    }

    /*
    template <typename T, class CRange>
    template <class Range2, class Range3>
    MultiArray<T,CRange>& MultiArray<T,CRange>::operator=(const MultiArray<MultiArray<T,Range2>,Range3> in)
    {
	MAB::mRange.reset(new Range(merge(in.range(), in[ in.beginIndex() ].range())));
	// assert that Range2 has always same extension
	mCont.clear();
	for(auto i = in.beginIndex(); i != in.endIndex(); ++i){
	    mCont.insert(mCont.end(), in[i].mCont.begin(), in[i].mCont.end());
	}
	assert(mCont.size() == MAB::mRange->size());
	return *this;
	} */   

    template <typename T, class CRange>
    T& MultiArray<T,CRange>::operator[](const typename CRange::IndexType& i)
    {
	return mCont[ i.pos() ];
    }

    template <typename T, class CRange>
    const T& MultiArray<T,CRange>::operator[](const typename CRange::IndexType& i) const
    {
	return mCont[ i.pos() ];
    }

    template <typename T, class CRange>
    T& MultiArray<T,CRange>::at(const typename CRange::IndexType::MetaType& meta)
    {
	return mCont[ MAB::beginIndex().at(meta).pos() ];
    }

    template <typename T, class CRange>
    const T& MultiArray<T,CRange>::at(const typename CRange::IndexType::MetaType& meta) const
    {
	return mCont[ MAB::beginIndex().at(meta).pos() ];
    }
	
    template <typename T, class CRange>
    bool MultiArray<T,CRange>::isConst() const
    {
	return false;
    }
    
    template <typename T, class CRange>
    bool MultiArray<T,CRange>::isSlice() const
    {
	return false;
    }

    template <typename T, class CRange>
    template <class Range2>
    MultiArray<T,Range2> MultiArray<T,CRange>::format(const std::shared_ptr<Range2>& nr)
    {
	assert(MAB::mRange->size() == nr->size());
	return MultiArray<T,Range2>( nr, std::move(mCont) );
    }
    
    template <typename T, class CRange>
    const T* MultiArray<T,CRange>::data() const
    {
	return mCont.data();
    }

    template <typename T, class CRange>
    T* MultiArray<T,CRange>::data()
    {
	return mCont.data();
    }
    
    /*
    template <typename T, class CRange>
    void MultiArray<T,CRange>::manipulate(ManipulatorBase<T>& mb,
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
	MultiArrayBase<T,CRange>(range), mFunc() {}
    */
    template <typename T, class CRange, class Function>
    FunctionalMultiArray<T,CRange,Function>::FunctionalMultiArray(const std::shared_ptr<CRange>& range,
								  const Function& func) :
	MultiArrayBase<T,CRange>(range), mFunc(func) { /*CHECK;*/ }

    template <typename T, class CRange, class Function>
    const T& FunctionalMultiArray<T,CRange,Function>::operator[](const typename CRange::IndexType& i) const
    {
	mVal = mFunc(i);
	return mVal;
    }

    template <typename T, class CRange, class Function>
    bool FunctionalMultiArray<T,CRange,Function>::isConst() const
    {
	return true;
    }

    template <typename T, class CRange, class Function>
    bool FunctionalMultiArray<T,CRange,Function>::isSlice() const
    {
	return false;
    }
    
}
