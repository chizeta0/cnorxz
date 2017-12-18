// -*- C++ -*-

#ifndef __multi_array_h__
#define __multi_array_h__

#include <cstdlib>
#include <vector>
#include <memory>
#include <iterator>
#include <algorithm>

#include "base_def.h"
#include "mbase_def.h"

#include "ranges/rheader.h"

namespace MultiArrayTools
{

    // Explicitely specify subranges in template argument !!!
    template <typename T, class... SRanges>
    class MultiArrayBase
    {
    public:

	typedef T value_type;
	typedef ContainerRange<SRanges...> CRange;
	typedef typename CRange::IndexType IndexType;
	
	class const_iterator : public std::iterator<std::random_access_iterator_tag,T>
	{
	public:

	    DEFAULT_MEMBERS(const_iterator);
	    
	    const_iterator(const MultiArrayBase& ma);
	    const_iterator(const MultiArrayBase& ma, const typename CRange::IndexType& index);
	    
	    // Requirements:
	    bool operator==(const const_iterator& it) const;
	    bool operator!=(const const_iterator& it) const;

	    const T& operator*() const;
	    T const* operator->() const;

	    const_iterator& operator++();
	    const_iterator operator++(int);
	    const_iterator& operator--();
	    const_iterator operator--(int);

	    const_iterator& operator+=(int diff);
	    const_iterator& operator-=(int diff);
	    const_iterator operator+(int num) const;
	    const_iterator operator-(int num) const;

	    int operator-(const const_iterator& it) const;

	    const T& operator[](int num) const;

	    bool operator<(const const_iterator& it) const;
	    bool operator>(const const_iterator& it) const;
	    bool operator<=(const const_iterator& it) const;
	    bool operator>=(const const_iterator& it) const;

	    // Multi Array specific:
	    typename ContainerRange<SRanges...>::IndexType index() const;
	    
	protected:
	    MultiArrayBase const* mMAPtr = nullptr;
	    size_t mPos;
	};

	DEFAULT_MEMBERS(MultiArrayBase);
	MultiArrayBase(const std::shared_ptr<SRanges>&... ranges);

	virtual ~MultiArrayBase() = default;

	virtual const T& operator[](const IndexType& i) const = 0;
	//virtual const T& operator[](const typename CRange::IndexType& i) const = 0;
	virtual const T& at(const typename CRange::IndexType::MetaType& meta) const = 0;

	virtual const T* data() const = 0;
	virtual const std::vector<T>& datav() const = 0;
	
	virtual size_t size() const; 
	virtual bool isSlice() const = 0;

	virtual const_iterator begin() const;
	virtual const_iterator end() const;
	
	virtual IndexType beginIndex() const;
	virtual IndexType endIndex() const;

	virtual const std::shared_ptr<CRange>& range() const;

	virtual bool isConst() const;

	virtual ConstOperationRoot<T,SRanges...>
	operator()(std::shared_ptr<typename SRanges::IndexType>&... inds) const;
	
	virtual bool isInit() const;

    protected:
	bool mInit = false;
	std::shared_ptr<CRange> mRange;

    };

    template <typename T, class... SRanges>
    class MutableMultiArrayBase : public MultiArrayBase<T,SRanges...>
    {
    public:

	typedef ContainerRange<SRanges...> CRange;
	typedef typename MultiArrayBase<T,SRanges...>::const_iterator const_iterator;
	typedef MultiArrayBase<T,SRanges...> MAB;
	typedef typename CRange::IndexType IndexType;
	
