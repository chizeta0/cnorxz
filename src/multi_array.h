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
#include "multi_array_operation.h"
#include "manipulator.h"
#include "name.h"

namespace MultiArrayTools
{

    template <class... Ranges>
    using TopRange = MultiRange<Ranges...>;
    
    template <typename T, class... Ranges>
    class MultiArrayBase
    {
    public:

	typedef TopRange<Ranges...> TopRangeType
	typedef T value_type;
	
	class const_iterator : public std::iterator<std::random_access_iterator_tag,T>
	{
	public:

	    DEFAULT_MEMBERS(const_iterator);
	    
	    const_iterator(const MultiArrayBase& ma);
	    const_iterator(const MultiArrayBase& ma, const typename TopRangeType::IndexType& index);
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
	    
	    const typename TopRangeType::IndexType& index() const;
	    typename TopRangeType::IndexType& index();

	protected:
	    MultiArrayBase const* mMAPtr = nullptr;
	    typename TopRangeType::IndexType mIndex;
	};

	DEFAULT_MEMBERS(MultiArrayBase);
	MultiArrayBase(const TopRangeType& range);

	virtual ~MultiArrayBase() = default;

	// only relevant for slices... has no effect for usual multiarrays
	virtual void link(IndefinitIndexBase* iibPtr) const;
	
	virtual const T& operator[](const typename TopRangeType::IndexType& i) const = 0;

	virtual size_t size() const; 
	virtual bool isSlice() const = 0;

	virtual const_iterator begin() const;
	virtual const_iterator end() const;
	
	virtual auto beginIndex() const -> decltype(Range().begin());
	virtual auto endIndex() const -> decltype(Range().end());

	virtual const TopRangeType& range() const;

	virtual bool isConst() const;
	
	template <class... NameTypes>
	ConstMultiArrayOperationRoot<T,TopRangeType> operator()(const NameTypes&... str) const;

	template <class NameType>
	ConstMultiArrayOperationRoot<T,TopRangeType> operator()(const NameType& name, bool master) const;

	virtual bool isInit() const;
	
    protected:
	bool mInit = false;
	std::shared_ptr<TopRangeType> mRange;

    };

    template <typename T, class... Ranges>
    class MutableMultiArrayBase : public MultiArrayBase<T,Ranges...>
    {
    public:

	typedef typename MultiArrayBase<T,Ranges...>::const_iterator const_iterator;
	typedef MultiArrayBase<T,Ranges...> MAB;
	typedef typename MAB::TopRangeType TopRangeType;
	typedef typename TopRangeType::IndexType IndexType;
	
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

	    const IndexType& index() const;
	    IndexType& index();
	    
	protected:
	    MutableMultiArrayBase* mMAPtr = nullptr;
	    IndexType mIndex;
	};

	
	DEFAULT_MEMBERS(MutableMultiArrayBase);
	MutableMultiArrayBase(const TopRangeType& range);

	virtual T& operator[](const IndexType& i) = 0;
	
	virtual iterator begin();
	virtual iterator end();

	virtual bool isConst() const override;

	template <class... NameTypes>
	ConstMultiArrayOperationRoot<T,Ranges...> operator()(bool x, const NameTypes&... str) const
	{
	    return MAB::operator()(str...);
	}

	template <class... NameTypes>
	ConstMultiArrayOperationRoot<T,Ranges...> operator()(const NameTypes&... str) const;

	template <class NameType>
	ConstMultiArrayOperationRoot<T,Ranges...> operator()(const NameType& name, bool master) const;
	
	template <class... NameTypes>
	MultiArrayOperationRoot<T,Ranges...> operator()(const NameTypes&... str);

	template <class NameType>
	MultiArrayOperationRoot<T,Ranges...> operator()(const NameType& name, bool master);

    };
    
    template <typename T, class Ranges...>
    class MultiArray : public MutableMultiArrayBase<T,Ranges...>
    {
    public:

	typedef MultiArrayBase<T,Ranges...> MAB;
	typedef typename MultiArrayBase<T,Ranges...>::const_iterator const_iterator;
	typedef typename MutableMultiArrayBase<T,Ranges...>::iterator iterator;
	typedef typename MAB::TopRangeType TopRangeType;
	typedef typename TopRangeType::IndexType IndexType;
	
	DEFAULT_MEMBERS(MultiArray);
	MultiArray(const TopRangeType& range);
	MultiArray(const TopRangeType& range, const std::vector<T>& vec);
	MultiArray(const TopRangeType& range, std::vector<T>&& vec);

	template <class Range2, class Range3>
	MultiArray(const MultiArray<MultiArray<T,Range2>,Range3> in);

	// implement contstructor using FunctionalMultiArray as Input !!!
	
	template <class Range2, class Range3>
	MultiArray& operator=(const MultiArray<MultiArray<T,Range2>,Range3> in);
	
	T& operator[](const IndexType& i) override;
	const T& operator[](const IndexType& i) const override;

	virtual bool isConst() const override;	
	virtual bool isSlice() const override;

	const T* data() const;

	//	virtual void manipulate(ManipulatorBase<T>& mb,
	//			const typename Ranges...::IndexType& manBegin,
	//				const typename Ranges...::IndexType& manEnd);
	
	template <typename U, class RangeX>
	friend class MultiArray;
	
    private:
	std::vector<T> mCont;
    };

    template <typename T, class Ranges..., class Function>
    class FunctionalMultiArray : public MultiArrayBase<T,Ranges...>
    {
    public:
	typedef MultiArrayBase<T,Ranges...> MAB;
	typedef typename MultiArrayBase<T,Ranges...>::const_iterator const_iterator;
	typedef typename MAB::TopRangeType TopRangeType;
	typedef typename TopRangeType::IndexType IndexType;
	
	DEFAULT_MEMBERS(FunctionalMultiArray);
	//FunctionalMultiArray(const Ranges...& range);
	FunctionalMultiArray(const TopRangeType& range, const Function& func);

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
