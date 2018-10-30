
#ifndef __multi_array_base_h__
#define __multi_array_base_h__

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
	typedef ContainerRange<T,SRanges...> CRange;
	typedef ContainerIndex<T,typename SRanges::IndexType...> IndexType;

    protected:
	bool mInit = false;
	std::shared_ptr<CRange> mRange;
	std::shared_ptr<IndexType> mProtoI;

    public:

	DEFAULT_MEMBERS(MultiArrayBase);
	MultiArrayBase(const std::shared_ptr<SRanges>&... ranges);
	MultiArrayBase(const typename CRange::Space& space);

	virtual ~MultiArrayBase() = default;

	template <typename X>
	const T& operator[](const ContainerIndex<X,typename SRanges::IndexType...>& i);
	
	virtual const T& operator[](const IndexType& i) const = 0;
	virtual const T& at(const typename CRange::IndexType::MetaType& meta) const = 0;

	virtual const T* data() const = 0;
	
	virtual size_t size() const; 
	virtual bool isSlice() const = 0;

	virtual IndexType begin() const;
	virtual IndexType end() const;
	
	virtual IndexType beginIndex() const;
	virtual IndexType endIndex() const;

	virtual const std::shared_ptr<CRange>& range() const;

	virtual bool isConst() const;

	virtual std::shared_ptr<MultiArrayBase<T,AnonymousRange> > anonymous(bool slice = false) const = 0;

	virtual ConstOperationRoot<T,SRanges...>
	op(const std::shared_ptr<IndexType>& ind) const;
	
	virtual ConstOperationRoot<T,SRanges...>
	operator()(const std::shared_ptr<typename SRanges::IndexType>&... inds) const;

	template <class... MappedRanges>
	ConstOperationRoot<T,MappedRanges...>
	m(const std::shared_ptr<typename MappedRanges::IndexType>&... inds) const;

	virtual bool isInit() const;

	template <size_t N>
	auto getRangePtr() const
	    -> decltype(mRange->template getPtr<N>());
	
    };

    template <typename T, class... SRanges>
    class MutableMultiArrayBase : public MultiArrayBase<T,SRanges...>
    {
    public:

	typedef ContainerRange<T,SRanges...> CRange;
	typedef MultiArrayBase<T,SRanges...> MAB;
	typedef ContainerIndex<T,typename SRanges::IndexType...> IndexType;
	
	using MultiArrayBase<T,SRanges...>::operator[];
	using MultiArrayBase<T,SRanges...>::at;
	using MultiArrayBase<T,SRanges...>::data;
	using MultiArrayBase<T,SRanges...>::begin;
	using MultiArrayBase<T,SRanges...>::end;
	
	DEFAULT_MEMBERS(MutableMultiArrayBase);
	MutableMultiArrayBase(const std::shared_ptr<SRanges>&... ranges);
	MutableMultiArrayBase(const typename CRange::Space& space);

	template <typename X>
	T& operator[](const ContainerIndex<X,typename SRanges::IndexType...>& i);
	
	virtual T& operator[](const IndexType& i) = 0;
	virtual T& at(const typename CRange::IndexType::MetaType& meta) = 0;
	
	virtual T* data() = 0;
	
	//virtual IndexType begin();
	//virtual IndexType end();

	virtual bool isConst() const override;

	virtual std::shared_ptr<MultiArrayBase<T,AnonymousRange> > anonymousMove() = 0;
	
	virtual ConstOperationRoot<T,SRanges...>
	op(const std::shared_ptr<IndexType>& ind) const override;

	virtual ConstOperationRoot<T,SRanges...>
	operator()(const std::shared_ptr<typename SRanges::IndexType>&... inds) const override;

	virtual OperationRoot<T,SRanges...>
	op(const std::shared_ptr<IndexType>& ind);

	virtual OperationRoot<T,SRanges...> operator()(const std::shared_ptr<typename SRanges::IndexType>&... inds);

	template <class... MappedRanges>
	OperationRoot<T,MappedRanges...>
	m(const std::shared_ptr<typename MappedRanges::IndexType>&... inds);

	template <class... MappedRanges>
	ConstOperationRoot<T,MappedRanges...>
	m(const std::shared_ptr<typename MappedRanges::IndexType>&... inds) const;

    };

    
} // end namespace MultiArrayTools

/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */

    
#endif
