// -*- C++ -*-

#ifndef __multi_array_h__
#define __multi_array_h__

#include <cstdlib>
#include <vector>
#include <memory>
#include <iterator>
#include <algorithm>

#include "base_def.h"
#include "multi_range.h"
//#include "multi_array_operation.h"
//#include "manipulator.h"
//#include "name.h"

namespace MultiArrayTools
{
    
    template <typename T, class CRange>
    class MultiArrayBase
    {
    public:

	typedef T value_type;
	
	class const_iterator : public std::iterator<std::random_access_iterator_tag,T>
	{
	public:

	    DEFAULT_MEMBERS(const_iterator);
	    
	    const_iterator(const MultiArrayBase& ma);
	    const_iterator(const MultiArrayBase& ma, const typename CRange::IndexType& index);
	    virtual ~const_iterator() = default;
	    
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
	    typename CRange::IndexType index() const;

	protected:
	    MultiArrayBase const* mMAPtr = nullptr;
	    size_t mPos;
	};

	DEFAULT_MEMBERS(MultiArrayBase);
	MultiArrayBase(const std::shared_ptr<CRange>& range);

	virtual ~MultiArrayBase() = default;
	
	virtual const T& operator[](const typename CRange::IndexType& i) const = 0;

	virtual const T* data() const = 0;
	
	virtual size_t size() const; 
	virtual bool isSlice() const = 0;

	virtual const_iterator begin() const;
	virtual const_iterator end() const;
	
	virtual typename CRange::IndexType beginIndex() const;
	virtual typename CRange::IndexType endIndex() const;

	virtual const CRange& range() const;

	virtual bool isConst() const;
	/*
	template <class... NameTypes>
	ConstMultiArrayOperationRoot<T,CRange> operator()(const NameTypes&... str) const;

	template <class NameType>
	ConstMultiArrayOperationRoot<T,CRange> operator()(const NameType& name, bool master) const;
	*/
	virtual bool isInit() const;

    protected:
	bool mInit = false;
	std::shared_ptr<CRange> mRange;

    };

    template <typename T, class CRange>
    class MutableMultiArrayBase : public MultiArrayBase<T,CRange>
    {
    public:

	typedef typename MultiArrayBase<T,CRange>::const_iterator const_iterator;
	typedef MultiArrayBase<T,CRange> MAB;
	typedef typename CRange::IndexType IndexType;
	
	class iterator : public std::iterator<std::random_access_iterator_tag,T>,
			 public std::iterator<std::output_iterator_tag,T>
	{
	public:

	    DEFAULT_MEMBERS(iterator);
	    
	    iterator(MutableMultiArrayBase& ma);
	    iterator(MutableMultiArrayBase& ma, const IndexType& index);
	    virtual ~iterator() = default;
	    
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
	MutableMultiArrayBase(const std::shared_ptr<CRange>& range);

	virtual T& operator[](const IndexType& i) = 0;

	virtual T* data() = 0;
	
	virtual iterator begin();
	virtual iterator end();

	virtual bool isConst() const override;
	/*
	template <class... NameTypes>
	ConstMultiArrayOperationRoot<T,CRange> operator()(bool x, const NameTypes&... str) const
	{
	    return MAB::operator()(str...);
	}

	template <class... NameTypes>
	ConstMultiArrayOperationRoot<T,CRange> operator()(const NameTypes&... str) const;

	template <class NameType>
	ConstMultiArrayOperationRoot<T,CRange> operator()(const NameType& name, bool master) const;
	
	template <class... NameTypes>
	MultiArrayOperationRoot<T,CRange> operator()(const NameTypes&... str);

	template <class NameType>
	MultiArrayOperationRoot<T,CRange> operator()(const NameType& name, bool master);
	*/
    };
    
    template <typename T, class CRange>
    class MultiArray : public MutableMultiArrayBase<T,CRange>
    {
    public:

	typedef MultiArrayBase<T,CRange> MAB;
	typedef typename MultiArrayBase<T,CRange>::const_iterator const_iterator;
	typedef typename MutableMultiArrayBase<T,CRange>::iterator iterator;
	typedef typename CRange::IndexType IndexType;
	
	DEFAULT_MEMBERS(MultiArray);
	MultiArray(const std::shared_ptr<CRange>& range);
	MultiArray(const std::shared_ptr<CRange>& range, const std::vector<T>& vec);
	MultiArray(const std::shared_ptr<CRange>& range, std::vector<T>&& vec);

	template <class Range2, class Range3>
	MultiArray(const MultiArray<MultiArray<T,Range2>,Range3> in);

	// implement contstructor using FunctionalMultiArray as Input !!!
	
	//template <class Range2, class Range3>
	//MultiArray& operator=(const MultiArray<MultiArray<T,Range2>,Range3> in);
	
	T& operator[](const IndexType& i) override;
	const T& operator[](const IndexType& i) const override;

	virtual bool isConst() const override;	
	virtual bool isSlice() const override;

	template <class Range2>
	MultiArray<T,Range2> format(const std::shared_ptr<Range2>& nr); // reformat array using 'nr' which in
	//                                                                 total must have the same size as mRange
	
	const T* data() const override;
	T* data() override;

	//	virtual void manipulate(ManipulatorBase<T>& mb,
	//			const typename CRange::IndexType& manBegin,
	//				const typename CRange::IndexType& manEnd);
	
	template <typename U, class RangeX>
	friend class MultiArray;
	
    private:
	std::vector<T> mCont;
    };

    template <typename T, class CRange, class Function>
    class FunctionalMultiArray : public MultiArrayBase<T,CRange>
    {
    public:
	typedef MultiArrayBase<T,CRange> MAB;
	typedef typename MultiArrayBase<T,CRange>::const_iterator const_iterator;
	typedef typename CRange::IndexType IndexType;
	
	DEFAULT_MEMBERS(FunctionalMultiArray);
	//FunctionalMultiArray(const CRange& range);
	FunctionalMultiArray(const std::shared_ptr<CRange>& range, const Function& func);

	virtual const T& operator[](const IndexType& i) const override;

	virtual bool isConst() const override;	
	virtual bool isSlice() const override;

    protected:
	mutable T mVal;
	Function mFunc;
    };
    
}

#include "multi_array.cc"

#endif
