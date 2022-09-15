
#ifndef __cxz_darray_base_h__
#define __cxz_darray_base_h__

#include <cstdlib>
#include <vector>
#include <memory>
#include <algorithm>

#include "base/base.h"
#include "dcontainer_index.h"
//#include "operation/"

namespace CNORXZ
{
    template <typename T>
    class DArrayBase
    {
    public:
	typedef DConstContainerIndex<T> const_iterator;

    protected:
	RangePtr mRange;
	bool mInit = false;
	
    public:

	DArrayBase(const RangePtr& range);
	DEFAULT_MEMBERS(DArrayBase);

	virtual ~DArrayBase() = default;

	template <typename I, typename M>
	const T& operator[](const IndexInterface<I,M>& i) const;

	template <typename I, typename M>
	const T& at(const IndexInterface<I,M>& i) const;

	template <typename I, typename M>
	DArrayBase sl(const IndexInterface<I,M>& i) const;
	
	virtual const T* data() const = 0;
	virtual size_t size() const;
	virtual RangePtr range() const;

	virtual const_iterator begin() const;
	virtual const_iterator end() const;
	virtual const_iterator cbegin() const = 0;
	virtual const_iterator cend() const = 0;

	virtual bool isView() const = 0;
	virtual bool isInit() const;

	//template <typename I, typename M>
	//ConstOperationRoot<T,I> operator()(const IndexPtr<I,M>& i) const;
    };

    template <typename T>
    class MDArrayBase : public DArrayBase<T>
    {
    public:
	typedef DArrayBase<T> DAB;
	typedef typename DAB::const_iterator const_iterator;
	typedef DContainerIndex<T> iterator;

	using DAB::operator[];
	using DAB::at;
	using DAB::data;
	using DAB::begin;
	using DAB::end;
	using DAB::cbegin;
	using DAB::cend;
	//using DAB::operator();

	MDArrayBase(const RangePtr& range);
	DEFAULT_MEMBERS(MDArrayBase);

	template <typename I, typename M>
	T& operator[](const IndexInterface<I,M>& i);

	template <typename I, typename M>
	T& at(const IndexInterface<I,M>& i);

	template <typename I, typename M>
	std::shared_ptr<DArrayBase<T>> sl(const IndexInterface<I,M>& i);

	virtual T* data() = 0;
	
	virtual iterator begin();
	virtual iterator end();
	
	//template <typename I, typename M>
	//OperationRoot<T,I> operator()(const IndexPtr<I,M>& i);
    };
}

#endif