	class iterator : public std::iterator<std::random_access_iterator_tag,T>,
			 public std::iterator<std::output_iterator_tag,T>
	{
	public:

	    DEFAULT_MEMBERS(iterator);
	    
	    iterator(MutableMultiArrayBase& ma);
	    iterator(MutableMultiArrayBase& ma, const IndexType& index);
	    
	    // Requirements:
	    bool operator==(const iterator& it) const;
	    bool operator!=(const iterator& it) const;

	    const T& operator*() const;
	    T const* operator->() const;
	    T& operator*();
	    T* operator->();

	    iterator& operator++();
	    iterator operator++(int);
	    iterator& operator--();
	    iterator operator--(int);

	    iterator& operator+=(int diff);
	    iterator& operator-=(int diff);
	    iterator operator+(int num) const;
	    iterator operator-(int num) const;

	    int operator-(const iterator& it) const;

	    const T& operator[](int num) const;
	    T& operator[](int num);

	    bool operator<(const iterator& it) const;
	    bool operator>(const iterator& it) const;
	    bool operator<=(const iterator& it) const;
	    bool operator>=(const iterator& it) const;

	    // Multi Array specific:
	    typename CRange::IndexType index() const;
	    	    
	protected:
	    MutableMultiArrayBase* mMAPtr = nullptr;
	    size_t mPos;
	};

	
	DEFAULT_MEMBERS(MutableMultiArrayBase);
	MutableMultiArrayBase(const std::shared_ptr<SRanges>&... ranges);

	virtual T& operator[](const IndexType& i) = 0;
	virtual T& at(const typename CRange::IndexType::MetaType& meta) = 0;
	
	virtual T* data() = 0;
	virtual std::vector<T>& datav() = 0;
	
	virtual iterator begin();
	virtual iterator end();

	virtual bool isConst() const override;

	virtual ConstOperationRoot<T,SRanges...>
	operator()(std::shared_ptr<typename SRanges::IndexType>&... inds) const override;
	virtual OperationRoot<T,SRanges...> operator()(std::shared_ptr<typename SRanges::IndexType>&... inds);
    };
    
    template <typename T, class... SRanges>
    class MultiArray : public MutableMultiArrayBase<T,SRanges...>
    {
    public:

	typedef ContainerRange<SRanges...> CRange;
	typedef MultiArrayBase<T,SRanges...> MAB;
	typedef typename MultiArrayBase<T,SRanges...>::const_iterator const_iterator;
	typedef typename MutableMultiArrayBase<T,SRanges...>::iterator iterator;
	typedef typename CRange::IndexType IndexType;
	
	DEFAULT_MEMBERS(MultiArray);
	MultiArray(const std::shared_ptr<SRanges>&... ranges);
	MultiArray(const std::shared_ptr<SRanges>&... ranges, const std::vector<T>& vec);
	MultiArray(const std::shared_ptr<SRanges>&... ranges, std::vector<T>&& vec);

	// Only if ALL ranges have default extensions:
	//MultiArray(const std::vector<T>& vec);
	//MultiArray(std::vector<T>&& vec);
	
	// template <class Range2, class Range3>
	// MultiArray(const MultiArray<MultiArray<T,Range2>,Range3> in);

	// implement contstructor using FunctionalMultiArray as Input !!!
	
	//template <class Range2, class Range3>
	//MultiArray& operator=(const MultiArray<MultiArray<T,Range2>,Range3> in);
	
	virtual T& operator[](const IndexType& i) override;
	virtual const T& operator[](const IndexType& i) const override;
	virtual T& at(const typename CRange::IndexType::MetaType& meta) override;
	virtual const T& at(const typename CRange::IndexType::MetaType& meta) const override;
	
	virtual bool isConst() const override;	
	virtual bool isSlice() const override;

	template <class... SRanges2>
	MultiArray<T,SRanges2...> format(const std::shared_ptr<SRanges2>&... nrs); // reformat array using 'nr' which in
	//                                                                 total must have the same size as mRange
	
	virtual const T* data() const override;
	virtual T* data() override;

	virtual const std::vector<T>& datav() const override;
	virtual std::vector<T>& datav() override;
	
	//	virtual void manipulate(ManipulatorBase<T>& mb,
	//			const typename CRange::IndexType& manBegin,
	//				const typename CRange::IndexType& manEnd);
	
	template <typename U, class... SRanges2>
	friend class MultiArray;
	
    private:
	std::vector<T> mCont;
    };

    template <typename T, class Function, class... SRanges>
    class FunctionalMultiArray : public MultiArrayBase<T,SRanges...>
    {
    public:

