
#ifndef __cxz_array_base_h__
#define __cxz_array_base_h__

#include <cstdlib>
#include <vector>
#include <memory>
#include <algorithm>

#include "base/base.h"
#include "aindex.h"
#include "operation/op_types.h"

namespace CNORXZ
{
    template <typename T>
    class CArrayBase
    {
    public:
	typedef AIndex<T> const_iterator;

	CArrayBase(const RangePtr& range);
	DEFAULT_MEMBERS(CArrayBase);

	virtual ~CArrayBase() = default;

	template <typename I, typename M>
	const T& operator[](const IndexInterface<I,M>& i) const;

	template <typename I, typename M>
	const T& at(const IndexInterface<I,M>& i) const;

	template <class... Indices>
	const T& operator[](const SPack<Indices...>& pack) const;

	template <class... Indices>
	const T& at(const SPack<Indices...>& pack) const;

	const T& operator[](const DPack& pack) const;
	const T& at(const DPack& pack) const;

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

	template <class Index>
	COpRoot<T,Index> operator()(const Sptr<Index>& i) const;

    protected:
	RangePtr mRange;

	template <class Acc>
	const_iterator itLex(const Acc& acc) const;

	template <class Acc>
	const_iterator itLexSave(const Acc& acc) const;
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

	template <class... Indices>
	T& operator[](const SPack<Indices...>& pack);

	template <class... Indices>
	T& at(const SPack<Indices...>& pack);

	T& operator[](const DPack& pack);
	T& at(const DPack& pack);

	template <typename I, typename M>
	Sptr<ArrayBase<T>> sl(const IndexInterface<I,M>& i);

	virtual T* data() = 0;
	
	virtual iterator begin();
	virtual iterator end();
	
	template <class Index>
	OpRoot<T,Index> operator()(const Sptr<Index>& i);

    protected:
	
	template <class Acc>
	iterator itLex(const Acc& acc);

	template <class Acc>
	iterator itLexSave(const Acc& acc);
    };

}

#endif
