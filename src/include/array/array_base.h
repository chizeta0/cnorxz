
#ifndef __cxz_array_base_h__
#define __cxz_array_base_h__

#include <cstdlib>
#include <vector>
#include <memory>
#include <algorithm>

#include "base/base.h"
#include "aindex.h"
//#include "operation/"

namespace CNORXZ
{
    template <typename T>
    class CArrayBase
    {
    public:
	typedef AIndex<T> const_iterator;

    protected:
	RangePtr mRange;
	
    public:

	CArrayBase(const RangePtr& range);
	DEFAULT_MEMBERS(CArrayBase);

	virtual ~CArrayBase() = default;

	template <typename I, typename M>
	const T& operator[](const IndexInterface<I,M>& i) const;

	template <typename I, typename M>
	const T& at(const IndexInterface<I,M>& i) const;

	template <typename I, typename M>
	Sptr<CArrayBase<T>> sl(const IndexInterface<I,M>& i) const;
	
	virtual const T* data() const = 0;
	virtual SizeT size() const;
	virtual RangePtr range() const;

	virtual const_iterator begin() const;
	virtual const_iterator end() const;
	virtual const_iterator cbegin() const = 0;
	virtual const_iterator cend() const = 0;

	virtual bool isView() const = 0;

	//template <typename I, typename M>
	//ConstOperationRoot<T,I> operator()(const IndexPtr<I,M>& i) const;
    };

    template <typename T>
    class ArrayBase : public CArrayBase<T>
    {
    public:
	typedef CArrayBase<T> CAB;
	typedef typename CAB::const_iterator const_iterator;
	typedef BIndex<T> iterator;

	using CAB::operator[];
	using CAB::at;
	using CAB::data;
	using CAB::begin;
	using CAB::end;
	using CAB::cbegin;
	using CAB::cend;
	//using CAB::operator();

	ArrayBase(const RangePtr& range);
	DEFAULT_MEMBERS(ArrayBase);

	template <typename I, typename M>
	T& operator[](const IndexInterface<I,M>& i);

	template <typename I, typename M>
	T& at(const IndexInterface<I,M>& i);

	template <typename I, typename M>
	Sptr<ArrayBase<T>> sl(const IndexInterface<I,M>& i);

	virtual T* data() = 0;
	
	virtual iterator begin();
	virtual iterator end();
	
	//template <typename I, typename M>
	//OperationRoot<T,I> operator()(const IndexPtr<I,M>& i);
    };
}

#endif