	typedef ContainerRange<SRanges...> CRange;
	typedef MultiArrayBase<T,CRange> MAB;
	typedef typename MultiArrayBase<T,CRange>::const_iterator const_iterator;
	typedef typename CRange::IndexType IndexType;
	
	DEFAULT_MEMBERS(FunctionalMultiArray);
	//FunctionalMultiArray(const CRange& range);
	FunctionalMultiArray(const std::shared_ptr<SRanges>&... ranges, const Function& func);

	virtual const T& operator[](const IndexType& i) const override;

	virtual bool isConst() const override;	
	virtual bool isSlice() const override;

    protected:
	mutable T mVal;
	Function mFunc;
    };
    
}

/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */

namespace MultiArrayTools
{
    /**************************************
     *  MultiArrayBase::const_iterator    *	     
     **************************************/

    template <typename T, class... SRanges>
    MultiArrayBase<T,SRanges...>::const_iterator::const_iterator(const MultiArrayBase<T,SRanges...>& ma):
	mMAPtr(&ma), mPos(0) { }

    template <typename T, class... SRanges>
    MultiArrayBase<T,SRanges...>::const_iterator::const_iterator(const MultiArrayBase<T,SRanges...>& ma,
							     const typename CRange::IndexType& index):
	mMAPtr(&ma), mPos(index.pos()) { }
    
    template <typename T, class... SRanges>
    bool MultiArrayBase<T,SRanges...>::const_iterator::operator==(const const_iterator& it) const
    {
	return mMAPtr == it.mMAPtr and mPos == it.mPos;
    }

    template <typename T, class... SRanges>
    bool MultiArrayBase<T,SRanges...>::const_iterator::operator!=(const const_iterator& it) const
    {
	return mMAPtr != it.mMAPtr or mPos != it.mPos;
    }
    
    template <typename T, class... SRanges>
    const T& MultiArrayBase<T,SRanges...>::const_iterator::operator*() const
    {
	return mMAPtr->data()[mPos];
    }

    template <typename T, class... SRanges>
    T const* MultiArrayBase<T,SRanges...>::const_iterator::operator->() const
    {
	return &mMAPtr->data()[mPos];
    }
    
    template <typename T, class... SRanges>
    typename MultiArrayBase<T,SRanges...>::const_iterator& MultiArrayBase<T,SRanges...>::const_iterator::operator++()
    {
	++mPos;
	return *this;
    }

    template <typename T, class... SRanges>
    typename MultiArrayBase<T,SRanges...>::const_iterator MultiArrayBase<T,SRanges...>::const_iterator::operator++(int)
    {
	const_iterator tmp(*this);
	++mPos;
	return tmp;
    }

    template <typename T, class... SRanges>
    typename MultiArrayBase<T,SRanges...>::const_iterator& MultiArrayBase<T,SRanges...>::const_iterator::operator--()
    {
	--mPos;
	return *this;
    }

    template <typename T, class... SRanges>
    typename MultiArrayBase<T,SRanges...>::const_iterator MultiArrayBase<T,SRanges...>::const_iterator::operator--(int)
    {
	const_iterator tmp(*this);
	--mPos;
	return tmp;
    }
    
    template <typename T, class... SRanges>
    typename MultiArrayBase<T,SRanges...>::const_iterator& MultiArrayBase<T,SRanges...>::const_iterator::operator+=(int diff)
    {
	mPos += diff;
	return *this;
    }

    template <typename T, class... SRanges>
    typename MultiArrayBase<T,SRanges...>::const_iterator& MultiArrayBase<T,SRanges...>::const_iterator::operator-=(int diff)
    {
	mPos -= diff;
	return *this;
    }

    template <typename T, class... SRanges>
    typename MultiArrayBase<T,SRanges...>::const_iterator MultiArrayBase<T,SRanges...>::const_iterator::operator+(int num) const
    {
	const_iterator tmp(*this);
	tmp += num;
	return tmp;
    }

