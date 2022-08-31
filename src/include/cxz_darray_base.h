
#ifndef __cxz_darray_base_h__
#define __cxz_darray_base_h__

#include <cstdlib>
#include <vector>
#include <memory>
#include <algorithm>

#include "base_def.h"
#include "mbase_def.h"

#include "ranges/rheader.h"

namespace CNORXZ
{
    template <typename T>
    class DArrayBase
    {
    public:
	typedef T value_type;
	typedef DConstContainerIndex<value_type> const_iterator;

    protected:
	RangePtr mRange;
	const_iterator mIt;
	bool mInit = false;
	
    public:

	DArrayBase(const RangePtr& range);
	DEFAULT_MEMBERS(DArrayBase);

	virtual ~DArrayBase() = default;

	template <typename I>
	const value_type& operator[](const IndexInterface<I>& i) const;

	template <typename I>
	const value_type& at(const IndexInterface<I>& i) const;

	template <typename I>
	DArrayBase sl(const IndexInterface<I>& i) const;
	
	virtual const T* data() const = 0;
	virtual size_t size() const;
	virtual RangePtr range() const;

	virtual const_iterator begin() const;
	virtual const_iterator end() const;
	virtual const_iterator cbegin() const = 0;
	virtual const_iterator cend() const = 0;

	virtual bool isView() const = 0;
	virtual bool isInit() const;

	template <typename I>
	ConstOperationRoot<T,I> operator()(const IndexPtr<I>& i) const;
    };

    template <typename T>
    class MDArrayBase : public DArrayBase<T>
    {
    public:
	typedef DArrayBase<T> DAB;
	typedef DAB::value_type value_type;
	typedef DAB::const_iterator const_iterator;
	typedef DContainerIndex<value_type> iterator;

	using DAB::operator[];
	using DAB::at;
	using DAB::data;
	using DAB::begin;
	using DAB::end;
	using DAB::cbegin;
	using DAB::cend;
	using DAB::operator();

	MDArrayBase(const RangePtr& range);
	DEFAULT_MEMBERS(MDArrayBase);

	template <typename I>
	value_type& operator[](const IndexInterface<I>& i);

	template <typename I>
	value_type& at(const IndexInterface<I>& i);

	template <typename I>
	DArrayBase sl(const IndexInterface<I>& i);

	virtual T* data() = 0;
	
	virtual iterator begin();
	virtual iterator end();

	template <typename I>
	OperationRoot<T,I> operator()(const IndexPtr<I>& i);
    };
}

#endif