    template <typename T, class... SRanges>
    typename MultiArrayBase<T,SRanges...>::const_iterator MultiArrayBase<T,SRanges...>::const_iterator::operator-(int num) const
    {
	const_iterator tmp(*this);
	tmp -= num;
    }
    
    template <typename T, class... SRanges>
    int MultiArrayBase<T,SRanges...>::const_iterator::operator-(const const_iterator& it) const
    {
	return mPos - it.mPos;
    }
    
    template <typename T, class... SRanges>
    const T& MultiArrayBase<T,SRanges...>::const_iterator::operator[](int num) const
    {
	return *(operator+(num));
    }
    
    template <typename T, class... SRanges>
    bool MultiArrayBase<T,SRanges...>::const_iterator::operator<(const const_iterator& it) const
    {
	return mPos < it.mPos;
    }

    template <typename T, class... SRanges>
    bool MultiArrayBase<T,SRanges...>::const_iterator::operator>(const const_iterator& it) const
    {
	return mPos > it.mPos;
    }

    template <typename T, class... SRanges>
    bool MultiArrayBase<T,SRanges...>::const_iterator::operator<=(const const_iterator& it) const
    {
	return mPos <= it.mPos;
    }

    template <typename T, class... SRanges>
    bool MultiArrayBase<T,SRanges...>::const_iterator::operator>=(const const_iterator& it) const
    {
	return mPos >= it.mPos;
    }

    template <typename T, class... SRanges>
    typename MultiArrayBase<T,SRanges...>::IndexType
    MultiArrayBase<T,SRanges...>::const_iterator::index() const
    {
	auto i = mMAPtr->beginIndex();
	i = mPos;
	return i;
    }
    
    /**********************
     *  MultiArrayBase    *	     
     **********************/

    template <typename T, class... SRanges>
    MultiArrayBase<T,SRanges...>::MultiArrayBase(const std::shared_ptr<SRanges>&... ranges)
    {
	ContainerRangeFactory<SRanges...> crf(ranges...);
	mRange = std::dynamic_pointer_cast<ContainerRange<SRanges...> >( crf.create() );
    }
    
    template <typename T, class... SRanges>
    size_t MultiArrayBase<T,SRanges...>::size() const
    {
	return mRange->size();
    }

    template <typename T, class... SRanges>
    typename MultiArrayBase<T,SRanges...>::const_iterator MultiArrayBase<T,SRanges...>::begin() const
    {
	return const_iterator(*this, beginIndex());
    }
    
    template <typename T, class... SRanges>
    typename MultiArrayBase<T,SRanges...>::const_iterator MultiArrayBase<T,SRanges...>::end() const
    {
	return const_iterator(*this, endIndex());
    }
    
    template <typename T, class... SRanges>
    typename MultiArrayBase<T,SRanges...>::IndexType
    MultiArrayBase<T,SRanges...>::beginIndex() const
    {
	return mRange->begin();
    }

    template <typename T, class... SRanges>
    typename MultiArrayBase<T,SRanges...>::IndexType
    MultiArrayBase<T,SRanges...>::endIndex() const
    {
	return mRange->end();
    }

    template <typename T, class... SRanges>
    const std::shared_ptr<typename MultiArrayBase<T,SRanges...>::CRange>&
    MultiArrayBase<T,SRanges...>::range() const
    {
	return mRange;
    }

    template <typename T, class... SRanges>
    bool MultiArrayBase<T,SRanges...>::isConst() const
    {
	return true;
    }
    
    template <typename T, class... SRanges>
    ConstOperationRoot<T,SRanges...>
    MultiArrayBase<T,SRanges...>::operator()(std::shared_ptr<typename SRanges::IndexType>&... inds) const
    {
	return ConstOperationRoot<T,SRanges...>(*this, inds...);
    }
    
    template <typename T, class... SRanges>
    bool MultiArrayBase<T,SRanges...>::isInit() const
    {
	return mInit;
    }

    /****************************************
     *  MutableMultiArrayBase::iterator     *	     
     ****************************************/
    
    template <typename T, class... SRanges>
    MutableMultiArrayBase<T,SRanges...>::iterator::iterator(MutableMultiArrayBase<T,SRanges...>& ma):
	mMAPtr(&ma), mPos(0)
    { }
    
    template <typename T, class... SRanges>
    MutableMultiArrayBase<T,SRanges...>::iterator::iterator(MutableMultiArrayBase<T,SRanges...>& ma,
							const typename CRange::IndexType& index):
	mMAPtr(&ma), mPos(index.pos())
    { }
    
    template <typename T, class... SRanges>
    bool MutableMultiArrayBase<T,SRanges...>::iterator::operator==(const iterator& it) const
    {
	return mMAPtr == it.mMAPtr and mPos == it.mPos;
    }

    template <typename T, class... SRanges>
    bool MutableMultiArrayBase<T,SRanges...>::iterator::operator!=(const iterator& it) const
    {
	return mMAPtr != it.mMAPtr or mPos != it.mPos;
    }
    
    template <typename T, class... SRanges>
    const T& MutableMultiArrayBase<T,SRanges...>::iterator::operator*() const
    {
	return mMAPtr->data()[mPos];
    }

    template <typename T, class... SRanges>
    T const* MutableMultiArrayBase<T,SRanges...>::iterator::operator->() const
    {
	return &mMAPtr->data()[mPos];
    }

    template <typename T, class... SRanges>
    T& MutableMultiArrayBase<T,SRanges...>::iterator::operator*()
    {
	return mMAPtr->data()[mPos];
    }

    template <typename T, class... SRanges>
    T* MutableMultiArrayBase<T,SRanges...>::iterator::operator->()
    {
	return &mMAPtr->data()[mPos];
    }

    template <typename T, class... SRanges>
    typename MutableMultiArrayBase<T,SRanges...>::iterator& MutableMultiArrayBase<T,SRanges...>::iterator::operator++()
    {
	++mPos;
	return *this;
    }

    template <typename T, class... SRanges>
    typename MutableMultiArrayBase<T,SRanges...>::iterator MutableMultiArrayBase<T,SRanges...>::iterator::operator++(int)
    {
	iterator tmp(*this);
	++mPos;
	return tmp;
    }

    template <typename T, class... SRanges>
    typename MutableMultiArrayBase<T,SRanges...>::iterator& MutableMultiArrayBase<T,SRanges...>::iterator::operator--()
    {
	--mPos;
	return *this;
    }

    template <typename T, class... SRanges>
    typename MutableMultiArrayBase<T,SRanges...>::iterator MutableMultiArrayBase<T,SRanges...>::iterator::operator--(int)
    {
	iterator tmp(*this);
	--mPos;
	return tmp;
    }
    
    template <typename T, class... SRanges>
    typename MutableMultiArrayBase<T,SRanges...>::iterator& MutableMultiArrayBase<T,SRanges...>::iterator::operator+=(int diff)
    {
	mPos += diff;
	return *this;
    }

    template <typename T, class... SRanges>
    typename MutableMultiArrayBase<T,SRanges...>::iterator& MutableMultiArrayBase<T,SRanges...>::iterator::operator-=(int diff)
    {
	mPos -= diff;
	return *this;
    }

    template <typename T, class... SRanges>
    typename MutableMultiArrayBase<T,SRanges...>::iterator MutableMultiArrayBase<T,SRanges...>::iterator::operator+(int num) const
    {
	iterator tmp(*this);
	tmp += num;
	return tmp;
    }

    template <typename T, class... SRanges>
    typename MutableMultiArrayBase<T,SRanges...>::iterator MutableMultiArrayBase<T,SRanges...>::iterator::operator-(int num) const
    {
	iterator tmp(*this);
	tmp -= num;
    }
    
    template <typename T, class... SRanges>
    int MutableMultiArrayBase<T,SRanges...>::iterator::operator-(const iterator& it) const
    {
	return mPos - it.mPos;
    }
    
    template <typename T, class... SRanges>
    const T& MutableMultiArrayBase<T,SRanges...>::iterator::operator[](int num) const
    {
	return *(operator+(num));
    }

    template <typename T, class... SRanges>
    T& MutableMultiArrayBase<T,SRanges...>::iterator::operator[](int num)
    {
	return *(operator+(num));
    }
    
    template <typename T, class... SRanges>
    bool MutableMultiArrayBase<T,SRanges...>::iterator::operator<(const iterator& it) const
    {
	return mPos < it.mPos;
    }

    template <typename T, class... SRanges>
    bool MutableMultiArrayBase<T,SRanges...>::iterator::operator>(const iterator& it) const
    {
	return mPos > it.mPos;
    }

    template <typename T, class... SRanges>
    bool MutableMultiArrayBase<T,SRanges...>::iterator::operator<=(const iterator& it) const
    {
	return mPos <= it.mPos;
    }

    template <typename T, class... SRanges>
    bool MutableMultiArrayBase<T,SRanges...>::iterator::operator>=(const iterator& it) const
    {
	return mPos >= it.mPos;
    }

    template <typename T, class... SRanges>
    typename MutableMultiArrayBase<T,SRanges...>::IndexType
    MutableMultiArrayBase<T,SRanges...>::iterator::index() const
    {
	auto i = mMAPtr->beginIndex();
	i = mPos;
	return i;
    }    
    
    /******************************
     *  MutableMultiArrayBase     *	     
     ******************************/

    template <typename T, class... SRanges>
    MutableMultiArrayBase<T,SRanges...>::MutableMultiArrayBase(const std::shared_ptr<SRanges>&... ranges) :
	MultiArrayBase<T,SRanges...>(ranges...) {}

    template <typename T, class... SRanges>
    typename MutableMultiArrayBase<T,SRanges...>::iterator MutableMultiArrayBase<T,SRanges...>::begin()
    {
	return iterator(*this, MAB::beginIndex());
    }

    template <typename T, class... SRanges>
    typename MutableMultiArrayBase<T,SRanges...>::iterator MutableMultiArrayBase<T,SRanges...>::end()
    {
	return iterator(*this, MAB::endIndex());
    }
    
    template <typename T, class... SRanges>
    bool MutableMultiArrayBase<T,SRanges...>::isConst() const
    {
	return false;
    }

    template <typename T, class... SRanges>
    OperationRoot<T,SRanges...>
    MutableMultiArrayBase<T,SRanges...>::operator()(std::shared_ptr<typename SRanges::IndexType>&... inds)
    {
	return OperationRoot<T,SRanges...>(*this, inds...);
    }
    
    template <typename T, class... SRanges>
    ConstOperationRoot<T,SRanges...>
    MutableMultiArrayBase<T,SRanges...>::operator()(std::shared_ptr<typename SRanges::IndexType>&... inds) const
    {
	return ConstOperationRoot<T,SRanges...>(*this, inds...);
    }
    
    
    /*******************
     *  MultiArray     *	     
     *******************/

    template <typename T, class... SRanges>
    MultiArray<T,SRanges...>::MultiArray(const std::shared_ptr<SRanges>&... ranges) :
	MutableMultiArrayBase<T,SRanges...>(ranges...),
	mCont(MAB::mRange->size())
    {
	MAB::mInit = true;
    }

    template <typename T, class... SRanges>
    MultiArray<T,SRanges...>::MultiArray(const std::shared_ptr<SRanges>&... ranges, const std::vector<T>& vec) :
	MutableMultiArrayBase<T,SRanges...>(ranges...),
	mCont(vec)
    {
	MAB::mInit = true;
	if(mCont.size() > MAB::mRange->size()){
	    mCont.erase(mCont.begin() + MAB::mRange->size(), mCont.end());
	}
    }
    
    template <typename T, class... SRanges>
    MultiArray<T,SRanges...>::MultiArray(const std::shared_ptr<SRanges>&... ranges, std::vector<T>&& vec) :
	MutableMultiArrayBase<T,SRanges...>(ranges...),
	mCont(vec)
    {
	MAB::mInit = true;
	if(mCont.size() > MAB::mRange->size()){
	    mCont.erase(mCont.begin() + MAB::mRange->size(), mCont.end());
	}
    }

    /*
    template <typename T, class... SRanges>
    template <class Range2, class Range3>
    MultiArray<T,SRanges...>::MultiArray(const MultiArray<MultiArray<T,Range2>,Range3> in) :
	MutableMultiArrayBase<T,SRanges...>(merge(in.range(), in[ in.beginIndex() ].range()))
	// assert that Range2 has always same extension
    {
	MAB::mInit = true;
	mCont.clear();
	for(auto i = in.beginIndex(); i != in.endIndex(); ++i){
	    mCont.insert(mCont.end(), in[i].mCont.begin(), in[i].mCont.end());
	}
	assert(mCont.size() == MAB::mRange->size());
    }
    */
    /*
    template <typename T, class... SRanges>
    template <class Range2, class Range3>
    MultiArray<T,SRanges...>& MultiArray<T,SRanges...>::operator=(const MultiArray<MultiArray<T,Range2>,Range3> in)
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

    template <typename T, class... SRanges>
    T& MultiArray<T,SRanges...>::operator[](const typename CRange::IndexType& i)
    {
	return mCont[ i.pos() ];
    }

    template <typename T, class... SRanges>
    const T& MultiArray<T,SRanges...>::operator[](const typename CRange::IndexType& i) const
    {
	return mCont[ i.pos() ];
    }

    template <typename T, class... SRanges>
    T& MultiArray<T,SRanges...>::at(const typename CRange::IndexType::MetaType& meta)
    {
	return mCont[ MAB::beginIndex().at(meta).pos() ];
    }

    template <typename T, class... SRanges>
    const T& MultiArray<T,SRanges...>::at(const typename CRange::IndexType::MetaType& meta) const
    {
	return mCont[ MAB::beginIndex().at(meta).pos() ];
    }
	
    template <typename T, class... SRanges>
    bool MultiArray<T,SRanges...>::isConst() const
    {
	return false;
    }
    
    template <typename T, class... SRanges>
    bool MultiArray<T,SRanges...>::isSlice() const
    {
	return false;
    }

    template <typename T, class... SRanges>
    template <class... SRanges2>
    MultiArray<T,SRanges2...> MultiArray<T,SRanges...>::format(const std::shared_ptr<SRanges2>&... nrs)
    {
	return MultiArray<T,SRanges2...>( nrs... , std::move(mCont) );
    }
    
    template <typename T, class... SRanges>
    const T* MultiArray<T,SRanges...>::data() const
    {
	return mCont.data();
    }

    template <typename T, class... SRanges>
    T* MultiArray<T,SRanges...>::data()
    {
	return mCont.data();
    }

    template <typename T, class... SRanges>
    const std::vector<T>& MultiArray<T,SRanges...>::datav() const
    {
	return mCont;
    }

    template <typename T, class... SRanges>
    std::vector<T>& MultiArray<T,SRanges...>::datav()
    {
	return mCont;
    }

    
    /*
    template <typename T, class... SRanges>
    void MultiArray<T,SRanges...>::manipulate(ManipulatorBase<T>& mb,
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
	MultiArrayBase<T,SRanges...>(range), mFunc() {}
    */
    template <typename T, class Function, class... SRanges>
    FunctionalMultiArray<T,Function,SRanges...>::FunctionalMultiArray(const std::shared_ptr<SRanges>&... ranges,
								  const Function& func) :
	MultiArrayBase<T,SRanges...>(ranges...), mFunc(func) {}

    template <typename T, class Function, class... SRanges>
    const T& FunctionalMultiArray<T,Function,SRanges...>::operator[](const typename CRange::IndexType& i) const
    {
	mVal = mFunc(i);
	return mVal;
    }

    template <typename T, class Function, class... SRanges>
    bool FunctionalMultiArray<T,Function,SRanges...>::isConst() const
    {
	return true;
    }

    template <typename T, class Function, class... SRanges>
    bool FunctionalMultiArray<T,Function,SRanges...>::isSlice() const
    {
	return false;
    }
    
}

#endif
